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
#ifndef _PASSENGER_APPLICATION_POOL2_CONTEXT_H_
#define _PASSENGER_APPLICATION_POOL2_CONTEXT_H_

#include <boost/thread.hpp>
#include <boost/pool/object_pool.hpp>
#include <Exceptions.h>
#include <Utils/ClassUtils.h>
#include <Core/SpawningKit/Factory.h>

namespace Passenger {
namespace ApplicationPool2 {


using namespace boost;

class Session;
class Process;


/**
 * State shared by Pool, Group, Process and Session. It contains statistics
 * and counters, memory management objects, configuration objects, etc.
 * This struct was introduced so that Group, Process and Sessions don't have
 * to depend on Pool (which introduces circular dependencies).
 *
 * The fields are separated in several groups. Each group may have its own mutex.
 * If it does, then all operations on any of the fields in that group requires
 * grabbing the mutex unless documented otherwise.
 */
class Context {
private:
	/****** Memory management objects *****/

	P_RO_PROPERTY_REF(private, boost::mutex, MmSyncher);
	P_RO_PROPERTY_REF(private, object_pool<Session>, SessionObjectPool);
	P_RO_PROPERTY_REF(private, object_pool<Process>, ProcessObjectPool);


	/****** Configuration objects ******/

	P_PROPERTY_CONST_REF(private, SpawningKit::FactoryPtr, SpawningKitFactory);


public:
	/****** Initialization ******/

	Context()
		: mSessionObjectPool(64, 1024),
		  mProcessObjectPool(4, 64)
		{ }

	void finalize() {
		if (mSpawningKitFactory == NULL) {
			throw RuntimeException("spawningKitFactory must be set");
		}
	}


	/****** Configuration objects ******/

	const SpawningKit::ConfigPtr &getSpawningKitConfig() const {
		return mSpawningKitFactory->getConfig();
	}
};


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL2_CONTEXT_H_ */
