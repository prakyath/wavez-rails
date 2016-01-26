/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014-2015 Phusion Holding B.V.
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
#include <Core/Controller.h>

/*************************************************************************
 *
 * State inspection functions for Core::Controller
 *
 *************************************************************************/

namespace Passenger {
namespace Core {

using namespace std;
using namespace boost;


/****************************
 *
 * Public methods
 *
 ****************************/


Json::Value
Controller::getConfigAsJson() const {
	Json::Value doc = ParentClass::getConfigAsJson();
	doc["single_app_mode"] = singleAppMode;
	doc["stat_throttle_rate"] = statThrottleRate;
	doc["show_version_in_header"] = showVersionInHeader;
	doc["data_buffer_dir"] = getContext()->defaultFileBufferedChannelConfig.bufferDir;
	return doc;
}

void
Controller::configure(const Json::Value &doc) {
	ParentClass::configure(doc);
	if (doc.isMember("show_version_in_header")) {
		showVersionInHeader = doc["show_version_in_header"].asBool();
	}
	if (doc.isMember("data_buffer_dir")) {
		getContext()->defaultFileBufferedChannelConfig.bufferDir =
			doc["data_buffer_dir"].asString();
	}
}

Json::Value
Controller::inspectStateAsJson() const {
	Json::Value doc = ParentClass::inspectStateAsJson();
	if (turboCaching.isEnabled()) {
		Json::Value subdoc;
		subdoc["fetches"] = turboCaching.responseCache.getFetches();
		subdoc["hits"] = turboCaching.responseCache.getHits();
		subdoc["hit_ratio"] = turboCaching.responseCache.getHitRatio();
		subdoc["stores"] = turboCaching.responseCache.getStores();
		subdoc["store_successes"] = turboCaching.responseCache.getStoreSuccesses();
		subdoc["store_success_ratio"] = turboCaching.responseCache.getStoreSuccessRatio();
		doc["turbocaching"] = subdoc;
	}
	return doc;
}

Json::Value
Controller::inspectClientStateAsJson(const Client *client) const {
	Json::Value doc = ParentClass::inspectClientStateAsJson(client);
	doc["connected_at"] = timeToJson(client->connectedAt * 1000000.0);
	return doc;
}

Json::Value
Controller::inspectRequestStateAsJson(const Request *req) const {
	Json::Value doc = ParentClass::inspectRequestStateAsJson(req);
	Json::Value flags;
	const AppResponse *resp = &req->appResponse;

	if (req->startedAt != 0) {
		doc["started_at"] = timeToJson(req->startedAt * 1000000.0);
	}
	doc["state"] = req->getStateString();
	if (req->stickySession) {
		doc["sticky_session_id"] = req->options.stickySessionId;
	}
	doc["sticky_session"] = req->stickySession;
	doc["session_checkout_try"] = req->sessionCheckoutTry;

	flags["dechunk_response"] = req->dechunkResponse;
	flags["request_body_buffering"] = req->requestBodyBuffering;
	flags["https"] = req->https;
	doc["flags"] = flags;

	if (req->requestBodyBuffering) {
		doc["body_bytes_buffered"] = byteSizeToJson(req->bodyBytesBuffered);
	}

	if (req->session != NULL) {
		Json::Value &sessionDoc = doc["session"] = Json::Value(Json::objectValue);
		const Session *session = req->session.get();

		if (req->session->isClosed()) {
			sessionDoc["closed"] = true;
		} else {
			sessionDoc["pid"] = (Json::Int64) session->getPid();
			sessionDoc["gupid"] = session->getGupid().toString();
		}
	}

	if (req->appResponseInitialized) {
		doc["app_response_http_state"] = resp->getHttpStateString();
		if (resp->begun()) {
			doc["app_response_http_major"] = resp->httpMajor;
			doc["app_response_http_minor"] = resp->httpMinor;
			doc["app_response_want_keep_alive"] = resp->wantKeepAlive;
			doc["app_response_body_type"] = resp->getBodyTypeString();
			doc["app_response_body_fully_read"] = resp->bodyFullyRead();
			doc["app_response_body_already_read"] = byteSizeToJson(
				resp->bodyAlreadyRead);
			if (resp->httpState != AppResponse::ERROR) {
				if (resp->bodyType == AppResponse::RBT_CONTENT_LENGTH) {
					doc["app_response_content_length"] = byteSizeToJson(
						resp->aux.bodyInfo.contentLength);
				} else if (resp->bodyType == AppResponse::RBT_CHUNKED) {
					doc["app_response_end_chunk_reached"] = resp->aux.bodyInfo.endChunkReached;
				}
			} else {
				doc["app_response_parse_error"] = ServerKit::getErrorDesc(resp->aux.parseError);
			}
		}
	}

	doc["app_source_state"] = req->appSource.inspectAsJson();
	doc["app_sink_state"] = req->appSink.inspectAsJson();

	return doc;
}


} // namespace Core
} // namespace Passenger
