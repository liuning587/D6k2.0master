/*! @file base_task.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_task.cpp
�ļ�ʵ�ֹ��� :  ����ģʽ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����ģʽ
*  @author  LiJin
*  @version 1.0
*  @date    2017.10.20
*******************************************************************************/
#include "base_task.h"

#include <vector>
#include <sstream>
#include <cassert>
 
CBaseTask::CBaseTask()
{
	m_nTid = std::thread::id();
	m_pPendingJobsArrary = std::make_shared<std::vector<AsyncJobItem>>();
	m_nPendingFunctorCount = 0;
}

CBaseTask::~CBaseTask( )
{
	assert(IsStopped());
	Join();
}

bool CBaseTask::Start(bool bWaitThreadStarted, Functor fnPre, Functor fnPost)
{
 	m_nStatus = kStarting;

	assert(m_pThread.get() == nullptr);
	m_pThread.reset(new std::thread(std::bind(&CBaseTask::Run, this, fnPre, fnPost)));

	// �������߳��������
	if (bWaitThreadStarted)
	{
		while (m_nStatus < kRunning)
		{
			std::chrono::milliseconds timespan(1);
			std::this_thread::sleep_for(timespan);
		}
	}
	return true;
}
/*! \fn void CRedTask::Run(const Functor& fnPre, const Functor& fnPost)
********************************************************************************************************* 
** \brief CRedTask::Run 
** \details ����������
** \param fnPre 
** \param fnPost 
** \return void 
** \author LiJin 
** \date 2017��10��12�� 
** \note 
********************************************************************************************************/
void CBaseTask::Run(const Functor& fnPre, const Functor& fnPost)
{
	m_nTid = std::this_thread::get_id();

	if (m_szThreadName.empty())
	{
		std::ostringstream os;
		os << "Thread-" << std::this_thread::get_id();
		m_szThreadName = os.str();
	}
	// After everything have initialized, we set the status to kRunning
	m_nStatus.store(kRunning);

	auto fn = [this, fnPre]()
	{
		m_nStatus = kRunning;
		if (fnPre)
		{
			auto rc = fnPre();
			if (rc != kOK)
			{
				StopMainLoop();
			}
		}
	};

	QueueInLoop(std::move(fn));
	
	MainLoop();
	  	
	if (fnPost)
	{
		fnPost();
	}
	 
	m_nStatus = kStopped;
}

void CBaseTask::Stop(bool bWaitThreadExit)
{ 
	assert(m_nStatus == kRunning && IsRunning());
	m_nStatus = kStopping; 
	
	StopMainLoop();
	// �������߳��˳�Ϊֹ
	if (bWaitThreadExit)
	{
		while (!IsStopped())
		{
			std::chrono::milliseconds timespan(1);
			std::this_thread::sleep_for(timespan);
		}
		 
		Join(); 
	} 
}
/*! \fn void CRedTask::StopMainLoop()
********************************************************************************************************* 
** \brief CRedTask::StopMainLoop 
** \details ֹͣ�߳���ѭ��
** \return void 
** \author LiJin 
** \date 2017��10��9�� 
** \note 
********************************************************************************************************/
void CBaseTask::StopMainLoop()
{
//	assert(m_nStatus.load() == kRunning);
	// ���� ��ͣ
	m_nStatus.store(kStopping);
	// ���͵����У�������ѭ������������ѭ������StopInLoopִ��δ������
	QueueInLoop(std::bind(&CBaseTask::StopInLoop, this));
}

void CBaseTask::StopInLoop()
{
	assert(m_nStatus.load() == kStopping);

	// ִ����δ�������
	auto fn = [this] ()
	{
		for (int i = 0;; i++)
		{		
			DoPendingFunctors();
			if (IsPendingQueueEmpty())
			{
				break;
			}
		}
	};
 
	fn();
}

void CBaseTask::RunInLoop(const AsyncJobItem& functor)
{ 
	//!todo ��Ҫ����һ���ǲ����ڱ��߳���
	if (IsRunning() && IsInLoopThread())
	{
		functor();
	}
	else
	{
		QueueInLoop(functor);
	}
}

void CBaseTask::RunInLoop(AsyncJobItem&& functor)
{
	if (IsRunning() && IsInLoopThread())
	{
		functor();
	}
	else
	{
		QueueInLoop(std::move(functor));
	}
}


void CBaseTask::QueueInLoop(const AsyncJobItem& cb)
{
	std::lock_guard<std::mutex> lock(m_mtxPending);
	assert(m_pPendingJobsArrary);
	if (m_pPendingJobsArrary)
	{
		m_pPendingJobsArrary->emplace_back(std::move(cb));
	}
 	++m_nPendingFunctorCount;

#if 0
 	if (m_bNotified.load() == false)
	{ 
		// We must set m_bNotified to true before calling `m_pWatcher->Nodify()`
		// otherwise there is a change that:
		//  1. We called m_pWatcher-> Nodify() on thread1
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
			assert(!IsRunning());
		}
	}
#endif	 
}

void CBaseTask::QueueInLoop(AsyncJobItem&& cb)
{ 
	{
		std::lock_guard<std::mutex> lock(m_mtxPending);
		assert(m_pPendingJobsArrary);
		if (m_pPendingJobsArrary)
		{
			m_pPendingJobsArrary->emplace_back(std::move(cb));
		}
	}

 
	{
		std::unique_lock <std::mutex> lck(m_mtxQue);
		m_cvQue.notify_one();
	}
	++m_nPendingFunctorCount;
#if 0
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
#endif
}

void CBaseTask::DoPendingFunctors()
{ 
	std::vector<AsyncJobItem> Jobs;
	{
		std::lock_guard<std::mutex> lock(m_mtxPending);
		m_pPendingJobsArrary->swap(Jobs);
 	}
 	for (size_t i = 0; i < Jobs.size(); ++i)
	{
		Jobs[i]();
		--m_nPendingFunctorCount;
	}
}
/*! \fn void CBaseTask::MainLoop()
********************************************************************************************************* 
** \brief CBaseTask::MainLoop 
** \details ��ѭ��
** \return void 
** \author LiJin 
** \date 2017��10��9�� 
** \note 
********************************************************************************************************/
void CBaseTask::MainLoop()
{
	// �ȴ�������У����������������ݣ���ִ���������
	while (true)
	{
		if (m_nStatus.load() != kRunning)
		{
			break;
		}
		std::unique_lock <std::mutex> lck(m_mtxQue);
		m_cvQue.wait(lck);

		if (m_nStatus.load() == kRunning)
		{
			DoPendingFunctors();
		}
	} 
}

void CBaseTask::Join()
{
	// To avoid multi other threads call Join simultaneously
	std::lock_guard<std::mutex> guard(m_mtxThread);
	if (m_pThread && m_pThread->joinable())
	{
		try
		{
			m_pThread->join();
		}
		catch (const std::system_error& e)
		{
	//		LOG_ERROR << "Caught a system_error:" << e.what() << " code=" << e.code();
		}
		m_pThread.reset();
	}
} 

void CBaseTask::SetName(const std::string& szName)
{
	m_szThreadName = szName;
}

const std::string& CBaseTask::GetName() const
{
	return m_szThreadName;
}

std::thread::id CBaseTask::GetTid() const
{
	if (m_pThread)
	{
		return m_pThread->get_id();
	}

	return std::thread::id();
}

bool CBaseTask::IsPendingQueueEmpty()
{
	if (m_pPendingJobsArrary == nullptr)
		return true;
	return m_pPendingJobsArrary->empty();
}

/** @}*/
