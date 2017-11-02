#pragma once

#include "inner_pre.h"
#include "event_loop.h"

#include <thread>
#include <mutex>


struct event_base;
struct event;

namespace Evp
{
	class EventLoop;

	class  EventLoopThread : public ServerStatus
	{
	public:
		enum { kOK = 0 };

		// Return 0 means OK, anything else means failed.
		typedef std::function<int()> Functor;

		EventLoopThread();
		~EventLoopThread();

		// @param wait_thread_started - If it is true this method will block
		//  until the thread totally started
		// @param pre - This functor will be executed immediately when the thread is started. 
		// @param post - This functor will be executed at the moment when the thread is going to stop.
		bool Start(bool bWaitThreadStarted = true,
			Functor pre = Functor(),
			Functor post = Functor());

		void Stop(bool bWaitThreadExit = false);

		// @brief Join the thread. If you forget to call this method,
		// it will be invoked automatically in the destruct method ~EventLoopThread().
		// @note DO NOT call this method from any of the working thread.
		void Join();

		void AfterFork();
	public:
		void SetName(const std::string& szName);
		const std::string& GetName() const;
		EventLoop* GetEvtLoop() const;
		struct event_base* GetEventBase();
		std::thread::id GetTid() const;
		bool IsRunning() const;

	private:
		void Run(const Functor& pre, const Functor& post);

	private:
		std::shared_ptr<EventLoop> m_pEventLoop;

		std::mutex m_mutex;
		std::shared_ptr<std::thread> m_pThread; // Guard by mutex_

		std::string m_szName;
	};
}
