#pragma once

#ifndef PREDICT_SVC_TASK
#define PREDICT_SVC_TASK

#include <vector>

#include "base_module.h"
#include "station_info.h"
#include "base_task.h"
#include "dbapi.h"

// Ԥ���������
class CPredictSvcTask : public CBaseTask
{
public:
	CPredictSvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo, const unsigned int uTime = 5);
	virtual ~CPredictSvcTask();

protected:
	virtual void DoWork();

private:
	// ��ʱ15����Ԥ��һ��,����Ԥ�����ݱ��浽���ݿ���
	int Handle_timeout(const void* act = 0);

private:
	// ��ȡ������Ҫ��ʵʱ����

	// ����DB����(�������ݿ��е�����Ԥ������)���ڼ���
	bool LoadDBInfo();

	// ����Ԥ���㷨
	bool LoadPredictCalc();	

	// ����������ݵ����ݿ���
	bool SaveShortDataToDB();

	// ����������������һ������ݵ����ݿ��� ��power_predict_72h_final_15m��
	bool SaveLastShortDataToDB();

	// ���泬�������ݵ����ݿ���
	bool SaveUltraShortDataToDB();
	
	// ���泬���������е����һ��15�������ݵ����ݿ��� ��power_predict_4h_final_15m��
	bool SaveLastDayUltraShortDataToDB();

	// �ೡվ��Ϣ�洢
	std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

private:
	// ����Ԥ������
	// std::vector<>

	// ������Ԥ������
	// std::vector<>

	// DB�ӿ�
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // PREDICT_SVC_TASK
