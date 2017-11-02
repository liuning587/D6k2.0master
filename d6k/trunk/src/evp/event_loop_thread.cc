#include "inner_pre.h"

#include "event_loop.h"
#include "event_loop_thread.h"
#include "logging.h"

#include <string>
#include <sstream>

namespace Evp
{

	EventLoopThread::EventLoopThread(): m_pEventLoop(new EventLoop) 
	{
		DLOG_TRACE << "loop=" << m_pEventLoop;
	}

	EventLoopThread::~EventLoopThread() 
	{
		DLOG_TRACE << "loop=" << m_pEventLoop;
		assert(IsStopped());
		Join();
	}

	bool EventLoopThread::Start(bool bWaitThreadStarted, Functor pre, Functor post) 
	{
		DLOG_TRACE;
		m_nStatus = kStarting;

		assert(m_pThread.get() == nullptr);
		m_pThread.reset(new std::thread(std::bind(&EventLoopThread::Run, this, pre, post)));

		if (bWaitThreadStarted) 
		{
			while (m_nStatus < kRunning) 
			{
				evp_usleep(1);
			}
		}
		return true;
	}

	void EventLoopThread::Run(const Functor& pre, const Functor& post) 
	{
		DLOG_TRACE << "loop=" << m_pEventLoop;
		if (m_szName.empty()) 
		{
			std::ostringstream os;
			os << "Thread-" << std::this_thread::get_id();
			m_szName = os.str();
		}

		DLOG_TRACE << "loop=" << m_pEventLoop << " execute pre functor.";
		auto fn = [this, pre] () 
		{
			m_nStatus = kRunning;
			if (pre)
			{
				auto rc = pre();
				if (rc != kOK) 
				{
					m_pEventLoop->Stop();
				}
			}
		};
		m_pEventLoop->QueueInLoop(std::move(fn));
		m_pEventLoop->Run();

		DLOG_TRACE << "loop=" << m_pEventLoop << " execute post functor.";
		if (post) 
		{
			post();
		}

		assert(m_pEventLoop->IsStopped());
		DLOG_TRACE << "loop=" << m_pEventLoop << " EventLoopThread stopped";
		m_nStatus = kStopped;
	}

	void EventLoopThread::Stop(bool bWaitThreadExit)
	{
		DLOG_TRACE << "loop=" << m_pEventLoop << " wait_thread_exit=" << bWaitThreadExit;
		assert(m_nStatus == kRunning && IsRunning());
		m_nStatus = kStopping;
		m_pEventLoop->Stop();

		if (bWaitThreadExit) 
		{
			while (!IsStopped()) 
			{
				evp_usleep(1);
			}

			DLOG_TRACE << "loop=" << m_pEventLoop << " thread stopped.";
			Join();
			DLOG_TRACE << "loop=" << m_pEventLoop << " thread totally stopped.";
		}
		DLOG_TRACE << "loop=" << m_pEventLoop;
	}

	void EventLoopThread::Join()
	{
		// To avoid multi other threads call Join simultaneously
		std::lock_guard<std::mutex> guard(m_mutex);
		if (m_pThread && m_pThread->joinable()) 
		{
		 	DLOG_TRACE << "loop=" << m_pEventLoop << " thread=" << m_pThread << " joinable";
			try 
			{
				m_pThread->join();
			}
			catch (const std::system_error& e) 
			{
				LOG_ERROR << "Caught a system_error:" << e.what() << " code=" << e.code();
			}
			m_pThread.reset();
		}
	}

	void EventLoopThread::SetName(const std::string& szName) 
	{
		m_szName = szName;
	}

	const std::string& EventLoopThread::GetName() const 
	{
		return m_szName;
	}

	EventLoop* EventLoopThread::GetEvtLoop() const 
	{
		return m_pEventLoop.get();
	}
	
	struct event_base* EventLoopThread::GetEventBase()
	{
		return GetEvtLoop()->GetEventBase();
	}

	std::thread::id EventLoopThread::GetTid() const
	{
		if (m_pThread)
		{
			return m_pThread->get_id();
		}

		return std::thread::id();
	}

	bool EventLoopThread::IsRunning() const 
	{
		// Using m_pEventLoop->IsRunning() is more exact to query where thread is
		// running or not instead of m_nStatus == kRunning
		//
		// Because in some particular circumstances,
		// when m_nStatus==kRunning and m_pEventLoop::running_ == false,
		// the application will broke down
		return m_pEventLoop->IsRunning();
	}

	void EventLoopThread::AfterFork() 
	{
		GetEvtLoop()->AfterFork();
	}

}
