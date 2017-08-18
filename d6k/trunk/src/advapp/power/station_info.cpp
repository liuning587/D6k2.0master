
#include "station_info.h"

CStationInfo::CStationInfo()
{
	m_vecStationInfo.clear();

	m_pPlantData = nullptr;	
	m_pWeatherStationData = nullptr;
	m_pPredictCalcData = nullptr;

	m_vecInverterData.clear();

	m_nStationID = 0;
	m_strStationName = "";
	m_strAlgorithmName = "";
	m_strReversePath = "";
}

CStationInfo::~CStationInfo()
{
}

bool CStationInfo::Init()
{
	if (m_pPlantData == nullptr)
	{
		m_pPlantData = std::make_shared<CPlantData>();
	}
	
	if (m_pWeatherStationData == nullptr)
	{
		m_pWeatherStationData = std::make_shared<CWeatherStationData>();
	}
	
	if (m_pPredictCalcData == nullptr)
	{
		m_pPredictCalcData = std::make_shared<CPredictCalcData>();
	}

	return true;
}

const std::vector<DATA_INFO>& CStationInfo::GetStationInfo()
{
	return m_vecStationInfo;
}

void CStationInfo::SetStationInfo(DATA_INFO& dataInfo)
{
	m_vecStationInfo.push_back(dataInfo);
}

std::shared_ptr<CPlantData> CStationInfo::GetPlant()
{
	Q_ASSERT(m_pPlantData);
	if (m_pPlantData == nullptr)
	{
		m_pPlantData = std::make_shared<CPlantData>();
	}

	return m_pPlantData;
}

std::vector<std::shared_ptr<CInverterData>>& CStationInfo::GetInverter()
{
	return m_vecInverterData;
}

std::shared_ptr<CWeatherStationData> CStationInfo::GetWeatherStation()
{
	Q_ASSERT(m_pWeatherStationData);
	if (m_pWeatherStationData == nullptr)
	{
		m_pWeatherStationData = std::make_shared<CWeatherStationData>();
	}

	return m_pWeatherStationData;
}

std::shared_ptr<CPredictCalcData> CStationInfo::GetPredictCalc()
{
	Q_ASSERT(m_pPredictCalcData);
	if (m_pPredictCalcData == nullptr)
	{
		m_pPredictCalcData = std::make_shared<CPredictCalcData>();
	}

	return m_pPredictCalcData;
}

void CStationInfo::SetStationID(const int nStationID)
{
	m_nStationID = nStationID;
}

const int CStationInfo::GetStationID() const
{
	return m_nStationID;
}

void CStationInfo::SetStationName(const std::string strStationName)
{
	m_strStationName = strStationName;
}

const std::string CStationInfo::GetStationName() const
{
	return m_strStationName;
}

void CStationInfo::SetAlgorithmName(const std::string strAlgorithmName)
{
	m_strAlgorithmName = strAlgorithmName;
}

const std::string CStationInfo::GetAlgorithmName() const
{
	return m_strAlgorithmName;
}

void CStationInfo::SetReversePath(const std::string strReversePath)
{
	m_strReversePath = strReversePath;
}

const std::string CStationInfo::GetReversePath() const
{
	return m_strReversePath;
}

