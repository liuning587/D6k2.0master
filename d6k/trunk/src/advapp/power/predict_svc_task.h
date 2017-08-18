#pragma once

#ifndef PREDICT_SVC_TASK
#define PREDICT_SVC_TASK

#include <vector>
#include <QLibrary>

#include "base_module.h"
#include "station_info.h"
#include "dbapi.h"

// Ԥ���������
class CPredictSvcTask : public CBaseModule
{
public:
	CPredictSvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo);
	virtual ~CPredictSvcTask();

	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
	
protected:

	// �߳�ִ�к���
	virtual void MainLoop();

private:

	// ����Ԥ���㷨
	bool LoadPredictCalc();	

	// �ೡվ��Ϣ�洢
	const std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

private:
	// �Ƿ��˳��ؼ���
	volatile bool m_bQuit;

	std::vector<QLibrary*> m_vecLib;

	// library�����ݹ���
	QMap<QLibrary*, std::vector<std::shared_ptr<CStationInfo>>> m_mapLibAndData;
};

#endif // PREDICT_SVC_TASK
