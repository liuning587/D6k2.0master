
#include "power_define.h"

#include "plant_data.h"

CPlantData::CPlantData()
{
	m_isInit = false;
	m_pActPower = nullptr;
	m_pReactPower = nullptr;
}

CPlantData::~CPlantData()
{
	m_isInit = false;
}

void CPlantData::SetActPowerTagname(const std::string& strActPowerTagame)
{
	m_strActPowerTagname = strActPowerTagame;
}

void CPlantData::SetReactPowerTagname(const std::string& strReactPowerTagname)
{
	m_strReactPowerTagname = strReactPowerTagname;
}

bool CPlantData::Init()
{
	if (m_strActPowerTagname.empty() || m_strReactPowerTagname.empty())
	{
		return false;
	}

	m_pActPower = std::make_shared<CAinData>(m_strActPowerTagname);

	m_pReactPower = std::make_shared<CAinData>(m_strReactPowerTagname);

	m_isInit = true;

	return true;
}

bool CPlantData::GetRealTimeData()
{
	Q_ASSERT(m_pActPower && m_pReactPower);
	if (m_pActPower == nullptr || m_pReactPower == nullptr)
	{
		// log
		return false;
	}

	if (!m_isInit)
	{
		if (!Init())
		{
			// log
			return false;
		}
	}

	bool bRet = false;
	bRet = m_pActPower->GetRealTimeData();
	bRet |= m_pReactPower->GetRealTimeData();
	
	return bRet;
}


bool CPlantData::GetRealDataOfActPower(fp64* value)
{
	Q_ASSERT(m_pActPower && m_isInit);
	if (m_pActPower == nullptr)
	{
		// log
		return false;
	}

	if (!m_isInit)
	{
		if (!Init())
		{
			// log
			return false;
		}
	}

	if (!m_pActPower->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pActPower->GetValue();

	return true;
}

bool CPlantData::GetRealDataOfReactPower(fp64* value)
{
	Q_ASSERT(m_pReactPower && m_isInit);
	if (m_pReactPower == nullptr)
	{
		// log
		return false;
	}

	if (!m_isInit)
	{
		if (!Init())
		{
			// log
			return false;
		}
	}

	if (!m_pReactPower->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pReactPower->GetValue();

	return true;
}

void CPlantData::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}
