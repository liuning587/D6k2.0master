/*! @file invoke_timer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  invoke_timer.h
�ļ�ʵ�ֹ��� :  ��ʱ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��ʱ��
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


		// ������ʱ�������̰߳�ȫ��
		void Start();

		//ȡ����ʱ�����˳�֮ǰ��Ҫ�ֶ�ȡ����ʱ������������Դй© ��m_fnCancelCallback�ᱻ���á�
		void Cancel();
		// �˳�֮ǰ�Ļص�����
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
