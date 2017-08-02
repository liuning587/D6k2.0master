#pragma once

#ifndef HIS_SVC_TASK_H
#define HIS_SVC_TASK_H

#include <vector>

#include "station_info.h"

#include "dbapi.h"

#include <QThread>
#include <QObject>
#include <QTImer>

// 处理历史数据
class CHistorySvcTask : public QThread
{
public:
	CHistorySvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo);
	virtual ~CHistorySvcTask();

public:
	void CreateTimer();
	bool StartTimer();
	void StopTimer();

protected:
	virtual void run();

public slots:
	// 定时5分钟将气象数据、总有功、总无功和逆变器数据保存到数据库中
	void Slot_Handle_Timeout(const void* act = 0);

private:
	// 定时器
	QTimer* m_pTimer;

	// 多场站信息存储
	std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

	// DB接口
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // HIS_SVC_TASK_H
