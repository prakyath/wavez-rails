/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#ifndef _PASSENGER_CORE_CONTROLLER_H_
#define _PASSENGER_CORE_CONTROLLER_H_


//#define DEBUG_CC_EVENT_LOOP_BLOCKING

#define CC_BENCHMARK_POINT(client, req, value) \
	do { \
		if (OXT_UNLIKELY(benchmarkMode == value)) { \
			writeBenchmarkResponse(&client, &req); \
			return; \
		} \
	} while (false)


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/cstdint.hpp>
#include <oxt/macros.hpp>
#include <ev++.h>
#include <ostream>

#if defined(__GLIBCXX__) || defined(__APPLE__)
	#include <cxxabi.h>
	#define CXX_ABI_API_AVAILABLE
#endif

#include <sys/types.h>
#include <sys/uio.h>
#include <utility>
#include <typeinfo>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <cctype>

#include <Logging.h>
#include <MessageReadersWriters.h>
#include <Constants.h>
#include <ServerKit/Errors.h>
#include <ServerKit/HttpServer.h>
#include <ServerKit/HttpHeaderParser.h>
#include <MemoryKit/palloc.h>
#include <DataStructures/LString.h>
#include <DataStructures/StringKeyTable.h>
#include <StaticString.h>
#include <Utils.h>
#include <Utils/StrIntUtils.h>
#include <Utils/IOUtils.h>
#include <Utils/JsonUtils.h>
#include <Utils/HttpConstants.h>
#include <Utils/VariantMap.h>
#include <Utils/Timer.h>
#include <Core/ApplicationPool/ErrorRenderer.h>
#include <Core/Controller/Client.h>
#include <Core/Controller/AppResponse.h>
#include <Core/Controller/TurboCaching.h>
#include <Core/UnionStation/Context.h>

namespace Passenger {

using namespace std;
using namespace boost;
using namespace oxt;
using namespace ApplicationPool2;


namespace ServerKit {
	extern const HashedStaticString HTTP_COOKIE;
	extern const HashedStaticString HTTP_SET_COOKIE;
}

namespace Core {


class Controller: public ServerKit::HttpServer<Controller, Client> {
public:
	enum BenchmarkMode {
		BM_NONE,
		BM_AFTER_ACCEPT,
		BM_BEFORE_CHECKOUT,
		BM_AFTER_CHECKOUT,
		BM_RESPONSE_BEGIN,
		BM_UNKNOWN
	};

private:
	typedef ServerKit::HttpServer<Controller, Client> ParentClass;
	typedef ServerKit::Channel Channel;
	typedef ServerKit::FdSinkChannel FdSinkChannel;
	typedef ServerKit::FdSourceChannel FdSourceChannel;
	typedef ServerKit::FileBufferedChannel FileBufferedChannel;
	typedef ServerKit::FileBufferedFdSinkChannel FileBufferedFdSinkChannel;

	static const unsigned int MAX_SESSION_CHECKOUT_TRY = 10;

	unsigned int statThrottleRate;
	unsigned int responseBufferHighWatermark;
	BenchmarkMode benchmarkMode: 3;
	bool singleAppMode: 1;
	bool showVersionInHeader: 1;
	bool stickySessions: 1;
	bool gracefulExit: 1;

	const VariantMap *agentsOptions;
	psg_pool_t *stringPool;
	StringKeyTable< boost::shared_ptr<Options> > poolOptionsCache;

	StaticString defaultRuby;
	StaticString ustRouterAddress;
	StaticString ustRouterPassword;
	StaticString defaultUser;
	StaticString defaultGroup;
	StaticString defaultServerName;
	StaticString defaultServerPort;
	StaticString serverSoftware;
	StaticString defaultStickySessionsCookieName;
	StaticString defaultVaryTurbocacheByCookie;

