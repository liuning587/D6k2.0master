//#include "inner_pre.h"


#include "libevent.h"
#include "event_watcher.h"
#include "event_loop.h"
#include "logging.h"

#include <string.h>

namespace Evp
{
	EventWatcher::EventWatcher(struct event_base* evbase, const Handler& handler)
		: m_pEvBase(evbase), m_bAttached(false), m_fnHandler(handler)
	{
		m_pEvent = new event;
		memset(m_pEvent, 0, sizeof(struct event));
	}

	EventWatcher::EventWatcher(struct event_base* evbase, Handler&& handler)
		: m_pEvBase(evbase), m_bAttached(false), m_fnHandler(std::move(handler))
	{
		m_pEvent = new event;
		memset(m_pEvent, 0, sizeof(struct event));
	}

	EventWatcher::~EventWatcher() 
	{
		FreeEvent();
		Close();
	}

	bool EventWatcher::Init() 
	{
		if (!DoInit()) 
		{
			goto failed;
		}

		::event_base_set(m_pEvBase, m_pEvent);
		return true;

	failed:
		Close();
		return false;
	}


	void EventWatcher::Close() 
	{
		DoClose();
	}

	bool EventWatcher::Watch(Duration timeout)
	{
		struct timeval tv;
		struct timeval* pTmVal = nullptr;
		if (timeout.Nanoseconds() > 0) 
		{
			timeout.To(&tv);
			pTmVal = &tv;
		}

		if (m_bAttached)
		{
			// When InvokerTimer::m_bPeriodic == true, EventWatcher::Watch will be called many times
			// so we need to remove it from event_base before we add it into event_base
			if (EventDel(m_pEvent) != 0)
			{
				LOG_ERROR << "event_del failed. fd=" << this->m_pEvent->ev_fd << " m_pEvent=" << m_pEvent;
				// TODO how to deal with it when failed?
			}
			m_bAttached = false;
		}

		assert(!m_bAttached);
		if (EventAdd(m_pEvent, pTmVal) != 0) 
		{
			LOG_ERROR << "event_add failed. fd=" << this->m_pEvent->ev_fd << " m_pEvent=" << m_pEvent;
			return false;
		}
		m_bAttached = true;
		return true;
	}

	void EventWatcher::FreeEvent() 
	{
		if (m_pEvent) 
		{
			if (m_bAttached) 
			{
				EventDel(m_pEvent);
				m_bAttached = false;
			}

			delete (m_pEvent);
			m_pEvent = nullptr;
		}
	}

	void EventWatcher::Cancel() 
	{
		assert(m_pEvent);
		FreeEvent();

		if (m_fnCancelCallback) 
		{
			m_fnCancelCallback();
			m_fnCancelCallback = Handler();
		}
	}

	void EventWatcher::SetCancelCallback(const Handler& cb) 
	{
		m_fnCancelCallback = cb;
	}
 
	PipeEventWatcher::PipeEventWatcher(EventLoop* pLoop, const Handler& handler)
		: EventWatcher(pLoop->GetEventBase(), handler)
	{
		memset(m_arrPipe, 0, sizeof(m_arrPipe[0] * 2));
	}

	PipeEventWatcher::PipeEventWatcher(EventLoop* pLoop, Handler&& h)
		: EventWatcher(pLoop->GetEventBase(), std::move(h))
	{
		memset(m_arrPipe, 0, sizeof(m_arrPipe[0] * 2));
	}


	PipeEventWatcher::~PipeEventWatcher() 
	{
		Close();
	}

