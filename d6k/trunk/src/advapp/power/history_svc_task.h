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

// ������ʷ����
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
	// ��ʱ5���ӽ��������ݡ����й������޹�����������ݱ��浽���ݿ���
	void Slot_Handle_Timeout(const void* act = 0);

private:
	
	// �����еĲ�ֵ���浽������
	void Insert_All_History_Data(const QDateTime& insertTime);

	// ʱ���ʽ����yyyy-MM-dd��
	QString Set_Date_Format(const QDateTime &dateTime);
	

private:
	// ��ʱ��
	QTimer* m_pTimer;

	// �ೡվ��Ϣ�洢
	const std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

	// DB�ӿ�
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // HIS_SVC_TASK_H