	HashedStaticString PASSENGER_APP_GROUP_NAME;
	HashedStaticString PASSENGER_ENV_VARS;
	HashedStaticString PASSENGER_MAX_REQUESTS;
	HashedStaticString PASSENGER_STICKY_SESSIONS;
	HashedStaticString PASSENGER_STICKY_SESSIONS_COOKIE_NAME;
	HashedStaticString PASSENGER_REQUEST_OOB_WORK;
	HashedStaticString UNION_STATION_SUPPORT;
	HashedStaticString REMOTE_ADDR;
	HashedStaticString REMOTE_PORT;
	HashedStaticString REMOTE_USER;
	HashedStaticString FLAGS;
	HashedStaticString HTTP_COOKIE;
	HashedStaticString HTTP_DATE;
	HashedStaticString HTTP_HOST;
	HashedStaticString HTTP_CONTENT_LENGTH;
	HashedStaticString HTTP_CONTENT_TYPE;
	HashedStaticString HTTP_EXPECT;
	HashedStaticString HTTP_CONNECTION;
	HashedStaticString HTTP_STATUS;
	HashedStaticString HTTP_TRANSFER_ENCODING;

	unsigned int threadNumber;
	StaticString serverLogName;

	friend class TurboCaching<Request>;
	friend class ResponseCache<Request>;
	struct ev_check checkWatcher;
	TurboCaching<Request> turboCaching;

	#ifdef DEBUG_CC_EVENT_LOOP_BLOCKING
		struct ev_prepare prepareWatcher;
		ev_tstamp timeBeforeBlocking;
	#endif


	/****** Stage: initiatelize request ******/

	struct RequestAnalysis;

	void initializeFlags(Client *client, Request *req, RequestAnalysis &analysis);
	bool respondFromTurboCache(Client *client, Request *req);
	void initializePoolOptions(Client *client, Request *req, RequestAnalysis &analysis);
	void fillPoolOptionsFromAgentsOptions(Options &options);
	static void fillPoolOption(Request *req, StaticString &field,
		const HashedStaticString &name);
	static void fillPoolOption(Request *req, int &field,
		const HashedStaticString &name);
	static void fillPoolOption(Request *req, bool &field,
		const HashedStaticString &name);
	static void fillPoolOption(Request *req, unsigned int &field,
		const HashedStaticString &name);
	static void fillPoolOption(Request *req, unsigned long &field,
		const HashedStaticString &name);
	static void fillPoolOption(Request *req, long &field,
		const HashedStaticString &name);
	static void fillPoolOptionSecToMsec(Request *req, unsigned int &field,
		const HashedStaticString &name);
	void createNewPoolOptions(Client *client, Request *req,
		const HashedStaticString &appGroupName);
	void initializeUnionStation(Client *client, Request *req, RequestAnalysis &analysis);
	void setStickySessionId(Client *client, Request *req);
	const LString *getStickySessionCookieName(Request *req);


	/****** Stage: buffering body ******/

	void beginBufferingBody(Client *client, Request *req);
	Channel::Result whenBufferingBody_onRequestBody(Client *client, Request *req,
		const MemoryKit::mbuf &buffer, int errcode);


	/****** Stage: checkout session ******/

	void checkoutSession(Client *client, Request *req);
	static void sessionCheckedOut(const SessionPtr &session,
		const ExceptionPtr &e, void *userData);
	void sessionCheckedOutFromAnotherThread(Client *client, Request *req,
		SessionPtr session, ExceptionPtr e);
	void sessionCheckedOutFromEventLoopThread(Client *client, Request *req,
		const SessionPtr &session, const ExceptionPtr &e);
	void maybeSend100Continue(Client *client, Request *req);
	void initiateSession(Client *client, Request *req);
	static void checkoutSessionLater(Request *req);
	void reportSessionCheckoutError(Client *client, Request *req,
		const ExceptionPtr &e);
	void writeRequestQueueFullExceptionErrorResponse(Client *client,
		Request *req, const boost::shared_ptr<RequestQueueFullException> &e);
	void writeSpawnExceptionErrorResponse(Client *client, Request *req,
		const boost::shared_ptr<SpawnException> &e);
	void writeOtherExceptionErrorResponse(Client *client, Request *req,
		const ExceptionPtr &e);
	void endRequestWithErrorResponse(Client **c, Request **r,
		const StaticString &message, const SpawnException *e = NULL);
	bool friendlyErrorPagesEnabled(Request *req);


	/****** Stage: send request to application ******/

	struct SessionProtocolWorkingState;
	struct HttpHeaderConstructionCache;