	bool PipeEventWatcher::DoInit()
	{
		assert(m_arrPipe[0] == 0);

		if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, m_arrPipe) < 0)
		{
			int err = errno;
			LOG_ERROR << "create socketpair ERROR errno=" << err << " " << strerror(err);
			goto failed;
		}

		if (evutil_make_socket_nonblocking(m_arrPipe[0]) < 0 ||
			evutil_make_socket_nonblocking(m_arrPipe[1]) < 0) 
		{
			goto failed;
		}

		::event_set(m_pEvent, m_arrPipe[1], EV_READ | EV_PERSIST, &PipeEventWatcher::HandlerFn, this);
		return true;

	failed:
		Close();
		return false;
	}

	void PipeEventWatcher::DoClose()
	{
		if (m_arrPipe[0] > 0) 
		{
			EVUTIL_CLOSESOCKET(m_arrPipe[0]);
			EVUTIL_CLOSESOCKET(m_arrPipe[1]);
			memset(m_arrPipe, 0, sizeof(m_arrPipe[0]) * 2);
		}
	}

	void PipeEventWatcher::HandlerFn(evpp_socket_t fd, short /*which*/, void* v) 
	{
	//	LOG_INFO << "PipeEventWatcher::HandlerFn fd=" << fd << " v=" << v;
		PipeEventWatcher* pPipeEvtWatcher = (PipeEventWatcher*)v;
#ifdef H_BENCHMARK_TESTING
		// Every time we only read 1 byte for testing the IO event performance.
		// We use it in the benchmark test program 
		//  1. evp/benchmark/ioevent/evp/
		//  1. evp/benchmark/ioevent/fd_channel_vs_pipe_event_watcher/
		char buf[1];
#else
		char buf[128] = { 0 };
#endif
		int n = 0;

		if ((n = ::recv(pPipeEvtWatcher->m_arrPipe[1], buf, sizeof(buf), 0)) > 0)
		{// 执行 loop里面的 m_pPendingFunctorsArrary 队列的未执行函数
			pPipeEvtWatcher->m_fnHandler();
		}
	}

	bool PipeEventWatcher::AsyncWait() 
	{
		return Watch(Duration());
	}
	/*! \fn void PipeEventWatcher::Notify() 
	********************************************************************************************************* 
	** \brief Evp::PipeEventWatcher::Notify 
	** \details 发送通知，告知线程，处理未完成的m_pPendingFunctorsArrary队列中的事项
	** \return void 
	** \author LiJin 
	** \date 2017年9月7日 
	** \note 
	********************************************************************************************************/
	void PipeEventWatcher::Notify() 
	{
		char buf[1] = {};

		if (::send(m_arrPipe[0], buf, sizeof(buf), 0) < 0)
		{
			return;
		}
	}
 
	//////////////////////////////////////////////////////////////////////////
	TimerEventWatcher::TimerEventWatcher(EventLoop* loop,
		const Handler& handler,	Duration timeout)
		: EventWatcher(loop->GetEventBase(), handler)
		, m_duTimeOut(timeout) 
	{

	}

	TimerEventWatcher::TimerEventWatcher(EventLoop* loop,
		Handler&& h,Duration timeout)
		: EventWatcher(loop->GetEventBase(), std::move(h))
		, m_duTimeOut(timeout)
	{

	}

	TimerEventWatcher::TimerEventWatcher(struct event_base* loop,
		const Handler& handler,	Duration timeout)
		: EventWatcher(loop, handler)
		, m_duTimeOut(timeout) 
	{

	}

	TimerEventWatcher::TimerEventWatcher(struct event_base* loop,
		Handler&& h,Duration timeout)
		: EventWatcher(loop, std::move(h))
		, m_duTimeOut(timeout) 
	{

	}

	bool TimerEventWatcher::DoInit()
	{
		::event_set(m_pEvent, -1, 0, TimerEventWatcher::HandlerFn, this);
		return true;
	}

	void TimerEventWatcher::HandlerFn(evpp_socket_t /*fd*/, short /*which*/, void* v) 
	{
		TimerEventWatcher* pTimerWatcher = (TimerEventWatcher*)v;
		pTimerWatcher->m_fnHandler();
	}

	bool TimerEventWatcher::AsyncWait()
	{
		return Watch(m_duTimeOut);
	}
 
	//////////////////////////////////////////////////////////////////////////
	SignalEventWatcher::SignalEventWatcher(signal_number_t signo, EventLoop* loop,
		const Handler& handler)
		: EventWatcher(loop->GetEventBase(), handler)
		, m_nSigNo(signo) 
	{
		assert(m_nSigNo);
	}

	SignalEventWatcher::SignalEventWatcher(signal_number_t signo, EventLoop* loop,
		Handler&& h)
		: EventWatcher(loop->GetEventBase(), std::move(h))
		, m_nSigNo(signo)
	{
		assert(m_nSigNo);
	}

	bool SignalEventWatcher::DoInit() 
	{
		assert(m_nSigNo);
		signal_set(m_pEvent, m_nSigNo, SignalEventWatcher::HandlerFn, this);
		return true;
	}

	void SignalEventWatcher::HandlerFn(signal_number_t /*sn*/, short /*which*/, void* v)
	{
		SignalEventWatcher* h = (SignalEventWatcher*)v;
		h->m_fnHandler();
	}

	bool SignalEventWatcher::AsyncWait()
	{
		return Watch(Duration());
	}
 
}

