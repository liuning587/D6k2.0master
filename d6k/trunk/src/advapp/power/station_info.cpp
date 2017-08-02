
#include "station_info.h"

CStationInfo::CStationInfo()
{
	m_vecStationInfo.clear();

	m_pPlantData = nullptr;
	m_pInverterData = nullptr;
	m_pWeatherStationData = nullptr;
	m_pPredictCalcData = nullptr;
}

CStationInfo::~CStationInfo()
{
}

bool CStationInfo::Init()
{
	m_pPlantData = std::make_shared<CPlantData>();
	m_pInverterData = std::make_shared<CInverterData>();
	m_pWeatherStationData = std::make_shared<CWeatherStationData>();
	m_pPredictCalcData = std::make_shared<CPredictCalcData>();

	return true;
}

const std::vector<DATA_INFO>& CStationInfo::GetStationInfo()
{
	return m_vecStationInfo;
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

std::shared_ptr<CInverterData> CStationInfo::GetInverter()
{
	Q_ASSERT(m_pInverterData);
	if (m_pInverterData == nullptr)
	{
		m_pInverterData = std::make_shared<CInverterData>();
	}

	return m_pInverterData;
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