	void sendHeaderToApp(Client *client, Request *req);
	void sendHeaderToAppWithSessionProtocol(Client *client, Request *req);
	static void sendBodyToAppWhenAppSinkIdle(Channel *_channel, unsigned int size);
	unsigned int determineHeaderSizeForSessionProtocol(Request *req,
		SessionProtocolWorkingState &state, string delta_monotonic);
	bool constructHeaderForSessionProtocol(Request *req, char * restrict buffer,
		unsigned int &size, const SessionProtocolWorkingState &state, string delta_monotonic);
	void sendHeaderToAppWithHttpProtocol(Client *client, Request *req);
	bool constructHeaderBuffersForHttpProtocol(Request *req, struct iovec *buffers,
		unsigned int maxbuffers, unsigned int & restrict_ref nbuffers,
		unsigned int & restrict_ref dataSize, HttpHeaderConstructionCache &cache);
	bool sendHeaderToAppWithHttpProtocolAndWritev(Request *req, ssize_t &bytesWritten,
		HttpHeaderConstructionCache &cache);
	void sendHeaderToAppWithHttpProtocolWithBuffering(Request *req, unsigned int offset,
		HttpHeaderConstructionCache &cache);
	void sendBodyToApp(Client *client, Request *req);
	void halfCloseAppSink(Client *client, Request *req);
	Channel::Result whenSendingRequest_onRequestBody(Client *client, Request *req,
		const MemoryKit::mbuf &buffer, int errcode);
	static void resumeRequestBodyChannelWhenAppSinkIdle(Channel *_channel,
		unsigned int size);
	void startBodyChannel(Client *client, Request *req);
	void stopBodyChannel(Client *client, Request *req);
	void logAppSocketWriteError(Client *client, int errcode);


	/****** Stage: forward application response to client ******/

	static Channel::Result _onAppSourceData(Channel *_channel,
		const MemoryKit::mbuf &buffer, int errcode);
	Channel::Result onAppSourceData(Client *client, Request *req,
		const MemoryKit::mbuf &buffer, int errcode);
	void onAppResponseBegin(Client *client, Request *req);
	void prepareAppResponseCaching(Client *client, Request *req);
	void onAppResponse100Continue(Client *client, Request *req);
	bool constructHeaderBuffersForResponse(Request *req, struct iovec *buffers,
		unsigned int maxbuffers, unsigned int & restrict_ref nbuffers,
		unsigned int & restrict_ref dataSize,
		unsigned int & restrict_ref nCacheableBuffers);
	unsigned int constructDateHeaderBuffersForResponse(char *dateStr,
		unsigned int bufsize);
	bool sendResponseHeaderWithWritev(Client *client, Request *req,
		ssize_t &bytesWritten);
	void sendResponseHeaderWithBuffering(Client *client, Request *req,
		unsigned int offset);
	void logResponseHeaders(Client *client, Request *req, struct iovec *buffers,
		unsigned int nbuffers, unsigned int dataSize);
	void markHeaderBuffersForTurboCaching(Client *client, Request *req,
		struct iovec *buffers, unsigned int nbuffers);
	static ServerKit::HttpHeaderParser<AppResponse, ServerKit::HttpParseResponse>
		createAppResponseHeaderParser(ServerKit::Context *ctx, Request *req);
	static ServerKit::HttpChunkedBodyParser createAppResponseChunkedBodyParser(
		Request *req);
	static unsigned int formatAppResponseChunkedBodyParserLoggingPrefix(char *buf,
		unsigned int bufsize, void *userData);
	void prepareAppResponseChunkedBodyParsing(Client *client, Request *req);
	void writeResponseAndMarkForTurboCaching(Client *client, Request *req,
		const MemoryKit::mbuf &buffer);
	void markResponsePartForTurboCaching(Client *client, Request *req,
		const MemoryKit::mbuf &buffer);
	void maybeThrottleAppSource(Client *client, Request *req);
	static void _outputBuffersFlushed(FileBufferedChannel *_channel);
	void outputBuffersFlushed(Client *client, Request *req);
	static void _outputDataFlushed(FileBufferedChannel *_channel);
	void outputDataFlushed(Client *client, Request *req);
	void handleAppResponseBodyEnd(Client *client, Request *req);
	OXT_FORCE_INLINE void keepAliveAppConnection(Client *client, Request *req);
	void storeAppResponseInTurboCache(Client *client, Request *req);
	void finalizeUnionStationWithSuccess(Client *client, Request *req);


