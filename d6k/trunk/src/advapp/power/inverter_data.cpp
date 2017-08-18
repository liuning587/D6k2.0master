
#include "power_define.h"

#include "inverter_data.h"

CInverterData::CInverterData()
{
	m_isInit = false;

	m_pInverterStatus = nullptr;
	m_pInverterActivePower = nullptr;
	m_pInverterReactivePower = nullptr;
}

CInverterData::~CInverterData()
{

}

void CInverterData::SetInverterStatusTagname(const std::string& strInverterTagname)
{
	m_strInverterTagname = strInverterTagname;
}

void CInverterData::SetInverterActivePowerTagname(const std::string& strInverterActiveTagname)
{
	m_strInverterActiveTagname = strInverterActiveTagname;
}

void CInverterData::SetInverterReactivePowerTagname(const std::string& strInverterReactiveTagname)
{
	m_strInverterReactiveTagname = strInverterReactiveTagname;
}

void CInverterData::SetInverterVoltageTagname(const std::string& strInverterVoltageTagname)
{
	m_strInverterVoltageTagname = strInverterVoltageTagname;
}

bool CInverterData::Init()
{
	if (m_strInverterTagname.empty() || m_strInverterActiveTagname.empty() || m_strInverterReactiveTagname.empty() || m_strInverterVoltageTagname.empty())
	{
		// log
		return false;
	}

	m_pInverterStatus = std::make_shared<CDinData>(m_strInverterTagname);

	m_pInverterActivePower = std::make_shared<CAinData>(m_strInverterActiveTagname);

	m_pInverterReactivePower = std::make_shared<CAinData>(m_strInverterReactiveTagname);

	m_pInverterVoltage = std::make_shared<CAinData>(m_strInverterVoltageTagname);

	m_isInit = true;

	return true;
}

bool CInverterData::GetRealTimeData()
{
	Q_ASSERT(m_pInverterStatus && m_pInverterActivePower && m_pInverterReactivePower);
	if (m_pInverterStatus == nullptr || m_pInverterActivePower == nullptr || m_pInverterReactivePower == nullptr)
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
	bRet = m_pInverterStatus->GetRealTimeData();
	bRet |= m_pInverterActivePower->GetRealTimeData();
	bRet |= m_pInverterReactivePower->GetRealTimeData();

	return bRet;
}

bool CInverterData::GetRealDataOfStatus(int8u* value)
{
	Q_ASSERT(m_pInverterStatus && m_isInit);
	if (m_pInverterStatus == nullptr)
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

	if (!m_pInverterStatus->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pInverterStatus->GetValue();

	return true;
}

bool CInverterData::GetRealDataOfActivePower(fp64* value)
{
	Q_ASSERT(m_pInverterActivePower && m_isInit);
	if (m_pInverterActivePower == nullptr)
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

	if (!m_pInverterActivePower->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pInverterActivePower->GetValue();

	return true;
}

bool CInverterData::GetRealDataOfVoltage(fp64* value)
{
	Q_ASSERT(m_pInverterVoltage && m_isInit);
	if (m_pInverterVoltage == nullptr)
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

	if (!m_pInverterVoltage->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pInverterVoltage->GetValue();

	return true;
}

bool CInverterData::GetRealDataOfReactivePower(fp64* value)
{
	Q_ASSERT(m_pInverterReactivePower && m_isInit);
	if (m_pInverterReactivePower == nullptr)
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

	if (!m_pInverterReactivePower->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pInverterReactivePower->GetValue();

	return true;
}

void CInverterData::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}
