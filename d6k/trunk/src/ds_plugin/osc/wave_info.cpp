

#if _MSC_VER >= 1600
#define _WIN32_WINNT  0x0500
#endif

#if 0
#if _MSC_VER >= 1600
#include <vld.h>
#endif
#endif

#include "wave_info.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include <QDebug>
#include <QFileInfo>

/*! \fn float CChannel::GetSampRawValue(int nIndex) const
*********************************************************************************************************
** \brief CChannel::GetSampRawValue
** \details 获取通道采样值
** \param nIndex
** \return float
** \author LiJin
** \date 2018年2月5日
** \note
********************************************************************************************************/
float CChannel::GetSampRawValue(int nIndex) const
{
	Q_ASSERT(nIndex >= 0 && nIndex < m_arrSampData.size());
	if (nIndex >= 0 && nIndex < m_arrSampData.size())
	{
		return m_arrSampData.at(nIndex);
	}
	return 0.0f;
}

CAnaChannel::CAnaChannel()
{
	m_nIndex = 0;
	m_fA = 0;
	m_fB = 0;
	m_fSkew = 0;
	m_nMin = 0;
	m_nMax = 0;
	m_fPrimary = 0;
	m_fSecondary = 0;

	m_fMaxSampValue = 0;
	m_fMinSampValue = 0;
}

CAnaChannel::~CAnaChannel()
{

}

CDigitalChannel::CDigitalChannel()
{
	m_nIndex = 0;
	m_nY = 0;
}

CDigitalChannel::~CDigitalChannel()
{

}

