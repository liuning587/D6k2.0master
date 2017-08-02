#pragma once

#ifndef HIS_SVC_TASK_H
#define HIS_SVC_TASK_H

#include <vector>

#include "station_info.h"

#include "dbapi.h"

#include <QThread>
#include <QObject>
#include <QTImer>

// ������ʷ����
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
	// ��ʱ5���ӽ��������ݡ����й������޹�����������ݱ��浽���ݿ���
	void Slot_Handle_Timeout(const void* act = 0);

private:
	// ��ʱ��
	QTimer* m_pTimer;

	// �ೡվ��Ϣ�洢
	std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

	// DB�ӿ�
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // HIS_SVC_TASK_H
