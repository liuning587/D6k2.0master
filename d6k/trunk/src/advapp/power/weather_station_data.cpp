
#include "power_define.h"

#include "weather_station_data.h"

CWeatherStationData::CWeatherStationData()
{
	m_isInit = false;

	m_pTotalRadiation = nullptr;
	m_pDirectRadiation = nullptr;
	m_pScattRadiation = nullptr;
	m_pAvergWindDirect = nullptr;
	m_pAvergWindSpeed = nullptr;
	m_pAirTemperature = nullptr;
	m_pRelativeHumdty = nullptr;
	m_pAirPressure = nullptr;
}

CWeatherStationData::~CWeatherStationData()
{
	m_isInit = false;
}

void CWeatherStationData::SetTotalRadiationTagname(const std::string& strTotalRadiationTagname)
{
	m_strTotalRadiationTagname = strTotalRadiationTagname;
}

void CWeatherStationData::SetDirectRadiationTagname(const std::string& strDirectRadiationTagname)
{
	m_strDirectRadiationTagname = strDirectRadiationTagname;
}

void CWeatherStationData::SetScattRadiationTagname(const std::string& strScattRadiationTagname)
{
	m_strScattRadiationTagname = strScattRadiationTagname;
}

void CWeatherStationData::SetAvergWindDirectTagname(const std::string& strAvergWindDirectTagname)
{
	m_strAvergWindDirectTagname = strAvergWindDirectTagname;
}

void CWeatherStationData::SetAvergWindSpeedTagname(const std::string& strAvergWindSpeedTagname)
{
	m_strAvergWindSpeedTagname = strAvergWindSpeedTagname;
}

void CWeatherStationData::SetAirTemperatureTagname(const std::string& strAirTemperatureTagname)
{
	m_strAirTemperatureTagname = strAirTemperatureTagname;
}

void CWeatherStationData::SetRelativeHumdtyTagname(const std::string& strRelativeHumdtyTagname)
{
	m_strRelativeHumdtyTagname = strRelativeHumdtyTagname;
}

void CWeatherStationData::SetAirPressureTagname(const std::string& strAirPressureTagname)
{
	m_strAirPressureTagname = strAirPressureTagname;
}

void CWeatherStationData::SetComponentDescriptionTagname(const std::string& strComponentDescription)
{
	m_strComponentDescription = strComponentDescription;
}

bool CWeatherStationData::Init()
{
	if (m_strTotalRadiationTagname.empty() || m_strDirectRadiationTagname.empty() ||
		m_strScattRadiationTagname.empty() || m_strAvergWindDirectTagname.empty() ||
		m_strAvergWindSpeedTagname.empty() || m_strAirTemperatureTagname.empty() ||
		m_strRelativeHumdtyTagname.empty() || m_strAirPressureTagname.empty() || m_strComponentDescription.empty())
	{
		return false;
	}

	m_pTotalRadiation = std::make_shared<CAinData>(m_strTotalRadiationTagname);
	m_pDirectRadiation = std::make_shared<CAinData>(m_strDirectRadiationTagname);
	m_pScattRadiation = std::make_shared<CAinData>(m_strScattRadiationTagname);
	m_pAvergWindDirect = std::make_shared<CAinData>(m_strAvergWindDirectTagname);
	m_pAvergWindSpeed = std::make_shared<CAinData>(m_strAvergWindSpeedTagname);
	m_pAirTemperature = std::make_shared<CAinData>(m_strAirTemperatureTagname);
	m_pRelativeHumdty = std::make_shared<CAinData>(m_strRelativeHumdtyTagname);
	m_pAirPressure = std::make_shared<CAinData>(m_strAirPressureTagname);
	m_pComponentDescription = std::make_shared<CAinData>(m_strComponentDescription);

	m_isInit = true;

	return true;
}

bool CWeatherStationData::GetRealTimeData()
{
	Q_ASSERT(m_pTotalRadiation && m_pDirectRadiation && m_pScattRadiation && m_pAvergWindDirect &&
		m_pAvergWindSpeed && m_pAirTemperature && m_pRelativeHumdty && m_pAirPressure);
	if (m_pTotalRadiation == nullptr || m_pDirectRadiation == nullptr || m_pScattRadiation == nullptr || m_pAvergWindDirect == nullptr ||
		m_pAvergWindSpeed == nullptr || m_pAirTemperature == nullptr || m_pRelativeHumdty == nullptr || m_pAirPressure == nullptr)
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
	bRet = m_pTotalRadiation->GetRealTimeData();
	bRet |= m_pDirectRadiation->GetRealTimeData();
	bRet |= m_pScattRadiation->GetRealTimeData();
	bRet |= m_pAvergWindDirect->GetRealTimeData();
	bRet |= m_pAvergWindSpeed->GetRealTimeData();
	bRet |= m_pAirTemperature->GetRealTimeData();
	bRet |= m_pRelativeHumdty->GetRealTimeData();
	bRet |= m_pAirPressure->GetRealTimeData();

	return bRet;
}

bool CWeatherStationData::GetRealDataOfTotalRadiation(fp64* value)
{
	Q_ASSERT(m_pTotalRadiation && m_isInit);
	if (m_pTotalRadiation == nullptr)
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

	if (!m_pTotalRadiation->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pTotalRadiation->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfDirectRadiation(fp64* value)
{
	Q_ASSERT(m_pDirectRadiation && m_isInit);
	if (m_pDirectRadiation == nullptr)
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

	if (!m_pDirectRadiation->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pDirectRadiation->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfScattRadiation(fp64* value)
{
	Q_ASSERT(m_pScattRadiation && m_isInit);
	if (m_pScattRadiation == nullptr)
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

	if (!m_pScattRadiation->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pScattRadiation->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfAvergWindDirect(fp64* value)
{
	Q_ASSERT(m_pAvergWindDirect && m_isInit);
	if (m_pAvergWindDirect == nullptr)
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

	if (!m_pAvergWindDirect->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pAvergWindDirect->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfAvergWindSpeed(fp64* value)
{
	Q_ASSERT(m_pAvergWindSpeed && m_isInit);
	if (m_pAvergWindSpeed == nullptr)
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

	if (!m_pAvergWindSpeed->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pAvergWindSpeed->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfAirTemperature(fp64* value)
{
	Q_ASSERT(m_pAirTemperature && m_isInit);
	if (m_pAirTemperature == nullptr)
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

	if (!m_pAirTemperature->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pAirTemperature->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfRelativeHumdty(fp64* value)
{
	Q_ASSERT(m_pRelativeHumdty && m_isInit);
	if (m_pRelativeHumdty == nullptr)
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

	if (!m_pRelativeHumdty->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pRelativeHumdty->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfAirPressure(fp64* value)
{
	Q_ASSERT(m_pAirPressure && m_isInit);
	if (m_pAirPressure == nullptr)
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

	if (!m_pAirPressure->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pAirPressure->GetValue();

	return true;
}

bool CWeatherStationData::GetRealDataOfComponentDescription(fp64* value)
{
	Q_ASSERT(m_pComponentDescription && m_isInit);
	if (m_pComponentDescription == nullptr)
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

	if (!m_pComponentDescription->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pComponentDescription->GetValue();

	return true;
}

void CWeatherStationData::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}

