#pragma once

#include "inner_pre.h"

#include "duration.h"

struct event;
struct event_base;

namespace Evp
{
	class EventLoop;
	class  EventWatcher
	{
	public:
		typedef std::function<void()> Handler;

		virtual ~EventWatcher();

		bool Init();

		// @note It MUST be called in the event thread.
		void Cancel();

		// @brief :
		// @param[IN] const Handler& cb - The callback which will be called when this event is canceled.
		// @return void -
		void SetCancelCallback(const Handler& cb);

		void ClearHandler() 
		{
			m_fnHandler = Handler(); 
		}

	protected:
		// @note It MUST be called in the event thread.
		// @param timeout the maximum amount of time to wait for the event, or 0 to wait forever
		bool Watch(Duration timeout);

	protected:
		EventWatcher(struct event_base* evbase, const Handler& handler);
		EventWatcher(struct event_base* evbase, Handler&& handler);

		void Close();
		void FreeEvent();

		virtual bool DoInit() = 0;
		virtual void DoClose() 
		{

		}

	protected:
		struct event* m_pEvent;
		struct event_base* m_pEvBase;
		bool m_bAttached;
		Handler m_fnHandler;
		Handler m_fnCancelCallback;
	};

	class  PipeEventWatcher : public EventWatcher
	{
	public:
		PipeEventWatcher(EventLoop* loop, const Handler& handler);
		PipeEventWatcher(EventLoop* loop, Handler&& handler);
		~PipeEventWatcher();

		bool AsyncWait();
		void Notify();
		evpp_socket_t wfd() const
		{ 
			return m_arrPipe[0]; 
		}
	private:
		virtual bool DoInit();
		virtual void DoClose();
		static void HandlerFn(evpp_socket_t fd, short which, void* v);

		evpp_socket_t m_arrPipe[2]; // Write to pipe_[0] , Read from pipe_[1]
	};
	

	class  SignalEventWatcher : public EventWatcher
	{
	public:
		SignalEventWatcher(signal_number_t signo, EventLoop* loop, const Handler& handler);
		SignalEventWatcher(signal_number_t signo, EventLoop* loop, Handler&& handler);

		bool AsyncWait();
	private: 
		virtual bool DoInit();
		static void HandlerFn(signal_number_t sn, short which, void* v);

		int m_nSigNo;
	};

	class  TimerEventWatcher : public EventWatcher
	{
	public:
		TimerEventWatcher(EventLoop* loop, const Handler& handler, Duration timeout);
		TimerEventWatcher(EventLoop* loop, Handler&& handler, Duration timeout);
		TimerEventWatcher(struct event_base* loop, const Handler& handler, Duration timeout);
		TimerEventWatcher(struct event_base* loop, Handler&& handler, Duration timeout);

		bool AsyncWait();

	private:
		virtual bool DoInit();
		static void HandlerFn(evpp_socket_t fd, short which, void* v);
	private:
		Duration m_duTimeOut;
	};



}

