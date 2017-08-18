#pragma once

#ifndef HIS_SVC_TASK_H
#define HIS_SVC_TASK_H

#include <vector>

#include "station_info.h"

#include "dbapi.h"

#include <QThread>
#include <QObject>
#include <QTImer>
#include <QDateTime>

// 处理历史数据
class CHistorySvcTask : public QThread
{
public:
	CHistorySvcTask(const std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo);
	virtual ~CHistorySvcTask();

public:
	void CreateTimer();
	bool StartTimer();
	void StopTimer();

protected:
	virtual void run();

protected:
	// log
	void LogMsg(const char *szLogTxt, int nLevel);

public slots:
	// 定时5分钟将气象数据、总有功、总无功和逆变器数据保存到数据库中
	void Slot_Handle_Timeout(const void* act = 0);

private:
	
	// 将所有的测值保存到数据中
	void Insert_All_History_Data(const QDateTime& insertTime);

	// 时间格式化（yyyy-MM-dd）
	QString Set_Date_Format(const QDateTime &dateTime);
	

private:
	// 定时器
	QTimer* m_pTimer;

	// 多场站信息存储
	const std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

	// DB接口
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // HIS_SVC_TASK_H
