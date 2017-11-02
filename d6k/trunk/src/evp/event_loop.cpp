


#include "libevent.h"
#include "event_loop.h"
#include "event_watcher.h"
#include "invoke_timer.h"
#include "logging.h"

#include <vector>
#include <thread>
#include <mutex>


namespace Evp
{	
	EventLoop::EventLoop()
		: m_bCreateEvbaseMyself(true), m_bNotified(false), m_nPendingFunctorCount(0) 
	{
		DLOG_TRACE;
#if LIBEVENT_VERSION_NUMBER >= 0x02001500
		struct event_config* cfg = event_config_new();
		if (cfg)
		{
			// Does not cache time to get a preciser timer
			event_config_set_flag(cfg, EVENT_BASE_FLAG_NO_CACHE_TIME);
			m_pEvBase = event_base_new_with_config(cfg);
			event_config_free(cfg);
		}
#else
		m_pEvBase = event_base_new();
#endif
		Init();
	}

	EventLoop::EventLoop(struct event_base* pBase)
		: m_pEvBase(pBase), m_bCreateEvbaseMyself(false), m_bNotified(false), m_nPendingFunctorCount(0) 
	{
		DLOG_TRACE;
		Init();

		// When we build an EventLoop instance from an existing event_base
		// object, we will never call EventLoop::Run() method.
		// So we need to watch the task queue here.
		bool rc = m_pPipeWatcher->AsyncWait();
		assert(rc);
		if (!rc) 
		{
			LOG_FATAL << "PipeEventWatcher init failed.";
		}
		m_nStatus.store(kRunning);
	}

	EventLoop::~EventLoop()
	{
		DLOG_TRACE;
		m_pPipeWatcher.reset();

		if (m_pEvBase != nullptr && m_bCreateEvbaseMyself) 
		{
			event_base_free(m_pEvBase);
			m_pEvBase = nullptr;
		}

		delete m_pPendingFunctorsArrary;
		m_pPendingFunctorsArrary = nullptr;
	}

	void EventLoop::Init()
	{
		DLOG_TRACE;
		m_nStatus.store(kInitializing);
#ifdef H_HAVE_BOOST
		const size_t kPendingFunctorCount = 1024 * 16;
		this->m_pPendingFunctorsArrary = new boost::lockfree::queue<Functor*>(kPendingFunctorCount);
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
		this->m_pPendingFunctorsArrary = new moodycamel::ConcurrentQueue<Functor>();
#else
		this->m_pPendingFunctorsArrary = new std::vector<Functor>();
#endif

		m_nTid = std::this_thread::get_id(); // The default thread id

		InitNotifyPipeWatcher();

		m_nStatus.store(kInitialized);
	}

	void EventLoop::InitNotifyPipeWatcher() 
	{
		// Initialized task queue notify pipe watcher
		m_pPipeWatcher.reset(new PipeEventWatcher(this, std::bind(&EventLoop::DoPendingFunctors, this)));
		int nRc = m_pPipeWatcher->Init();
		assert(nRc);
		if (!nRc) 
		{
			LOG_FATAL << "PipeEventWatcher init failed.";
		}
	}

	void EventLoop::Run() 
	{
		DLOG_TRACE;
		m_nStatus.store(kStarting);
		m_nTid = std::this_thread::get_id(); // The actual thread id

		int rc = m_pPipeWatcher->AsyncWait();
		assert(rc);
		if (!rc) 
		{
			LOG_FATAL << "PipeEventWatcher init failed.";
		}

		// After everything have initialized, we set the status to kRunning
		m_nStatus.store(kRunning);

		rc = event_base_dispatch(m_pEvBase);
		if (rc == 1) 
		{
			LOG_ERROR << "event_base_dispatch error: no event registered";
		}
		else if (rc == -1) 
		{
			int nErrNo = errno;
			LOG_ERROR << "event_base_dispatch error " << nErrNo << " " << strerror(nErrNo);
		}

		// Make sure m_pWatcher does construct, initialize and destruct in the same thread.
		m_pPipeWatcher.reset();
		DLOG_TRACE << "EventLoop stopped, tid=" << std::this_thread::get_id();

		m_nStatus.store(kStopped);
	}
	
	void EventLoop::Stop() 
	{
		DLOG_TRACE;
		assert(m_nStatus.load() == kRunning);
		m_nStatus.store(kStopping);
		DLOG_TRACE << "EventLoop::Stop";
		QueueInLoop(std::bind(&EventLoop::StopInLoop, this));
	}

