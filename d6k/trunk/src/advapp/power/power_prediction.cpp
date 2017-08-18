
#include "power_prediction.h"

#include <QFileInfo>

#include "power_define.h"

CPowerPrediction::CPowerPrediction(int32u nOccNo)
{
	m_pReadInfo = nullptr;
	m_pHisSvcTask = nullptr;
	m_pPredictCalcTask = nullptr;
	m_pWeatherFileTask = nullptr;
}

CPowerPrediction::~CPowerPrediction()
{
	Shutdown();
}

bool CPowerPrediction::Init(const char * pszProjectName)
{
	// eg:F:/work/trunk/project/xxxx
	QString strRootPath = QFileInfo(m_strPathName.c_str()).path();

	strRootPath += strRootPath + POWER_PREDICT_PATH_NAME;
	m_strPathName = strRootPath.toStdString();

	Q_ASSERT(!m_strPathName.empty());
	if (m_strPathName.empty())
	{
		return false;
	}

	// 调用父类接口，初始化内存库
	Initalize(pszProjectName);

	// 启动log
	CreateLog(LOG_FILENAME);

	// 加载组态配置生成的文件
	if (!LoadConfig())
	{
		// log
		return true;
	}

	Q_ASSERT(m_vecStationInfo.size() > 0);
	if (m_vecStationInfo.size() < 1)
	{
		// log
		return false;
	}

	m_pReadInfo = std::make_shared<CReadPowerXml>(m_strPathName);

	m_pHisSvcTask = std::make_shared<CHistorySvcTask>(m_vecStationInfo);

	m_pPredictCalcTask = std::make_shared<CPredictSvcTask>(m_vecStationInfo);
	m_pPredictCalcTask->Initialize();

	m_pWeatherFileTask = std::make_shared<CWeatherFileSvcTask>(WEATHER_FILE_DATA_SAVE_PERIOD);

	return true;
}

bool CPowerPrediction::Run()
{
	Q_ASSERT(m_pReadInfo && m_pHisSvcTask && m_pPredictCalcTask && m_pWeatherFileTask);
	if (m_pReadInfo == nullptr || m_pHisSvcTask == nullptr || m_pPredictCalcTask == nullptr || m_pWeatherFileTask == nullptr)
	{
		return false;
	}

	Q_ASSERT(m_vecStationInfo.size() > 0);
	if (m_vecStationInfo.size() < 1)
	{
		// log
		return false;
	}

	// todo:
	// 系统启动先要检查ftp反向隔离来的文件吗？
	

	// 启动历史数据存储线程
	m_pHisSvcTask->start();

	// 启动预测数据存储线程
	m_pPredictCalcTask->Run();

	// 启动处理天气预报文件数据线程
	m_pWeatherFileTask->start();

	return true;
}

void CPowerPrediction::Shutdown()
{
	// 关闭内存库
	Shutdown();

	if (m_pHisSvcTask != nullptr)
	{
		m_pHisSvcTask->terminate();
		m_pHisSvcTask->wait();
	}

	if (m_pPredictCalcTask != nullptr)
	{
		m_pPredictCalcTask->Shutdown();
	}

	if (m_pWeatherFileTask != nullptr)
	{
		m_pWeatherFileTask->terminate();
		m_pWeatherFileTask->wait();
	}
}

bool CPowerPrediction::LoadConfig()
{
	Q_ASSERT(m_pReadInfo);
	if (m_pReadInfo == nullptr)
	{
		return false;
	}

	if (!m_pReadInfo->LoadInfo())
	{
		return false;
	}

	m_vecStationInfo = m_pReadInfo->GetAllStationInfo();

	return true;
}

// 处理历史任务
bool CPowerPrediction::HistoryDataHandle()
{
	// 定时5分钟历史数据存储

	return true;
}

// 预测处理
bool CPowerPrediction::PredictHandle()
{
	// 定时15分钟预测一次。
	// 加载预测算法
	// LoadPredictCalc();

	return true;
}

bool CPowerPrediction::WeatherFileHandle()
{
	return true;
}

void CPowerPrediction::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}

std::vector<std::shared_ptr<CStationInfo>>& CPowerPrediction::GetStationInfo()
{
	return m_vecStationInfo;
}