bool CAnaChannel::LogString(const QString & szLog, int nLevel)const
{
	extern bool LogString(const char *szLogTxt, int nLevel);
	return LogString(szLog.toLocal8Bit().constData(), nLevel);
}
/*! \fn bool CAnaChannel::ParseInfo(const QString & szInput)
*********************************************************************************************************
** \brief CAnaChannel::ParseInfo
** \details 加载模拟通道配置信息
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月16日
** \note
********************************************************************************************************/
bool CAnaChannel::ParseInfo(const QString & szInput)
{
	int nSize = 0;
	QString szTemp;
	bool bOk = false;
	QStringList lstContent = szInput.split(',');

	nSize = lstContent.size();
//	Q_ASSERT(nSize == LI2_ANA_MAX_COUNT || nSize == LI1_ANA_MAX_COUNT);

	if (nSize > 0)
	{
		szTemp = lstContent.at(LI2_INDEX);
		m_nIndex = szTemp.toInt(&bOk);

		Q_ASSERT(bOk);
		if (bOk == false)
			return false;
	
		m_szID = lstContent.at(LI2_ID).toLocal8Bit().constData();
		m_szPhase = lstContent.at(LI2_PHASE).toLocal8Bit().constData();
		m_szCCBM = lstContent.at(LI2_CCBM).toLocal8Bit().constData();
		
		// 单位 必需字段
		m_szUnit = lstContent.at(LI2_UU).toLocal8Bit().constData();

		szTemp = lstContent.at(LI2_A);
		m_fA = szTemp.toFloat(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		szTemp = lstContent.at(LI2_B);
		m_fB = szTemp.toFloat(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		//  SKEW 为非必须
		szTemp = lstContent.at(LI2_SKEW);
		m_fSkew = szTemp.toFloat(&bOk);
// 		Q_ASSERT(bOk);
// 		if (bOk == false)
// 			return false;		 	

		szTemp = lstContent.at(LI2_MIN);
		m_nMin = szTemp.toInt(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		Q_ASSERT(m_nMin <= 99999 && m_nMin >= -99999);

		szTemp = lstContent.at(LI2_MAX);
		m_nMax = szTemp.toInt(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		Q_ASSERT(m_nMax <= 99999 && m_nMax >= -99999);

		if (nSize > LI1_ANA_MAX_COUNT)
		{
			szTemp = lstContent.at(LI1_ANA_MAX_COUNT);
			if (nSize == LI1_ANA_MAX_COUNT+1 && szTemp.isEmpty())
			{// 最后一个为空
				return true;
			}

			szTemp = lstContent.at(LI2_PRIMARY);
			m_fPrimary = szTemp.toFloat(&bOk);
			Q_ASSERT(bOk);
			if (bOk == false)
				return false;

			szTemp = lstContent.at(LI2_SECONDARY);
			m_fSecondary = szTemp.toFloat(&bOk);
			Q_ASSERT(bOk);
			if (bOk == false)
				return false;

			szTemp = lstContent.at(LI2_PS);

			if (szTemp.contains('P') || szTemp.contains('p'))
			{
				m_PSType = P_TYPE;
				return true;
			}
			if (szTemp.contains('S') || szTemp.contains('s'))
			{
				m_PSType = S_TYPE;
				return true;
			}
		}
		else
		{
			return true;
		}	
	}
	return false;
}

bool CAnaChannel::GetValue(int nIndex, float &fValue)const
{
	Q_ASSERT(nIndex >= 0 && nIndex < m_arrSampData.size());
	if (nIndex < 0 || nIndex >= m_arrSampData.size())
		return false;

	float fRet = m_arrSampData.at(nIndex);
	fValue = fRet * m_fA + m_fB;
	return true;
}
/*! \fn bool CDigitalChannel::ParseInfo(const QString & szInput)
*********************************************************************************************************
** \brief CDigitalChannel::ParseInfo
** \details 解析状态通道的配置信息
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月16日
** \note
********************************************************************************************************/
bool CDigitalChannel::ParseInfo(const QString & szInput)
{
	int nSize = 0;
	QString szTemp;
	bool bOk = false;
	QStringList lstContent = szInput.split(',');

	Q_ASSERT(lstContent.size() == LI_DIGI_MAX_COUNT);

	nSize = lstContent.size();
	if (nSize)
	{
		szTemp = lstContent.at(LI_INDEX);
		m_nIndex = szTemp.toInt(&bOk);

		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		m_szID = lstContent.at(LI_ID).toLocal8Bit().constData();
		m_szPhase = lstContent.at(LI_PHASE).toLocal8Bit().constData();
		m_szCCBM = lstContent.at(LI_CCBM).toLocal8Bit().constData();

		szTemp = lstContent.at(LI_Y);
		m_nY = szTemp.toInt(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		Q_ASSERT(m_nY == 0 || m_nY == 1);

		return true;
	}
	return false;
}

bool CDigitalChannel::LogString(const QString & szLog, int nLevel)const
{
	extern bool LogString(const char *szLogTxt, int nLevel);
	return LogString(szLog.toLocal8Bit().constData(), nLevel);
}


CWaveCfg::CWaveCfg()
{
	m_nTotoalChannelCount = 0;
	m_nAChannelCount = 0;
	m_nDChannelCount = 0;

	m_fLineFreq = 0;

	m_nSampCount = 0;

	m_fTimeMult = 0;
}

CWaveCfg::~CWaveCfg()
{

}

bool CWaveCfg::LogString(const QString & szLog, int nLevel)const
{
	extern bool LogString(const char *szLogTxt, int nLevel);
	return LogString(szLog.toLocal8Bit().constData(), nLevel);
}
/*! \fn bool CWaveCfg::ReadFile(const QString & szFilePathName)
*********************************************************************************************************
** \brief CWaveCfg::ReadFile
** \details 加载并解析配置文件
** \param szFilePathName 完整的配置文件名 带后缀.cfg
** \return bool
** \author LiJin
** \date 2018年1月17日
** \note
********************************************************************************************************/
bool CWaveCfg::ReadFile(const QString & szFilePathName)
{
	QString szLog;
	Q_ASSERT(szFilePathName.length() > 3);
	if (szFilePathName.length() < 3)
	{
		szLog = QObject::tr("Open file error! The filename is invalid. name = [%1] ").arg(szFilePathName);
		LogString(szLog, 0);

		return false;
	}

	QFile cfgfile(szFilePathName);

	if (cfgfile.exists() == false)
	{
		szLog = QObject::tr("Cannot find corresponding  (.cfg). file= [%1] ").arg(szFilePathName);
		LogString(szLog, 0);

		cfgfile.close();
		return false;
	}

	if (cfgfile.open(QFile::ReadOnly | QFile::Text) == false)
	{
		szLog = QObject::tr("Cannot open cfg file (.cfg). file= [%1] ").arg(szFilePathName);
		LogString(szLog, 0);

		return false;
	}

	QTextStream cfgstream(&cfgfile);

	QTextCodec *pCodec = QTextCodec::codecForName("gbk");
	cfgstream.setCodec(pCodec);

	QString szLineTemp;
	int nIdx = 0;
	bool bRet = false;

	while (!cfgstream.atEnd())
	{
		szLineTemp = cfgstream.readLine().trimmed();

		nIdx++;

		if (nIdx == LI_STATION_INFO)
		{// 解析厂站信息
			bRet = ParseStation(szLineTemp);
			if (bRet)
				continue;
			else
				break;
		}
		if (nIdx == LI_CHANNEL_INFO)
		{// 解析通道数量信息
			bRet = ParseChannelCount(szLineTemp);
			if (bRet)
				continue;
			else
				break;

		}
		if (nIdx > LI_CHANNEL_INFO)
		{// 解析通道的信息
			if (m_nAChannelCount > 0 && m_nAChannelCount <= kMaxChannelCount)
			{
				if (nIdx > LI_CHANNEL_INFO && nIdx <= LI_CHANNEL_INFO + m_nAChannelCount)
				{
					std::shared_ptr<CAnaChannel> pAna = std::make_shared<CAnaChannel>();
					bRet = pAna->ParseInfo(szLineTemp);

					if (bRet)
					{
						m_arrAnaChannelInfos.emplace_back(pAna);
						continue;
					}
					else
					{
						break;
					}
				}
			}
			if (m_nDChannelCount > 0 && m_nDChannelCount <= kMaxChannelCount)
			{
				if (nIdx > LI_CHANNEL_INFO + m_nAChannelCount && nIdx <= LI_CHANNEL_INFO + m_nAChannelCount + m_nDChannelCount)
				{
					std::shared_ptr<CDigitalChannel> pDigi = std::make_shared<CDigitalChannel>();
					bRet = pDigi->ParseInfo(szLineTemp);

					if (bRet)
					{
						m_arrDigiChannelInfos.emplace_back(pDigi);
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
		// 解析线路频率信息
		if (nIdx > LI_CHANNEL_INFO + m_nTotoalChannelCount && nIdx <= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET)
		{
			bRet = ParseLineFreq(szLineTemp);
			if (bRet)
				continue;
			else
				break;

		}
		// 解析采样率数目
		if (nIdx >= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_SAMPCOUNT_OFFSET && nIdx < LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_SAMPCOUNT_OFFSET + LI_SAMP_OFFSET)
		{
			bRet = ParseSampRateCount(szLineTemp);
			if (bRet)
				continue;
			else
				break;

		}
		if (m_nSampCount)
		{
			// 采样率
			if (nIdx > LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_SAMPCOUNT_OFFSET && nIdx <= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_SAMPCOUNT_OFFSET + m_nSampCount)
			{
				SampRate sr;
				bRet = ParseSampRate(szLineTemp, sr);
				Q_ASSERT(bRet);
				if (bRet)
				{
					m_arrSampRates.emplace_back(sr);
					continue;
				}
				else
				{
					break;
				}
			}
		}
		// 时间标识
		if (nIdx >= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_DT1_OFFSET
			&& nIdx <= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_DT2_OFFSET)
		{
			if (nIdx == LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_DT1_OFFSET)
			{
				bRet = ParseDataTime(szLineTemp, 0);
				if (bRet)
					continue;
				else
					break;
			}

			if (nIdx == LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_DT2_OFFSET)
			{
				bRet = ParseDataTime(szLineTemp, 1);
				if (bRet)
					continue;
				else
					break;

			}
		}
		// 文件类型
		//          通道信息偏移  通道数量 采样 时间标识
		if (nIdx >= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_FT_OFFSET
			&& nIdx < LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_MULT_OFFSET)
		{
			bRet = ParseFileType(szLineTemp);
			if (bRet)
				continue;
			else
				break;
		}
		// 时间标记乘数系数
		if (nIdx >= LI_CHANNEL_INFO + m_nTotoalChannelCount + LI_LINEFREQ_OFFSET + LI_SAMP_OFFSET + m_nSampCount + LI_MULT_OFFSET)			
		{
			ParseTimeMult(szLineTemp);			
		}
	}
	cfgfile.close();

	szLog = QObject::tr("Read config success. cfg file= [%1] ").arg(szFilePathName);
	LogString(szLog, 0);

	return bRet;
}
/*! \fn bool CWaveCfg::ParseStation(const QString & szInput)
*********************************************************************************************************
** \brief CWaveCfg::ParaStation
** \details 解析厂站相关信息
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月15日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseStation(const QString & szInput)
{
	int nSize = 0;

	QStringList lstContent = szInput.split(',');

	Q_ASSERT(lstContent.empty() == false);

	nSize = lstContent.size();
	if (nSize > 0)
	{
		m_szRevYear = lstContent.at(nSize - 1).toLocal8Bit().constData();

		if (nSize >= 3)
		{
			m_szStationName = lstContent.at(0).toLocal8Bit().constData();
			m_szRecDevID = lstContent.at(1).toLocal8Bit().constData();
		}
		return true;
	}
	else
	{// log

	}
	return false;
}
/*! \fn bool CWaveCfg::ParseChannelCount(const QString & szInput)
*********************************************************************************************************
** \brief CWaveCfg::ParseChannelCount
** \details 解析通道数量相关信息
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月16日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseChannelCount(const QString & szInput)
{
	int nSize = 0;

	QStringList lstContent = szInput.split(',');

	Q_ASSERT(lstContent.size() == 3);

	nSize = lstContent.size();
	if (nSize >= 3)
	{
		bool bOk = false;
		QString szTemp = lstContent.at(0);

		m_nTotoalChannelCount = szTemp.toInt(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
		{// log

			return false;
		}

		for (int i = 1; i <= 2; i++)
		{
			szTemp = lstContent.at(i);
			Q_ASSERT(szTemp.length() >= 2 && szTemp.length() <= 7);
			if (szTemp.length() >= 2 && szTemp.length() <= 7)
			{
				if (szTemp.indexOf('A', 0, Qt::CaseInsensitive) > 0)
				{
					m_nAChannelCount = szTemp.mid(0, szTemp.size() - 1).toInt(&bOk);
					if (bOk == false)
					{// log

						return false;
					}
					else
					{
						continue;
					}
				}
				if (szTemp.indexOf('D', 0, Qt::CaseInsensitive) > 0)
				{
					m_nDChannelCount = szTemp.mid(0, szTemp.size() - 1).toInt(&bOk);
					if (bOk == false)
					{// log

						return false;
					}
					else
					{
						continue;
					}
				}
			}
			else
			{ // log

			}
		}

		Q_ASSERT(m_nTotoalChannelCount == m_nDChannelCount + m_nAChannelCount);
	}
	else
	{
		return false;
	}
	return true;
}
/*! \fn bool  CWaveCfg::ParseLineFreq(const QString & szInput)
*********************************************************************************************************
** \brief CWaveCfg::ParseLineFreq
** \details 解析线路频率配置信息
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月16日
** \note
********************************************************************************************************/
bool  CWaveCfg::ParseLineFreq(const QString & szInput)
{
	bool bOk = false;

	m_fLineFreq = szInput.toFloat(&bOk);
	Q_ASSERT(bOk);
	if (bOk == false)
		return false;

	return true;
}
/*! \fn bool CWaveCfg::ParseFileType(const QString & szInput)
*********************************************************************************************************
** \brief CWaveCfg::ParseFileType
** \details 解析文件类型格式
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月16日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseFileType(const QString & szInput)
{
	if (szInput.compare("ASCII", Qt::CaseInsensitive) == 0)
	{
		this->m_nFileType = FT_ASCII;
		return true;
	}
	if (szInput.compare("BINARY", Qt::CaseInsensitive) == 0)
	{
		this->m_nFileType = FT_BINARY;
		return true;
	}
	return false;
}
/*! \fn bool CWaveCfg::ParseSampRate(const QString & szInput)
*********************************************************************************************************
** \brief CWaveCfg::ParseSampRate
** \details
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月17日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseSampRateCount(const QString & szInput)
{
	bool bOk = false;

	m_nSampCount = szInput.toInt(&bOk);
	Q_ASSERT(bOk);
	if (bOk == false)
		return false;
	Q_ASSERT(m_nSampCount >= 0 && m_nSampCount <= 999);
	if (m_nSampCount < 0 || m_nSampCount>999)
		return false;

	return true;
}
/*! \fn bool CWaveCfg::ParseSampRate(const QString & szInput, SampRate & sr)
*********************************************************************************************************
** \brief CWaveCfg::ParseSampRate
** \details 解析采样率等信息
** \param szInput
** \param sr
** \return bool
** \author LiJin
** \date 2018年1月17日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseSampRate(const QString & szInput, SampRate & sr)
{
	bool bOk = false;

	QStringList lstContent = szInput.split(",");
	Q_ASSERT(lstContent.size() == 2);
	if (lstContent.size() < 2)
	{
		return false;
	}
	
	QString szTemp;
	szTemp = lstContent.at(0);
	sr.Samp = szTemp.toFloat(&bOk);
	Q_ASSERT(bOk);
	if (bOk == false)
		return false;

	szTemp = lstContent.at(1);
	sr.EndSamp = szTemp.toInt(&bOk);
	Q_ASSERT(bOk);
	if (bOk == false)
		return false;

	Q_ASSERT(sr.EndSamp >= 1 && sr.EndSamp <= 9999999999);

	if (sr.EndSamp < 1 || sr.EndSamp>9999999999)
		return false;

	return true;
}
/*! \fn bool CWaveCfg::ParseTimeMult(const QString & szInput)
*********************************************************************************************************
** \brief CWaveCfg::ParseTimeMult
** \details 解析乘数系数
** \param szInput
** \return bool
** \author LiJin
** \date 2018年1月17日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseTimeMult(const QString & szInput)
{
	bool bOk = false;

	m_fTimeMult = szInput.toFloat(&bOk);
	Q_ASSERT(bOk);
	if (bOk == false)
		return false;

	return true;
}
/*! \fn bool CWaveCfg::ParseDataTime(const QString & szInput, int nIdx)
*********************************************************************************************************
** \brief CWaveCfg::ParseDataTime
** \details 解析日期时间
** \param szInput
** \param nIdx
** \return bool
** \author LiJin
** \date 2018年1月31日
** \note
********************************************************************************************************/
bool CWaveCfg::ParseDataTime(const QString & szInput, int nIdx)
{
	Q_ASSERT(nIdx == 0 || nIdx == 1);
	if (nIdx == 0)
	{
		m_szDataTime1 = szInput;
	}
	else if (nIdx == 1)
	{
		m_szDataTime2 = szInput;
	}

	return true;
}

/*! \fn bool CWaveCfg::PushSampValue(int nChannelIdx, float fValue)
********************************************************************************************************* 
** \brief CWaveCfg::PushSampValue 
** \details 推送采样值到通道中
** \param nChannelIdx 从0开始
** \param fValue 
** \return bool 
** \author LiJin 
** \date 2018年2月5日 
** \note 
********************************************************************************************************/
bool CWaveCfg::PushSampValue(int nChannelIdx, float fValue)
{
	Q_ASSERT(nChannelIdx >= 0 && nChannelIdx < m_nTotoalChannelCount);
	if (nChannelIdx < 0 || nChannelIdx >= m_nTotoalChannelCount)
		return false;

	if (nChannelIdx < m_nAChannelCount && nChannelIdx < m_arrAnaChannelInfos.size())
	{// 是模拟通道
		auto pAna = m_arrAnaChannelInfos.at(nChannelIdx);
		Q_ASSERT(pAna);
		if (pAna)
		{
			pAna->PushSampValue(fValue);
			return true;
		}
	}

	if (nChannelIdx >= m_nAChannelCount && nChannelIdx < m_nTotoalChannelCount)
	{
		int nDigIndex = nChannelIdx - m_nAChannelCount;
		Q_ASSERT(nDigIndex < m_arrDigiChannelInfos.size());
		if (nDigIndex < m_arrDigiChannelInfos.size())
		{
			auto pDigi = m_arrDigiChannelInfos.at(nDigIndex);
			Q_ASSERT(pDigi);
			if (pDigi)
			{
				pDigi->PushSampValue(fValue);
				// 开关量，非0即1
				Q_ASSERT((fValue > -0.1&&fValue < 0.1) || (fValue > 0.9&&fValue < 1.1));

				return true;
			}
		}
	}
	return false;
}

CWaveDat::CWaveDat(CWaveCfg & conf) : m_Conf(conf)
{

}

CWaveDat::~CWaveDat()
{
	m_arrDataLines.clear();
}

bool CWaveDat::LogString(const QString & szLog, int nLevel)const
{
	extern bool LogString(const char *szLogTxt, int nLevel);
	return LogString(szLog.toLocal8Bit().constData(), nLevel);
}
/*! \fn bool CWaveDat::ReadFile(const QString & szFilePathName)
*********************************************************************************************************
** \brief CWaveDat::ReadFile
** \details 读取数据文件
** \param szFilePathName
** \return bool
** \author LiJin
** \date 2018年1月31日
** \note
********************************************************************************************************/
bool CWaveDat::ReadFile(const QString & szFilePathName)
{
	QString szLog;
	QFile datfile(szFilePathName);

	if (datfile.exists() == false)
	{		
		datfile.close();

		szLog = QObject::tr("Cannot find corresponding (.dat). file= [%1] ").arg(szFilePathName);
		LogString(szLog, 0);

		return false;
	}

	CWaveCfg::FileType ft = m_Conf.GetFileType();

	if (ft == CWaveCfg::FT_ASCII)
	{	
		return ReadAsciiDataFile(szFilePathName);
	}

	return true;
}
/*! \fn bool CWaveDat::ReadAsciiDataFile(const QString & szFilePathName)
********************************************************************************************************* 
** \brief CWaveDat::ReadAsciiDataFile 
** \details 读取ASCII类型的数据文件
** \param szFilePathName 
** \return bool 
** \author LiJin 
** \date 2018年2月2日 
** \note 
********************************************************************************************************/
bool CWaveDat::ReadAsciiDataFile(const QString & szFilePathName)
{
	QString szLog;
	QFile datfile(szFilePathName);

	if (datfile.open(QFile::ReadOnly | QFile::Text) == false)
	{
		datfile.close();

		szLog = QObject::tr("Cannot open (.dat). file= [%1] ").arg(szFilePathName);
		LogString(szLog, 0);

		return false;
	}

	QTextStream datstream(&datfile);

	QTextCodec *pCodec = QTextCodec::codecForName("gbk");
	datstream.setCodec(pCodec);

	QString szLineTemp;
	int nIdx = 0;
	bool bRet = false;

	m_arrDataLines.clear();
	
	std::shared_ptr<CDataLine> pLine;
	
	while (datstream.atEnd() == false)
	{
		szLineTemp = datstream.readLine().trimmed();
		nIdx++;

		pLine = std::make_shared<CDataLine>();
		bRet = pLine->ParseLine(szLineTemp, m_Conf, nIdx, std::bind(&CWaveDat::LogString, this, std::placeholders::_1, std::placeholders::_2));
		if (bRet == false)
		{
			break;
		}
		else
		{
			if (m_arrDataLines.empty() == false)
			{
				if (pLine->GetIndex() !=  m_arrDataLines.back()->GetIndex() + 1)
				{
					szLog = QObject::tr("Warning : ASCII data file line [%1]'s index error. (.dat) file= [%2] ").arg(nIdx).arg(szFilePathName);
					LogString(szLog, 0);
				}
			}
			m_arrDataLines.emplace_back(pLine);
		}
	}

	datfile.close();

	if (bRet)
	{
		szLog = QObject::tr("Read ASCII data file successes. (.dat) file= [%1] ").arg(szFilePathName);
	}
	else
	{
		szLog = QObject::tr("Read ASCII data file failed. (.dat) file= [%1] ").arg(szFilePathName);
	}
	LogString(szLog, 0);

	return bRet;
}

/*! \fn bool CWaveDat::CDataLine::ParseLine(const QString & szInput, CWaveCfg& conf, int nLineIdx,
                                                        std::function<bool(const QString &, int)> fnLog)
********************************************************************************************************* 
** \brief CWaveDat::CDataLine::ParseLine 
** \details 
** \param szInput 
** \param conf 
** \param nLineIdx 
** \param  
** \param & 
** \param  
** \param fnLog 
** \return bool 
** \author LiJin 
** \date 2018年2月5日 
** \note 
********************************************************************************************************/
bool CWaveDat::CDataLine::ParseLine(const QString & szInput, CWaveCfg& conf, int nLineIdx, std::function<bool(const QString &, int)> fnLog)
{
	QString szLog;
	Q_ASSERT(szInput.isEmpty() == false);
	if (szInput.isEmpty())
	{
		szLog = QObject::tr("Data file line is empty. line= [%1] ").arg(nLineIdx);
		fnLog(szLog, 0);
		return false;
	}
	
	QStringList lstContent = szInput.split(',');
	Q_ASSERT(lstContent.size() > 0);

	int nSize = lstContent.size();

	Q_ASSERT(nSize == conf.GetTotalChannelCount() + 2);
	if (nSize != conf.GetTotalChannelCount() + 2 || nSize < 2)
	{
		szLog = QObject::tr("Data file line is incorrect. line= [%1] ").arg(nLineIdx);
		fnLog(szLog, 0);
		return false;
	}
#if 0
	//!todo 此处还需要推敲一下。
	int nSampCount = conf.GetSampCount();
	if (nSampCount==0)
	{// 如果采样数为0，则数据文件中的时间标志为必须
		Q_ASSERT(nSize == conf.GetTotalChannelCount() + 2);
		if (nSize != conf.GetTotalChannelCount() + 2)
		{
			szLog = QObject::tr("Data file line lost timestamp. line= [%1] ").arg(nLineIdx);
			fnLog(szLog, 0);
			return false;
		}
	}
	else
	{

	}
#endif
	bool bOk = false;
	QString szTemp = lstContent.at(0);

	m_nIndex = szTemp.toInt(&bOk);
	Q_ASSERT(bOk);
	if (bOk == false)
		return false;

	szTemp = lstContent.at(1);
	m_nTimeStamp = szTemp.toInt(&bOk, 0);
	// 解析出每个数据
	float fValue = 0.0f;
	for (int i = 2; i < nSize; i++)
	{
		szTemp = lstContent.at(i);
		fValue = szTemp.toFloat(&bOk);
		Q_ASSERT(bOk);
		if (bOk == false)
			return false;

		m_arrData.emplace_back(fValue);
		// 推送数据到通道中
		bOk = conf.PushSampValue(i - 2, fValue);
		Q_ASSERT(bOk);
	}
	
	return true;
}

CComtrade::CComtrade()
{

}

CComtrade::~CComtrade()
{

}

bool CComtrade::LogString(const QString & szLog, int nLevel)const
{
	extern bool LogString(const char *szLogTxt, int nLevel);

	return LogString(szLog.toLocal8Bit().constData(), nLevel);
}
/*! \fn bool CComtrade::OpenFileGroup(const QString &szFileName)
*********************************************************************************************************
** \brief CComtrade::OpenFileGroup
** \details 加载文件组
** \param szFileName  完整的文件名，是以dat为后缀的
** \return bool
** \author LiJin
** \date 2018年1月31日
** \note
********************************************************************************************************/
bool CComtrade::OpenFileGroup(const QString &szFileName)
{
	QString szCfgFileName = szFileName;
	szCfgFileName.replace(szFileName.size() - 3, 3, "cfg");

	QFileInfo fi(szFileName);
	m_szBaseName = fi.baseName();

	std::shared_ptr<CWaveCfg> pCfg = std::make_shared<CWaveCfg>();

	bool bRet = false;
	bRet = pCfg->ReadFile(szCfgFileName);

	if (bRet == false)
	{
		return false;
	}

	std::shared_ptr<CWaveDat> pDat = std::make_shared<CWaveDat>(*pCfg);
	bRet = pDat->ReadFile(szFileName);

	if (bRet == false)
	{
		return false;
	}
	else
	{
		m_pCfg = pCfg;
		m_pData = pDat;
	}

	return bRet;
}
