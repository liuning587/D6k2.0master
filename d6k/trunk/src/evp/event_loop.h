 
#pragma once

#include "duration.h"
#include "any.h"

#include <atomic>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>


#ifdef H_HAVE_BOOST
#include <boost/lockfree/queue.hpp>
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <concurrentqueue/concurrentqueue.h>
#pragma GCC diagnostic pop
#else
#include <concurrentqueue/concurrentqueue.h>
#endif // __GNUC__

#endif


#ifndef H_CASE_STRING_BIGIN
#define H_CASE_STRING_BIGIN(state) switch(state){
#define H_CASE_STRING(state) case state:return #state;break;
#define H_CASE_STRING_END()  default:return "Unknown";break;}
#endif


namespace Evp
{
	class ServerStatus
	{
	public:
		enum Status
		{
			kNull = 0,
			kInitializing = 1,
			kInitialized = 2,
			kStarting = 3,
			kRunning = 4,
			kStopping = 5,
			kStopped = 6,
		};

		enum SubStatus
		{
			kSubStatusNull = 0,
			kStoppingListener = 1,
			kStoppingThreadPool = 2,
		};

		std::string StatusToString() const
		{
			H_CASE_STRING_BIGIN(m_nStatus.load());
			H_CASE_STRING(kNull);
			H_CASE_STRING(kInitialized);
			H_CASE_STRING(kRunning);
			H_CASE_STRING(kStopping);
			H_CASE_STRING(kStopped);
			H_CASE_STRING_END();
		}

		bool IsRunning() const
		{
			return m_nStatus.load() == kRunning;
		}

		bool IsStopped() const
		{
			return m_nStatus.load() == kStopped;
		}

		bool IsStopping() const
		{
			return m_nStatus.load() == kStopping;
		}

	protected:
		std::atomic<Status> m_nStatus = { kNull };
		std::atomic<SubStatus> m_nSubStatus = { kSubStatusNull };
	};

	class EventLoop;
	class TimerEventWatcher;
	class PipeEventWatcher;
	class InvokeTimer;

	typedef std::shared_ptr<InvokeTimer> InvokeTimerPtr;

	class EventLoop : public ServerStatus
	{
	public:
		typedef std::function<void()> Functor;
	public:
		EventLoop();

		// Build an EventLoop using an existing event_base object,
		// so we can embed an EventLoop object into the old applications based on libevent
		// NOTE: Be careful to deal with the destructing work of event_base_ and watcher_ objects.
		explicit EventLoop(struct event_base* base);
		~EventLoop();


		// @brief Run the IO Event driving loop forever
		// @note It must be called in the IO Event thread
		void Run();

		// @brief Stop the event loop
		void Stop();

		// @brief Reinitialize some data fields after a fork
		void AfterFork();

		InvokeTimerPtr RunAfter(double delay_ms, const Functor& f);
		InvokeTimerPtr RunAfter(Duration delay, const Functor& f);
// 
// 		// RunEvery executes Functor f every period interval time.
 		InvokeTimerPtr RunEvery(Duration interval, const Functor& f);
// 
		void RunInLoop(const Functor& handler);
		void QueueInLoop(const Functor& handler);

	public:
		InvokeTimerPtr RunAfter(double delay_ms, Functor&& f);
		InvokeTimerPtr RunAfter(Duration delay, Functor&& f);

		// RunEvery executes Functor f every period interval time.
		InvokeTimerPtr RunEvery(Duration interval, Functor&& f);

		void RunInLoop(Functor&& handler);
		void QueueInLoop(Functor&& handler);

		struct event_base* GetEventBase()
		{
			return m_pEvBase;
		}
		bool IsInLoopThread() const 
		{
			return m_nTid == std::this_thread::get_id();
		}
#if 0
		void set_context(const Any& c) 
		{
			context_[0] = c;
		}
		const Any& context() const 
		{
			return context_[0];
		}
		void set_context(int index, const Any& c) 
		{
			assert(index < kContextCount && index >= 0);
			context_[index] = c;
		}
		const Any& context(int index) const 
		{
			assert(index < kContextCount && index >= 0);
			return context_[index];
		}
#endif

		int GetPendingFunctorCount() const 
		{
			return m_nPendingFunctorCount.load();
		}
		const std::thread::id& GetTid() const 
		{
			return m_nTid;
		}

	private:
		void Init();
		void InitNotifyPipeWatcher();
		void StopInLoop();
		void DoPendingFunctors();
		size_t GetPendingQueueSize();
		bool IsPendingQueueEmpty();

	private:
		struct event_base* m_pEvBase;
		bool m_bCreateEvbaseMyself;
		std::thread::id m_nTid;

		enum 
		{ 
			kContextCount = 16, 
		};
		Any m_arrContext[kContextCount];

		std::mutex m_mutex;
		// We use this to notify the thread when we put a task into the m_pPendingFunctorsArrary queue
		std::shared_ptr<PipeEventWatcher> m_pPipeWatcher;
		// When we put a task into the m_pPendingFunctorsArrary queue,
		// we need to notify the thread to execute it. But we don't want to notify repeatedly.
		std::atomic<bool> m_bNotified;
#ifdef H_HAVE_BOOST
		boost::lockfree::queue<Functor*>* m_pPendingFunctorsArrary;
#elif defined(H_HAVE_CAMERON314_CONCURRENTQUEUE)
		moodycamel::ConcurrentQueue<Functor>* m_pPendingFunctorsArrary;
#else
		std::vector<Functor>* m_pPendingFunctorsArrary; // @Guarded By m_mutex
#endif

		std::atomic<int> m_nPendingFunctorCount;
	};

}
