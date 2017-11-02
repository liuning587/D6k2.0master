#include "inner_pre.h"

#include "libevent.h"

#ifndef H_OS_WINDOWS
#include <signal.h>
#endif

#include <map>
#include <thread>
#include <mutex>

namespace Evp
{
	namespace
	{
		struct OnStartup
		{
			OnStartup()
			{
#ifndef H_OS_WINDOWS
				if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
				{
					//	LOG_ERROR << "SIGPIPE set failed.";
					exit(-1);
				}
				//LOG_INFO << "ignore SIGPIPE";
#endif
			}
			~OnStartup()
			{
			}
		} __s_onstartup;
	}

	static const std::string empty_string;

	std::string strerror(int e) 
	{
#ifdef H_OS_WINDOWS
		LPVOID buf = nullptr;
		::FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, e, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buf, 0, nullptr);

		if (buf) {
			std::string s = (char*)buf;
			LocalFree(buf);
			return s;
		}

		return empty_string;
#else
		char buf[2048] = {};
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
		int rc = strerror_r(e, buf, sizeof(buf) - 1); // XSI-compliant
		if (rc == 0)
		{
			return std::string(buf);
		}
#else
		const char* s = strerror_r(e, buf, sizeof(buf) - 1); // GNU-specific
		if (s) 
		{
			return std::string(s);
		}
#endif
		return std::string();
#endif
}

#ifdef H_DEBUG_MODE
	static std::map<struct event*, std::thread::id> evmap;
	static std::mutex mutex;
#endif

	int EventAdd(struct event* ev, const struct timeval* timeout)
	{
#ifdef H_DEBUG_MODE
		{
			std::lock_guard<std::mutex> guard(mutex);
			if (evmap.find(ev) == evmap.end())
			{
				auto id = std::this_thread::get_id();
				evmap[ev] = id;
			}
			else
			{
				//    LOG_ERROR << "Event " << ev << " fd=" << ev->ev_fd << " event_add twice!";
				assert("event_add twice");
			}
		}
		LOG_DEBUG << "event_add ev=" << ev << " fd=" << ev->ev_fd << " user_ptr=" << ev->ev_arg << " tid=" << std::this_thread::get_id();
#endif
		return event_add(ev, timeout);
	}

	int EventDel(struct event* ev)
	{
#ifdef H_DEBUG_MODE
		{
			std::lock_guard<std::mutex> guard(mutex);
			auto it = evmap.find(ev);
			if (it == evmap.end())
			{
				//    LOG_ERROR << "Event " << ev << " fd=" << ev->ev_fd << " not exist in event loop, maybe event_del twice.";
				assert("event_del twice");
			}
			else
			{
				auto id = std::this_thread::get_id();
				if (id != it->second)
				{
					//    LOG_ERROR << "Event " << ev << " fd=" << ev->ev_fd << " deleted in different thread.";
					assert(it->second == id);
				}
				evmap.erase(it);
			}
		}
		LOG_DEBUG << "event_del ev=" << ev << " fd=" << ev->ev_fd << " user_ptr=" << ev->ev_arg << " tid=" << std::this_thread::get_id();
#endif
		return event_del(ev);
	}

	int GetActiveEventCount()
	{
#ifdef H_DEBUG_MODE
		return evmap.size();
#else
		return 0;
#endif
	}

}