	/***** Hooks ******/

	static Channel::Result onBodyBufferData(Channel *_channel,
		const MemoryKit::mbuf &buffer, int errcode);
	#ifdef DEBUG_CC_EVENT_LOOP_BLOCKING
		static void onEventLoopPrepare(EV_P_ struct ev_prepare *w, int revents);
	#endif
	static void onEventLoopCheck(EV_P_ struct ev_check *w, int revents);


	/****** Internal utility functions ******/

	static TurboCaching<Request>::State getTurboCachingInitialState(
		const VariantMap *agentsOptions);
	void generateServerLogName(unsigned int number);
	void disconnectWithClientSocketWriteError(Client **client, int e);
	void disconnectWithAppSocketIncompleteResponseError(Client **client);
	void disconnectWithAppSocketReadError(Client **client, int e);
	void disconnectWithAppSocketWriteError(Client **client, int e);
	void endRequestWithAppSocketIncompleteResponse(Client **client,
		Request **req);
	void endRequestWithAppSocketReadError(Client **client, Request **req,
		int e);
	void endRequestWithSimpleResponse(Client **c, Request **r,
		const StaticString &body, int code = 200);
	void endRequestAsBadGateway(Client **client, Request **req);
	void writeBenchmarkResponse(Client **client, Request **req,
		bool end = true);
	bool getBoolOption(Request *req, const HashedStaticString &name,
		bool defaultValue = false);
	template<typename Number> static Number clamp(Number value,
		Number min, Number max);
	static void gatherBuffers(char * restrict dest, unsigned int size,
		const struct iovec *buffers, unsigned int nbuffers);
	static LString *resolveSymlink(const StaticString &path, psg_pool_t *pool);
	void parseCookieHeader(psg_pool_t *pool, const LString *headerValue,
		vector< pair<StaticString, StaticString> > &cookies) const;
	#ifdef DEBUG_CC_EVENT_LOOP_BLOCKING
		void reportLargeTimeDiff(Client *client, const char *name,
			ev_tstamp fromTime, ev_tstamp toTime);
	#endif


protected:
	/****** Stage: initiatelize request ******/

	virtual void onRequestBegin(Client *client, Request *req);


	/****** Hooks ******/

	virtual void onClientAccepted(Client *client);
	virtual void onRequestObjectCreated(Client *client, Request *req);
	virtual void deinitializeClient(Client *client);
	virtual void reinitializeRequest(Client *client, Request *req);
	virtual void deinitializeRequest(Client *client, Request *req);
	void reinitializeAppResponse(Client *client, Request *req);
	void deinitializeAppResponse(Client *client, Request *req);
	virtual Channel::Result onRequestBody(Client *client, Request *req,
		const MemoryKit::mbuf &buffer, int errcode);
	virtual bool shouldDisconnectClientOnShutdown(Client *client);
	virtual bool supportsUpgrade(Client *client, Request *req);


public:
	ResourceLocator *resourceLocator;
	PoolPtr appPool;
	UnionStation::ContextPtr unionStationContext;


	/****** Initialization and shutdown ******/

	Controller(ServerKit::Context *context, const VariantMap *_agentsOptions,
		unsigned int _threadNumber = 1);
	~Controller();
	void initialize();


	/****** Hooks ******/

	virtual unsigned int getClientName(const Client *client, char *buf, size_t size) const;
	virtual StaticString getServerName() const;


	/****** State inspection and configuration ******/

	virtual Json::Value getConfigAsJson() const;
	virtual void configure(const Json::Value &doc);
	virtual Json::Value inspectStateAsJson() const;
	virtual Json::Value inspectClientStateAsJson(const Client *client) const;
	virtual Json::Value inspectRequestStateAsJson(const Request *req) const;


	/****** Miscellaneous *******/

	static BenchmarkMode parseBenchmarkMode(const StaticString mode);
	void disconnectLongRunningConnections(const StaticString &gupid);
};


} // namespace Core
} // namespace Passenger

#endif /* _PASSENGER_CORE_CONTROLLER_H_ */
