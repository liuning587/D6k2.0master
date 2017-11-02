#include "inner_pre.h"
#include "event_loop_thread_pool.h"
#include "event_loop.h"
#include "logging.h"

namespace Evp
{
	EventLoopThreadPool::EventLoopThreadPool(EventLoop* pBaseLoop, uint32_t nThreadNumber)
		: m_pBaseLoop(pBaseLoop), m_nThreadNum(nThreadNumber)
	{
		DLOG_TRACE << "thread_num=" << GetThreadNum() << " base loop=" << m_pBaseLoop;
	}

	EventLoopThreadPool::~EventLoopThreadPool()
	{
		DLOG_TRACE << "Thread_num=" << GetThreadNum();
		Join();
		m_arrThreads.clear();
	}

	bool EventLoopThreadPool::Start(bool bWaitThreadStarted)
	{
		m_nStatus.store(kStarting);
		DLOG_TRACE << "Thread_num=" << GetThreadNum() << " base loop=" << m_pBaseLoop << " wait_thread_started=" << bWaitThreadStarted;

		if (m_nThreadNum == 0)
		{
			m_nStatus.store(kRunning);
			return true;
		}

		std::shared_ptr<std::atomic<uint32_t>> nStartedCount(new std::atomic<uint32_t>(0));
		std::shared_ptr<std::atomic<uint32_t>> nExitedCount(new std::atomic<uint32_t>(0));

		for (uint32_t i = 0; i < m_nThreadNum; ++i)
		{
			auto prefn = [this, nStartedCount] ()
			{
				DLOG_TRACE << "a working thread started tid=" << std::this_thread::get_id();
				this->OnThreadStarted(nStartedCount->fetch_add(1) + 1);
				return EventLoopThread::kOK;
			};

			auto postfn = [this, nExitedCount] ()
			{
				DLOG_TRACE << "a working thread exiting, tid=" << std::this_thread::get_id();
				this->OnThreadExited(nExitedCount->fetch_add(1) + 1);
				return EventLoopThread::kOK;
			};

			EventLoopThreadPtr t(new EventLoopThread());
			if (!t->Start(bWaitThreadStarted, prefn, postfn))
			{
				//FIXME error process
				LOG_ERROR << "start thread failed!";
				return false;
			}

			std::stringstream ss;
			ss << "EventLoopThreadPool-thread-" << i << "th";
			t->SetName(ss.str());
			m_arrThreads.push_back(t);
		}

		// when all the working thread have started,
		// status_ will be stored with kRunning in method OnThreadStarted

		if (bWaitThreadStarted)
		{
			while (!IsRunning())
			{
				evp_usleep(1);
			}
			assert(m_nStatus.load() == kRunning);
		}

		return true;
	}

	void EventLoopThreadPool::Stop(bool bWaitThreadExited)
	{
		DLOG_TRACE << "wait_thread_exit=" << bWaitThreadExited;
		Stop(bWaitThreadExited, DoneCallback());
	}

	void EventLoopThreadPool::Stop(DoneCallback fn)
	{
		DLOG_TRACE;
		Stop(false, fn);
	}

	void EventLoopThreadPool::Stop(bool bWaitThreadExit, DoneCallback fn)
	{
		DLOG_TRACE;
		m_nStatus.store(kStopping);

		if (m_nThreadNum == 0)
		{
			m_nStatus.store(kStopped);

			if (fn)
			{
				DLOG_TRACE << "calling stopped callback";
				fn();
			}
			return;
		}

		DLOG_TRACE << "wait_thread_exit=" << bWaitThreadExit;
		m_fnStoppedCallBack = fn;

		for (auto &t : m_arrThreads)
		{
			t->Stop();
		}

		// when all the working thread have stopped
		// status_ will be stored with kStopped in method OnThreadExited

		auto is_stopped_fn = [this] ()
		{
			for (auto &t : this->m_arrThreads)
			{
				if (!t->IsStopped())
				{
					return false;
				}
			}
			return true;
		};

		DLOG_TRACE << "before promise wait";
		if (m_nThreadNum > 0 && bWaitThreadExit)
		{
			while (!is_stopped_fn())
			{
				evp_usleep(1);
			}
		}
		DLOG_TRACE << "after promise wait";

		m_nStatus.store(kStopped);
	}

	void EventLoopThreadPool::Join()
	{
		DLOG_TRACE << "thread_num=" << GetThreadNum();
		for (auto &t : m_arrThreads)
		{
			t->Join();
		}
		m_arrThreads.clear();
	}

	void EventLoopThreadPool::AfterFork()
	{
		DLOG_TRACE << "thread_num=" << GetThreadNum();
		for (auto &t : m_arrThreads)
		{
			t->AfterFork();
		}
	}

	EventLoop* EventLoopThreadPool::GetNextLoop()
	{
		DLOG_TRACE;
		EventLoop* pLoop = m_pBaseLoop;

		if (IsRunning() && !m_arrThreads.empty())
		{
			// No need to lock here
			int64_t nNext = m_nNext.fetch_add(1);
			nNext = nNext % m_arrThreads.size();
			pLoop = (m_arrThreads[nNext])->GetEvtLoop();
		}

		return pLoop;
	}

	EventLoop* EventLoopThreadPool::GetNextLoopWithHash(uint64_t nHash)
	{
		EventLoop* pLoop = m_pBaseLoop;

		if (IsRunning() && !m_arrThreads.empty())
		{
			uint64_t nNext = nHash % m_arrThreads.size();
			pLoop = (m_arrThreads[nNext])->GetEvtLoop();
		}

		return pLoop;
	}

	uint32_t EventLoopThreadPool::GetThreadNum() const
	{
		return m_nThreadNum;
	}

	void EventLoopThreadPool::OnThreadStarted(uint32_t count)
	{
		DLOG_TRACE << "tid=" << std::this_thread::get_id() << " count=" << count << " started.";
		if (count == m_nThreadNum)
		{
			DLOG_TRACE << "thread pool totally started.";
			m_nStatus.store(kRunning);
		}
	}

	void EventLoopThreadPool::OnThreadExited(uint32_t count)
	{
		DLOG_TRACE << "tid=" << std::this_thread::get_id() << " count=" << count << " exited.";
		if (count == m_nThreadNum)
		{
			m_nStatus.store(kStopped);
			DLOG_TRACE << "this is the last thread stopped. Thread pool totally exited.";
			if (m_fnStoppedCallBack)
			{
				m_fnStoppedCallBack();
				m_fnStoppedCallBack = DoneCallback();
			}
		}
	}

}