	void EventLoop::StopInLoop() 
	{
		DLOG_TRACE << "EventLoop is stopping now, tid=" << std::this_thread::get_id();
		assert(m_nStatus.load() == kStopping);

		// 执行完未结的事项
		auto f = [this] () 
		{
			for (int i = 0;; i++) 
			{
				DLOG_TRACE << "calling DoPendingFunctors index=" << i;
				DoPendingFunctors();
				if (IsPendingQueueEmpty()) 
				{
					break;
				}
			}
		};

		DLOG_TRACE << "before event_base_loopexit, we invoke DoPendingFunctors";

		f();

		DLOG_TRACE << "start event_base_loopexit";
		event_base_loopexit(m_pEvBase, nullptr);
		DLOG_TRACE << "after event_base_loopexit, we invoke DoPendingFunctors";

		f();

		DLOG_TRACE << "end of StopInLoop";
	}

	void EventLoop::AfterFork() 
	{
		int rc = event_reinit(m_pEvBase);
		assert(rc == 0);

		if (rc != 0) 
		{
			LOG_FATAL << "event_reinit failed!";
			abort();
		}

		// We create EventLoopThread and initialize it in father process,
		// but we use it in child process.
		// If we have only one child process, everything goes well.
		//
		// But if we have multi child processes, something goes wrong.
		// Because EventLoop::m_pWatcher is created and initialized in father process
		// all children processes inherited father's pipe.
		//
		// When we use the pipe to do a notification in one child process
		// the notification may be received by another child process randomly.
		//
		// So we need to reinitialize the m_pWatcher
		InitNotifyPipeWatcher();
	}

	InvokeTimerPtr EventLoop::RunAfter(double delay_ms, const Functor& f) 
	{
		DLOG_TRACE;
		return RunAfter(Duration(delay_ms / 1000.0), f);
	}

	InvokeTimerPtr EventLoop::RunAfter(double delay_ms, Functor&& f)
	{
		DLOG_TRACE;
		return RunAfter(Duration(delay_ms / 1000.0), std::move(f));
	}

	InvokeTimerPtr EventLoop::RunAfter(Duration delay, const Functor& f) 
	{
		DLOG_TRACE;
		std::shared_ptr<InvokeTimer> t = InvokeTimer::Create(this, delay, f, false);
		t->Start();
		return t;
	}

	InvokeTimerPtr EventLoop::RunAfter(Duration delay, Functor&& f)
	{
		DLOG_TRACE;
		std::shared_ptr<InvokeTimer> t = InvokeTimer::Create(this, delay, std::move(f), false);
		t->Start();
		return t;
	}

	InvokeTimerPtr EventLoop::RunEvery(Duration interval, const Functor& f) 
	{
		DLOG_TRACE;
		std::shared_ptr<InvokeTimer> t = InvokeTimer::Create(this, interval, f, true);
		t->Start();
		return t;
	}

	InvokeTimerPtr EventLoop::RunEvery(Duration interval, Functor&& f) 
	{
		DLOG_TRACE;
		std::shared_ptr<InvokeTimer> t = InvokeTimer::Create(this, interval, std::move(f), true);
		t->Start();
		return t;
	}
	
	void EventLoop::RunInLoop(const Functor& functor)
	{
		DLOG_TRACE;
		if (IsRunning() && IsInLoopThread())
		{
			functor();
		}
		else
		{
			QueueInLoop(functor);
		}
	}

	void EventLoop::RunInLoop(Functor&& functor) 
	{
	 	DLOG_TRACE;
		if (IsRunning() && IsInLoopThread()) 
		{
			functor();
		}
		else
		{
			QueueInLoop(std::move(functor));
		}
	}

