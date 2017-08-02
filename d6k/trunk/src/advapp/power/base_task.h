#pragma once

#ifndef BASE_TASK_H
#define BASE_TASK_H


#include <QThread>
#include <QObject>
#include <QTImer>

class CBaseTask : public QThread
{
public:
	CBaseTask(const unsigned int uTime = 5);
	virtual ~CBaseTask();

	virtual void DoWork() = 0;
private:
	void CreateTimer();
	bool StartTimer();
	void StopTimer();

protected:
	virtual void run();

public slots:
	void Slot_Handle_Timeout(const void* act = 0);

private:
	// 定时器
	QTimer* m_pTimer;

	// 定时周期
	const unsigned int& m_uTime;
};

#endif // BASE_TASK_H
