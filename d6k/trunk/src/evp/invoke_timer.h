/*! @file invoke_timer.h
<PRE>
********************************************************************************
模块名       :
文件名       :  invoke_timer.h
文件实现功能 :  定时器
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   定时器
*  @author  LiJin
*  @version 1.0
*  @date    2017.09.07
*******************************************************************************/

#pragma once 

#include "inner_pre.h"

#include "duration.h"

#include <memory>
#include <functional>

namespace Evp
{
	class EventLoop;
	class TimerEventWatcher;
	class InvokeTimer;

	typedef std::shared_ptr<InvokeTimer> InvokeTimerPtr;

	class InvokeTimer : public std::enable_shared_from_this<InvokeTimer>
	{
	public:
		typedef std::function<void()> Functor;

		// @brief Create a timer. When the timer is timeout, the functor f will be invoked automatically.
		// @param evloop - The EventLoop runs this timer
		// @param timeout - The timeout when the timer is invoked
		// @param f -
		// @param periodic - To indicate this timer whether it is a periodic timer.
		//  If it is true this timer will be automatically invoked periodic.
		// @return evp::InvokeTimerPtr - The user layer can hold this shared_ptr and can cancel this timer at any time.
		static InvokeTimerPtr Create(EventLoop* pEvLoop,Duration timeout,	const Functor& f,bool bPeriodic);
		static InvokeTimerPtr Create(EventLoop* pEvLoop,Duration timeout,	Functor&& f,bool bPeriodic);
		~InvokeTimer();


		// 启动定时器，（线程安全）
		void Start();

		//取消定时器，退出之前需要手动取消定时器，否则有资源泄漏 ，m_fnCancelCallback会被调用。
		void Cancel();
		// 退出之前的回调函数
		void SetCancelCallback(const Functor& fn)
		{
			m_fnCancelCallback = fn;
		}
	private:
		InvokeTimer(EventLoop* pEvLoop, Duration timeout, const Functor& f, bool bPeriodic);
		InvokeTimer(EventLoop* pEvLoop, Duration timeout, Functor&& f, bool bPeriodic);
		void OnTimerTriggered();
		void OnCanceled();

	private:
		EventLoop* m_pEvLoop;
		Duration m_duTimeout;
		Functor m_Functor;
		Functor m_fnCancelCallback;
		std::shared_ptr<TimerEventWatcher> m_pTimerWatcher;
		bool m_bPeriodic;
		std::shared_ptr<InvokeTimer> m_pSelf; // Hold myself
	};
}

/** @}*/
