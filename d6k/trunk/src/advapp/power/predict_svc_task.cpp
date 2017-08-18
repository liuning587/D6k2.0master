
#include "predict_svc_task.h"

#include <string>

#include <QMap>

typedef int (*START_ALGORITHM)(std::vector<std::shared_ptr<CStationInfo>>);
typedef void (*STOP_ALGORITHM)();

CPredictSvcTask::CPredictSvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo) : m_vecStationInfo(vecStationInfo)
{

}

CPredictSvcTask::~CPredictSvcTask()
{
	Shutdown();
}

bool CPredictSvcTask::Initialize()
{
	if (m_vecStationInfo.size() <= 0)
	{
		// log
		return false;
	}

	QMap<std::string, std::vector<std::shared_ptr<CStationInfo>>> mapAlgorithmName;

	// �����㷨�������ʼ��
	for (auto var : m_vecStationInfo)
	{
		// ��ȡ�㷨
		std::string strAlgmName = var->GetAlgorithmName();
		mapAlgorithmName[strAlgmName].push_back(var);
	}

	// todo
	// �㷨��������ֹͣ�ӿ�Ҫ��ȡ�ɹ����ܽӿ�

	Q_ASSERT(mapAlgorithmName.size() > 0);
	if (mapAlgorithmName.size() <= 0)
	{
		// log
		return false;
	}

	// �㷨��̬����
	for (auto var : mapAlgorithmName.keys())
	{
		if (var.empty() || mapAlgorithmName[var].size() <= 0)
		{
			// log
			continue;
		}

		QLibrary* m_pModelLib = new QLibrary(var.c_str());

		if (!m_pModelLib->load())
		{
			// log
			continue;
		}		

		// ����lib����رպ�unload��
		m_vecLib.push_back(m_pModelLib);

		m_mapLibAndData[m_pModelLib] = mapAlgorithmName[var];
	}

	return true;
}

void CPredictSvcTask::Run()
{
	// �����㷨
	for (auto var : m_vecLib)
	{
		START_ALGORITHM start = (START_ALGORITHM)var->resolve("StartAlgorithm");
		if (start == nullptr)
		{
			// log
			continue;
		}

		// �����㷨
		start(m_mapLibAndData[var]);
	}
}

void CPredictSvcTask::Shutdown()
{
	// �ر��㷨
	for (auto var : m_vecLib)
	{
		if (var == nullptr)
		{
			continue;
		}

		STOP_ALGORITHM stop = (STOP_ALGORITHM)var->resolve("StopAlgorithm");
		if (stop)
		{
			stop();
		}

		var->unload();

		delete var;
	}
}

void CPredictSvcTask::MainLoop()
{
	// 
}


