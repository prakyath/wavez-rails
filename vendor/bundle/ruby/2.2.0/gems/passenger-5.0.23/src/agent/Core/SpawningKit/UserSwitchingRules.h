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
#ifndef _PASSENGER_SPAWNING_KIT_USER_SWITCHING_RULES_H_
#define _PASSENGER_SPAWNING_KIT_USER_SWITCHING_RULES_H_

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <boost/shared_array.hpp>
#include <oxt/backtrace.hpp>
#include <oxt/system_calls.hpp>
#include <Exceptions.h>
#include <Utils.h>
#include <Core/SpawningKit/Options.h>

namespace Passenger {
namespace SpawningKit {

using namespace std;
using namespace boost;
using namespace oxt;


struct UserSwitchingInfo {
	bool enabled;
	string username;
	string groupname;
	string home;
	string shell;
	uid_t uid;
	gid_t gid;
	int ngroups;
	shared_array<gid_t> gidset;
};

inline UserSwitchingInfo
prepareUserSwitching(const Options &options) {
	TRACE_POINT();
	UserSwitchingInfo info;

	if (geteuid() != 0) {
		struct passwd pwd, *userInfo;
		long bufSize;
		shared_array<char> strings;

		// _SC_GETPW_R_SIZE_MAX is not a maximum:
		// http://tomlee.co/2012/10/problems-with-large-linux-unix-groups-and-getgrgid_r-getgrnam_r/
		bufSize = std::max<long>(1024 * 128, sysconf(_SC_GETPW_R_SIZE_MAX));
		strings.reset(new char[bufSize]);

		userInfo = (struct passwd *) NULL;
		if (getpwuid_r(geteuid(), &pwd, strings.get(), bufSize, &userInfo) != 0
		 || userInfo == (struct passwd *) NULL)
		{
			throw RuntimeException("Cannot get user database entry for user " +
				getProcessUsername() + "; it looks like your system's " +
				"user database is broken, please fix it.");
		}

		info.enabled = false;
		info.username = userInfo->pw_name;
		info.groupname = getGroupName(userInfo->pw_gid);
		info.home = userInfo->pw_dir;
		info.shell = userInfo->pw_shell;
		info.uid = geteuid();
		info.gid = getegid();
		info.ngroups = 0;
		return info;
	}

	UPDATE_TRACE_POINT();
	string defaultGroup;
	string startupFile = absolutizePath(options.getStartupFile(),
		absolutizePath(options.appRoot));
	struct passwd pwd, *userInfo;
	struct group  grp;
	gid_t  groupId = (gid_t) -1;
	long pwdBufSize, grpBufSize;
	shared_array<char> pwdBuf, grpBuf;
	int ret;

	// _SC_GETPW_R_SIZE_MAX/_SC_GETGR_R_SIZE_MAX are not maximums:
	// http://tomlee.co/2012/10/problems-with-large-linux-unix-groups-and-getgrgid_r-getgrnam_r/
	pwdBufSize = std::max<long>(1024 * 128, sysconf(_SC_GETPW_R_SIZE_MAX));
	pwdBuf.reset(new char[pwdBufSize]);
	grpBufSize = std::max<long>(1024 * 128, sysconf(_SC_GETGR_R_SIZE_MAX));
	grpBuf.reset(new char[grpBufSize]);

	if (options.defaultGroup.empty()) {
		struct passwd *info;
		struct group *group;

		info = (struct passwd *) NULL;
		ret = getpwnam_r(options.defaultUser.c_str(), &pwd, pwdBuf.get(),
			pwdBufSize, &info);
		if (ret != 0) {
			info = (struct passwd *) NULL;
		}
		if (info == (struct passwd *) NULL) {
			throw RuntimeException("Cannot get user database entry for username '" +
				options.defaultUser + "'");
		}

		group = (struct group *) NULL;
		ret = getgrgid_r(info->pw_gid, &grp, grpBuf.get(), grpBufSize, &group);
		if (ret != 0) {
			group = (struct group *) NULL;
		}
		if (group == (struct group *) NULL) {
			throw RuntimeException(string("Cannot get group database entry for ") +
				"the default group belonging to username '" +
				options.defaultUser + "'");
		}
		defaultGroup = group->gr_name;
	} else {
		defaultGroup = options.defaultGroup;
	}

	UPDATE_TRACE_POINT();
	userInfo = (struct passwd *) NULL;
	if (!options.userSwitching) {
		// Keep userInfo at NULL so that it's set to defaultUser's UID.
	} else if (!options.user.empty()) {
		ret = getpwnam_r(options.user.c_str(), &pwd, pwdBuf.get(),
			pwdBufSize, &userInfo);
		if (ret != 0) {
			userInfo = (struct passwd *) NULL;
		}
	} else {
		struct stat buf;
		if (syscalls::lstat(startupFile.c_str(), &buf) == -1) {
			int e = errno;
			throw SystemException("Cannot lstat(\"" + startupFile +
				"\")", e);
		}
		ret = getpwuid_r(buf.st_uid, &pwd, pwdBuf.get(),
			pwdBufSize, &userInfo);
		if (ret != 0) {
			userInfo = (struct passwd *) NULL;
		}
	}
	if (userInfo == (struct passwd *) NULL || userInfo->pw_uid == 0) {
		userInfo = (struct passwd *) NULL;
		ret = getpwnam_r(options.defaultUser.c_str(), &pwd,
			pwdBuf.get(), pwdBufSize, &userInfo);
		if (ret != 0) {
			userInfo = (struct passwd *) NULL;
		}
	}

	UPDATE_TRACE_POINT();
	if (!options.userSwitching) {
		// Keep groupId at -1 so that it's set to defaultGroup's GID.
	} else if (!options.group.empty()) {
		struct group *groupInfo = (struct group *) NULL;

		if (options.group == "!STARTUP_FILE!") {
			struct stat buf;

			if (syscalls::lstat(startupFile.c_str(), &buf) == -1) {
				int e = errno;
				throw SystemException("Cannot lstat(\"" +
					startupFile + "\")", e);
			}

			ret = getgrgid_r(buf.st_gid, &grp, grpBuf.get(), grpBufSize,
				&groupInfo);
			if (ret != 0) {
				groupInfo = (struct group *) NULL;
			}
			if (groupInfo != NULL) {
				groupId = buf.st_gid;
			} else {
				groupId = (gid_t) -1;
			}
		} else {
			ret = getgrnam_r(options.group.c_str(), &grp, grpBuf.get(),
				grpBufSize, &groupInfo);
			if (ret != 0) {
				groupInfo = (struct group *) NULL;
			}
			if (groupInfo != NULL) {
				groupId = groupInfo->gr_gid;
			} else {
				groupId = (gid_t) -1;
			}
		}
	} else if (userInfo != (struct passwd *) NULL) {
		groupId = userInfo->pw_gid;
	}
	if (groupId == 0 || groupId == (gid_t) -1) {
		groupId = lookupGid(defaultGroup);
	}

	UPDATE_TRACE_POINT();
	if (userInfo == (struct passwd *) NULL) {
		throw RuntimeException("Cannot determine a user to lower privilege to");
	}
	if (groupId == (gid_t) -1) {
		throw RuntimeException("Cannot determine a group to lower privilege to");
	}

	UPDATE_TRACE_POINT();
	#ifdef __APPLE__
		int groups[1024];
		info.ngroups = sizeof(groups) / sizeof(int);
	#else
		gid_t groups[1024];
		info.ngroups = sizeof(groups) / sizeof(gid_t);
	#endif
	info.enabled = true;
	info.username = userInfo->pw_name;
	info.groupname = getGroupName(groupId);
	info.home = userInfo->pw_dir;
	info.shell = userInfo->pw_shell;
	info.uid = userInfo->pw_uid;
	info.gid = groupId;
	#if !defined(HAVE_GETGROUPLIST) && (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__))
		#define HAVE_GETGROUPLIST
	#endif
	#ifdef HAVE_GETGROUPLIST
		ret = getgrouplist(userInfo->pw_name, groupId,
			groups, &info.ngroups);
		if (ret == -1) {
			int e = errno;
			throw SystemException("getgrouplist() failed", e);
		}
		info.gidset = shared_array<gid_t>(new gid_t[info.ngroups]);
		for (int i = 0; i < info.ngroups; i++) {
			info.gidset[i] = groups[i];
		}
	#endif

	return info;
}


} // namespace SpawningKit
} // namespace Passenger

#endif /* _PASSENGER_SPAWNING_KIT_USER_SWITCHING_RULES_H_ */
