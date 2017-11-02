#include "inner_pre.h"

#include "event_loop.h"
#include "event_watcher.h"
#include "invoke_timer.h"
#include "logging.h"

namespace Evp
{
	InvokeTimer::InvokeTimer(EventLoop* pEvLoop, Duration timeout, const Functor& f, bool bPeriodic)
		: m_pEvLoop(pEvLoop), m_duTimeout(timeout), m_Functor(f), m_bPeriodic(bPeriodic)
	{
		DLOG_TRACE << "loop=" << m_pEvLoop;
	}

	InvokeTimer::InvokeTimer(EventLoop* pEvLoop, Duration timeout, Functor&& f, bool bPeriodic)
		: m_pEvLoop(pEvLoop), m_duTimeout(timeout), m_Functor(std::move(f)), m_bPeriodic(bPeriodic)
	{
		DLOG_TRACE << "loop=" << m_pEvLoop;
	}

	InvokeTimerPtr InvokeTimer::Create(EventLoop* pEvLoop, Duration timeout, const Functor& f, bool bPeriodic)
	{
		InvokeTimerPtr it(new InvokeTimer(pEvLoop, timeout, f, bPeriodic));
		it->m_pSelf = it;
		return it;
	}

	InvokeTimerPtr InvokeTimer::Create(EventLoop* pEvLoop, Duration timeout, Functor&& f, bool bPeriodic)
	{
		InvokeTimerPtr it(new InvokeTimer(pEvLoop, timeout, std::move(f), bPeriodic));
		it->m_pSelf = it;
		return it;
	}

	InvokeTimer::~InvokeTimer()
	{
		if (m_pTimerWatcher)
		{
			m_pTimerWatcher.reset();
		}

		if (m_pSelf)
		{
			m_pSelf.reset();
		}

		DLOG_TRACE << "m_pEvLoop =" << m_pEvLoop;
	}
	/*! \fn void InvokeTimer::Start()
	********************************************************************************************************* 
	** \brief Evp::InvokeTimer::Start 
	** \details 启动定时器
	** \return void 
	** \author LiJin 
	** \date 2017年9月7日 
	** \note 
	********************************************************************************************************/
	void InvokeTimer::Start()
	{
	 	DLOG_TRACE << "m_pEvLoop=" << m_pEvLoop << " refcount=" << m_pSelf.use_count();
		auto f = [this] ()
		{
			m_pTimerWatcher.reset(new TimerEventWatcher(m_pEvLoop, std::bind(&InvokeTimer::OnTimerTriggered, shared_from_this()), m_duTimeout));
			m_pTimerWatcher->SetCancelCallback(std::bind(&InvokeTimer::OnCanceled, shared_from_this()));
			m_pTimerWatcher->Init();
			m_pTimerWatcher->AsyncWait();
			DLOG_TRACE << "timer=" << m_pTimerWatcher.get() << " loop=" << m_pEvLoop << " refcount=" <<
				m_pSelf.use_count() << " periodic=" << m_bPeriodic << " timeout(ms)=" << m_duTimeout.Milliseconds();
		};
		m_pEvLoop->RunInLoop(std::move(f));
	}
	/*! \fn void InvokeTimer::Cancel() 
	********************************************************************************************************* 
	** \brief Evp::InvokeTimer::Cancel 
	** \details 取消定时器，如果是周期性的定时器，需要手动取消
	** \return void 
	** \author LiJin 
	** \date 2017年9月7日 
	** \note 
	********************************************************************************************************/
	void InvokeTimer::Cancel() 
	{
		DLOG_TRACE;
		if (m_pTimerWatcher)
		{
			m_pEvLoop->QueueInLoop(std::bind(&TimerEventWatcher::Cancel, m_pTimerWatcher));
		}
	}
	/*! \fn void InvokeTimer::OnTimerTriggered() 
	********************************************************************************************************* 
	** \brief Evp::InvokeTimer::OnTimerTriggered 
	** \details 定时器触发执行函数
	** \return void 
	** \author LiJin 
	** \date 2017年9月7日 
	** \note 
	********************************************************************************************************/
	void InvokeTimer::OnTimerTriggered() 
	{
		DLOG_TRACE << "loop=" << m_pEvLoop << " use_count=" << m_pSelf.use_count();
		// 执行定时器函数
		m_Functor();

		if (m_bPeriodic)
		{// 周期性定时器
			// 重新注册事件
			m_pTimerWatcher->AsyncWait();
		}
		else 
		{// 非周期性，执行完清理资源
			m_Functor = Functor();
		//  m_Functor = nullptr;
			m_fnCancelCallback = Functor();
			m_pTimerWatcher.reset();
			m_pSelf.reset();
		}
	}

	void InvokeTimer::OnCanceled()
	{
		DLOG_TRACE << "loop=" << m_pEvLoop << " use_count=" << m_pSelf.use_count();
		m_bPeriodic = false;
		if (m_fnCancelCallback)
		{
			m_fnCancelCallback();
			m_fnCancelCallback = Functor();
		}
		m_Functor = Functor();
		m_pTimerWatcher.reset();
		m_pSelf.reset();
	}

}
