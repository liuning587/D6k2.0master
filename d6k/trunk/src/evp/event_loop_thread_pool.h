#pragma once


#include "event_loop_thread.h"

#include <atomic>


namespace Evp
{
	class  EventLoopThreadPool : public ServerStatus
	{
	public:
		typedef std::function<void()> DoneCallback;

		EventLoopThreadPool(EventLoop* pBaseLoop, uint32_t nThreadNum);
		~EventLoopThreadPool();

		bool Start(bool bWaitThreadStarted = false);

		void Stop(bool bWaitThreadExited = false);
		void Stop(DoneCallback fn);

		// @brief Join all the working thread. If you forget to call this method,
		// it will be invoked automatically in the destruct method ~EventLoopThreadPool().
		// @note DO NOT call this method from any of the working thread.
		void Join();

		// @brief Reinitialize some data fields after a fork
		void AfterFork();
	public:
		EventLoop* GetNextLoop();
		EventLoop* GetNextLoopWithHash(uint64_t hash);

		uint32_t GetThreadNum() const;

	private:
		void Stop(bool wait_thread_exit, DoneCallback fn);
		void OnThreadStarted(uint32_t count);
		void OnThreadExited(uint32_t count);
	private:
		EventLoop* m_pBaseLoop;

		uint32_t m_nThreadNum = 0;
		std::atomic<int64_t> m_nNext = { 0 };

		DoneCallback m_fnStoppedCallBack;

		typedef std::shared_ptr<EventLoopThread> EventLoopThreadPtr;
		std::vector<EventLoopThreadPtr> m_arrThreads;
	};
}