	void EventLoop::QueueInLoop(const Functor& cb) 
	{
		DLOG_TRACE << "m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();
		{
#ifdef H_HAVE_BOOST
			auto f = new Functor(cb);
			while (!m_pPendingFunctorsArrary->push(f))
			{
			}
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
			while (!m_pPendingFunctorsArrary->enqueue(cb)) 
			{
			}
#else
			std::lock_guard<std::mutex> lock(m_mutex);
			m_pPendingFunctorsArrary->emplace_back(cb);
#endif
		}
		++m_nPendingFunctorCount;
		DLOG_TRACE << "queued a new Functor. m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();
		if ( m_bNotified.load() == false) 
		{
			DLOG_TRACE << "call m_pWatcher->Nofity() m_bNotified.store(true)";

			// We must set m_bNotified to true before calling `m_pWatcher->Nodify()`
			// otherwise there is a change that:
			//  1. We called m_pWatcher- > Nodify() on thread1
			//  2. On thread2 we watched this event, so wakeup the CPU changed to run this EventLoop on thread2 and executed all the pending task
			//  3. Then the CPU changed to run on thread1 and set m_bNotified to true
			//  4. Then, some thread except thread2 call this QueueInLoop to push a task into the queue, and find m_bNotified is true, so there is no change to wakeup thread2 to execute this task
			m_bNotified.store(true);

			// Sometimes one thread invoke EventLoop::QueueInLoop(...), but anther
			// thread is invoking EventLoop::Stop() to stop this loop. At this moment
			// this loop maybe is stopping and the m_pWatcher object maybe has been
			// released already.
			if (m_pPipeWatcher)
			{
				m_pPipeWatcher->Notify();
			}
			else 
			{
				DLOG_TRACE << "status=" << StatusToString();
				assert(!IsRunning());
			}
		}
		else
		{
			DLOG_TRACE << "No need to call m_pWatcher->Nofity()";
		}
	}

	void EventLoop::QueueInLoop(Functor&& cb) 
	{
		DLOG_TRACE << "m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << m_bNotified.load();
		{
#ifdef H_HAVE_BOOST
			auto f = new Functor(std::move(cb)); // TODO Add test code for it
			while (!m_pPendingFunctorsArrary->push(f)) 
			{
			}
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
			while (!m_pPendingFunctorsArrary->enqueue(std::move(cb)))
			{
			}
#else
			std::lock_guard<std::mutex> lock(m_mutex);
			m_pPendingFunctorsArrary->emplace_back(std::move(cb));
#endif
		}
		++m_nPendingFunctorCount;
		DLOG_TRACE << "queued a new Functor. m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();
		if (!m_bNotified.load()) 
		{
			DLOG_TRACE << "call m_pWatcher->Nofity() m_bNotified.store(true)";
			m_bNotified.store(true);
			if (m_pPipeWatcher) 
			{
				m_pPipeWatcher->Notify();
			}
			else
			{
				DLOG_TRACE << "m_pWatcher is empty, maybe we call EventLoop::QueueInLoop on a stopped EventLoop. status=" << StatusToString();
				assert(!IsRunning());
			}
		}
		else
		{
			DLOG_TRACE << "No need to call m_bNotified->Nofity()";
		}
	}

	void EventLoop::DoPendingFunctors() 
	{
		DLOG_TRACE << "m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();

#ifdef H_HAVE_BOOST
		m_bNotified.store(false);
		Functor* f = nullptr;
		while (m_pPendingFunctorsArrary->pop(f))
		{
			(*f)();
			delete f;
			--m_nPendingFunctorCount;
		}
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
		m_bNotified.store(false);
		Functor f;
		while (m_pPendingFunctorsArrary->try_dequeue(f)) 
		{
			f();
			--m_nPendingFunctorCount;
		}
#else
		std::vector<Functor> functors;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_bNotified.store(false);
			m_pPendingFunctorsArrary->swap(functors);
			DLOG_TRACE << "m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();
		}
		DLOG_TRACE << "m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();
		for (size_t i = 0; i < functors.size(); ++i) 
		{
			functors[i]();
			--m_nPendingFunctorCount;
		}
		DLOG_TRACE << "m_nPendingFunctorCount=" << m_nPendingFunctorCount << " PendingQueueSize=" << GetPendingQueueSize() << " m_bNotified=" << m_bNotified.load();
#endif
	}

	size_t EventLoop::GetPendingQueueSize() 
	{
#ifdef H_HAVE_BOOST
		return static_cast<size_t>(m_nPendingFunctorCount.load());
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
		return m_pPendingFunctorsArrary->size_approx();
#else
		return m_pPendingFunctorsArrary->size();
#endif
	}

	bool EventLoop::IsPendingQueueEmpty() 
	{
#ifdef H_HAVE_BOOST
		return m_pPendingFunctorsArrary->empty();
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
		return m_pPendingFunctorsArrary->size_approx() == 0;
#else
		return m_pPendingFunctorsArrary->empty();
#endif
	}
}
