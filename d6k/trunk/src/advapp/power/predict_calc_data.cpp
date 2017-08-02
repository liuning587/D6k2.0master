
#include "power_define.h"

#include "predict_calc_data.h"

CPredictCalcData::CPredictCalcData()
{
	m_isInit = false;
	m_pUltraShortPredict_4 = nullptr;
	m_pShortPredict_72 = nullptr;
}

CPredictCalcData::~CPredictCalcData()
{

}

void CPredictCalcData::SetUltraShortPredict_4_Tagname(const std::string& strUltraShortPredict4Tagname)
{
	m_strUltraShortPredict4Tagname = strUltraShortPredict4Tagname;
}

void CPredictCalcData::SetShortPredict_72_Tagname(const std::string& strShortPredict72Tagname)
{
	m_strShortPredict72Tagname = strShortPredict72Tagname;
}

bool CPredictCalcData::Init()
{
	if (m_strUltraShortPredict4Tagname.empty() || m_strShortPredict72Tagname.empty())
	{
		return false;
	}

	m_pUltraShortPredict_4 = std::make_shared<CAinData>(m_strUltraShortPredict4Tagname);
	m_pShortPredict_72 = std::make_shared<CAinData>(m_strShortPredict72Tagname);

	m_isInit = true;

	return true;
}

bool CPredictCalcData::GetRealTimeData()
{
	Q_ASSERT(m_pUltraShortPredict_4 && m_pShortPredict_72);
	if (m_pUltraShortPredict_4 == nullptr || m_pShortPredict_72 == nullptr)
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
	bRet = m_pUltraShortPredict_4->GetRealTimeData();
	bRet |= m_pShortPredict_72->GetRealTimeData();

	return bRet;
}

bool CPredictCalcData::GetRealDataOfUltraShortPredict_4(fp64* value)
{
	Q_ASSERT(m_pUltraShortPredict_4 && m_isInit);
	if (m_pUltraShortPredict_4 == nullptr)
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

	if (!m_pUltraShortPredict_4->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pUltraShortPredict_4->GetValue();

	return true;
}

bool CPredictCalcData::GetRealDataOfShortPredict_72(fp64* value)
{
	Q_ASSERT(m_pShortPredict_72 && m_isInit);
	if (m_pShortPredict_72 == nullptr)
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

	if (!m_pShortPredict_72->GetRealTimeData())
	{
		// log
		return false;
	}

	*value = m_pShortPredict_72->GetValue();

	return true;
}

void CPredictCalcData::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}
