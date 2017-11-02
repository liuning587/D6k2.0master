/*! @file base_task.h
<PRE>
********************************************************************************
模块名       :
文件名       :  base_task.h
文件实现功能 :  任务模式
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   任务模式
*  @author  LiJin
*  @version 1.0
*  @date    2017.10.20
*******************************************************************************/
#ifndef BASE_TASK_MODULE_H
#define BASE_TASK_MODULE_H
  

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>
 
class CBaseTask
{
public:
	enum { kOK = 0 };

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

	// Return 0 means OK, anything else means failed.
	typedef std::function<int()> Functor;

	typedef std::function<void()> AsyncJobItem;

	CBaseTask();
	virtual ~CBaseTask();

	// @param bWaitThreadStarted - If it is true this method will block
	//  until the thread totally started
	// @param fnPre - This functor will be executed immediately when the thread is started. 
	// @param fnPost - This functor will be executed at the moment when the thread is going to stop.
	bool Start(bool bWaitThreadStarted = true,
		Functor  fnPre = Functor(),
		Functor fnPost = Functor());

	void Stop(bool bWaitThreadExit = false);

	// @brief Join the thread. If you forget to call this method,
	// it will be invoked automatically in the destruct method ~CRedTask().
	// @note DO NOT call this method from any of the working thread.
	void Join();
	 
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

	bool IsInLoopThread() const
	{
		return m_nTid == std::this_thread::get_id();
	}
	// 对外接口
	void RunInLoop(AsyncJobItem&& handler);
	void QueueInLoop(AsyncJobItem&& handler);

	void RunInLoop(const AsyncJobItem& handler);
	void QueueInLoop(const AsyncJobItem& handler);
public:
	void SetName(const std::string& szName);
	const std::string& GetName() const;
	std::thread::id GetTid() const;

protected:
	// 任务状态
	std::atomic<Status> m_nStatus = { kNull };
private:
	void Run(const Functor& pre, const Functor& post);

	// @brief 停止主循环
	void StopMainLoop();
	void StopInLoop();

	void MainLoop();

	void DoPendingFunctors();
	size_t GetPendingQueueSize()
	{
		return m_nPendingFunctorCount.load();
	}
	bool IsPendingQueueEmpty();

private:
	 
	//! 线程ID
	std::thread::id m_nTid; 
	
	mutable std::mutex m_mtxQue; 
	std::condition_variable m_cvQue;

	std::mutex m_mtxThread;
	std::shared_ptr<std::thread> m_pThread; // Guard by m_mutex

	std::string m_szThreadName;

	std::mutex m_mtxPending;
	std::shared_ptr<std::vector<AsyncJobItem>> m_pPendingJobsArrary;
	//std::vector<Functor>* m_pPendingJobsArrary; // @Guarded By m_muxPending

	std::atomic<size_t> m_nPendingFunctorCount;
};
 
#endif // BASE_TASK_MODULE_H


/** @}*/

