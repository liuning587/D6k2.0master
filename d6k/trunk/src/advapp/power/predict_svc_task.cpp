
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

	// 加载算法插件并初始化
	for (auto var : m_vecStationInfo)
	{
		// 获取算法
		std::string strAlgmName = var->GetAlgorithmName();
		mapAlgorithmName[strAlgmName].push_back(var);
	}

	// todo
	// 算法的启动和停止接口要提取成功功能接口

	Q_ASSERT(mapAlgorithmName.size() > 0);
	if (mapAlgorithmName.size() <= 0)
	{
		// log
		return false;
	}

	// 算法动态库名
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

		// 保存lib方便关闭和unload。
		m_vecLib.push_back(m_pModelLib);

		m_mapLibAndData[m_pModelLib] = mapAlgorithmName[var];
	}

	return true;
}

void CPredictSvcTask::Run()
{
	// 运行算法
	for (auto var : m_vecLib)
	{
		START_ALGORITHM start = (START_ALGORITHM)var->resolve("StartAlgorithm");
		if (start == nullptr)
		{
			// log
			continue;
		}

		// 启动算法
		start(m_mapLibAndData[var]);
	}
}

void CPredictSvcTask::Shutdown()
{
	// 关闭算法
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


