#include "fesdb.h"
#include "fesapi/fes_magicmem.h"
#include "log/log.h"
#include <qglobal.h>



bool CFesDBR::LoadFesMemDB(const char* pAddr,int nSize1)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return false;
	}

	m_pMagicMem = (HEAD_MEM*)(pAddr);

	Q_ASSERT(m_pMagicMem->MagicHead1==MAGIC_HEAD && m_pMagicMem->MagicHead2==MAGIC_HEAD);
	if (m_pMagicMem->MagicHead1!=MAGIC_HEAD || m_pMagicMem->MagicHead2!=MAGIC_HEAD)
	{
		return false;
	}

	//m_szProjName = m_pMagicMem->Config;

	m_nChannelCount = m_pMagicMem->ChannelCount;
	m_nDeviceCount = m_pMagicMem->DeviceCount;
	m_nAinCount = m_pMagicMem->AinCount;
	m_nDinCount = m_pMagicMem->DinCount;
	m_nAoutCount = m_pMagicMem->AoutCount;
	m_nDoutCount = m_pMagicMem->DoutCount;
	m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	m_nAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;
	m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;
	m_nUserVarCount = m_pMagicMem->UserVariableCount;
	m_nSysVarCount = m_pMagicMem->SystemVariableCount;


	pAddr += sizeof HEAD_MEM;

	size_t nSize = 0;
	nSize = CreateChannel((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateDevice((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAin((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDin((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateAout((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateDout((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateAinAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAinLimitAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDinAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDinLimitAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateSysVars((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateUserVars((unsigned char*)pAddr);
	pAddr += nSize;

	

	//数据读完之后，形成关系表
	for (auto i:m_arrChannels)
	{
		if (i)
		{
			CScdChannel* pChannel = new CScdChannel(i,i->OccNo);
			m_pMgr->AddChannel(pChannel);
		}
	}

	for (auto i:m_arrDevices)
	{
		if (i)
		{
			CScdChannel * pChannel = m_pMgr->GetChannelByOccNo(i->ChannelOccNo);
			CScdDevice* pDev = new CScdDevice(i,i->OccNo);
			pChannel->AddDev(pDev, i->OccNo);
		}
	}

	for ( auto i:m_arrAins)
	{
		if (i)
		{
			CScdDevice* pDev = m_pMgr->GetDevice(i->ChannelOccNo,i->DeviceOccNo);
			if (pDev)
			{
				pDev->AddAin(i);
			}
		}
	}

	for ( auto i:m_arrDins)
	{
		if (i)
		{
			CScdDevice* pDev = m_pMgr->GetDevice(i->ChannelOccNo, i->DeviceOccNo);
			if (pDev)
			{
				pDev->AddDin(i);
			}
		}
	}

	for (auto i:m_arrAouts)
	{
		if (i)
		{
			CScdDevice* pDev = m_pMgr->GetDevice(i->ChannelOccNo, i->DeviceOccNo);
			if (pDev)
			{
				pDev->AddAout(i);
			}
		}
	}

	for (auto i : m_arrDouts)
	{
		if (i)
		{
			CScdDevice* pDev = m_pMgr->GetDevice(i->ChannelOccNo, i->DeviceOccNo);
			if (pDev)
			{
				pDev->AddDout(i);
			}
		}
	}

	return true;
}

size_t CFesDBR::CreateChannel(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pChannels = reinterpret_cast<CHANNEL*>(pAddr);
	
	QString szLog;

	for (int i = 0; i < m_nChannelCount;++i)
	{
		Q_ASSERT(m_pChannels[i].OccNo != INVALID_OCCNO && m_pChannels[i].OccNo<MAX_CHANNEL_OCCNO);
		if (m_pChannels[i].OccNo == INVALID_OCCNO || m_pChannels[i].OccNo>MAX_CHANNEL_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] channel's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pChannels[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrChannels.push_back(&m_pChannels[i]);
		m_mapChannels.insert(std::make_pair(m_pChannels[i].OccNo,&m_pChannels[i]));
	}

	return m_nChannelCount* sizeof CHANNEL;
}

size_t CFesDBR::CreateDevice(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDevices = reinterpret_cast<DEVICE*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nDeviceCount; ++i)
	{
		Q_ASSERT(m_pDevices[i].OccNo != INVALID_OCCNO && m_pDevices[i].OccNo < MAX_DEVICE_OCCNO);
		if (m_pDevices[i].OccNo == INVALID_OCCNO || m_pDevices[i].OccNo > MAX_DEVICE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] device's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDevices[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrDevices.push_back(&m_pDevices[i]);
		m_mapDevices.insert(std::make_pair(m_pDevices[i].OccNo, &m_pDevices[i]));
	}
	return m_nDeviceCount* sizeof DEVICE;
}

size_t CFesDBR::CreateAin(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAins = reinterpret_cast<AIN*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nAinCount; ++i)
	{
		Q_ASSERT(m_pAins[i].OccNo != INVALID_OCCNO && m_pAins[i].OccNo < MAX_OCCNO);
		if (m_pAins[i].OccNo == INVALID_OCCNO || m_pAins[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAins[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrAins.push_back(&m_pAins[i]);
		m_mapAins.insert(std::make_pair(m_pAins[i].OccNo, &m_pAins[i]));
	}
	return m_nAinCount* sizeof AIN;
}

size_t CFesDBR::CreateDin(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDins = reinterpret_cast<DIN*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nDinCount; ++i)
	{
		Q_ASSERT(m_pDins[i].OccNo != INVALID_OCCNO && m_pDins[i].OccNo < MAX_OCCNO);
		if (m_pDins[i].OccNo == INVALID_OCCNO || m_pDins[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDins[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrDins.push_back(&m_pDins[i]);
		m_mapDins.insert(std::make_pair(m_pDins[i].OccNo, &m_pDins[i]));
	}
	return m_nDinCount* sizeof DIN;
}

size_t CFesDBR::CreateAout(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAouts = reinterpret_cast<AOUT*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nAoutCount; ++i)
	{
		Q_ASSERT(m_pAouts[i].OccNo != INVALID_OCCNO && m_pAouts[i].OccNo < MAX_OCCNO);
		if (m_pAouts[i].OccNo == INVALID_OCCNO || m_pAouts[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] aout's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrAouts.push_back(&m_pAouts[i]);
		m_mapAouts.insert(std::make_pair(m_pAouts[i].OccNo, &m_pAouts[i]));
	}
	return m_nAoutCount* sizeof AOUT;
}

size_t CFesDBR::CreateDout(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDouts = reinterpret_cast<DOUT*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nDoutCount; ++i)
	{
		Q_ASSERT(m_pDouts[i].OccNo != INVALID_OCCNO && m_pDouts[i].OccNo < MAX_OCCNO);
		if (m_pDouts[i].OccNo == INVALID_OCCNO || m_pDouts[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] aout's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrDouts.push_back(&m_pDouts[i]);
		m_mapDouts.insert(std::make_pair(m_pDouts[i].OccNo, &m_pDouts[i]));
	}
	return m_nDoutCount* sizeof DOUT;
}


size_t CFesDBR::CreateUserVars(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pUserVars = reinterpret_cast<VARDATA*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nUserVarCount; ++i)
	{
		Q_ASSERT(m_pUserVars[i].OccNo != INVALID_OCCNO && m_pUserVars[i].OccNo < MAX_OCCNO);
		if (m_pUserVars[i].OccNo == INVALID_OCCNO || m_pUserVars[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] uservar's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pUserVars[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrUserVars.push_back(&m_pUserVars[i]);
		m_mapUserVars.insert(std::make_pair(m_pUserVars[i].OccNo, &m_pUserVars[i]));
	}
	return m_nUserVarCount* sizeof VARDATA;
}

size_t CFesDBR::CreateSysVars(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pSysVars = reinterpret_cast<VARDATA*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nSysVarCount; ++i)
	{
		Q_ASSERT(m_pSysVars[i].OccNo != INVALID_OCCNO && m_pSysVars[i].OccNo < MAX_OCCNO);
		if (m_pSysVars[i].OccNo == INVALID_OCCNO || m_pSysVars[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] sysvar's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pSysVars[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrSysVars.push_back(&m_pSysVars[i]);
		m_mapSysVars.insert(std::make_pair(m_pSysVars[i].OccNo, &m_pSysVars[i]));
	}
	return m_nSysVarCount* sizeof VARDATA;
}

void CFesDBR::LogMsg(const char * pszText, int nLevel)
{
	::LogMsg("scdview_fesdb", pszText, nLevel,nullptr);
}

CHANNEL* CFesDBR::GetChannelByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo >MAX_CHANNEL_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapChannels.find(nOccNo);
	if (it==m_mapChannels.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

DEVICE* CFesDBR::GetDeviceByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_DEVICE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_DEVICE_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapDevices.find(nOccNo);
	if (it == m_mapDevices.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

AIN* CFesDBR::GetAinByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapAins.find(nOccNo);
	if (it == m_mapAins.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

DIN* CFesDBR::GetDinByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapDins.find(nOccNo);
	if (it == m_mapDins.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

AOUT * CFesDBR::GetAoutByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapAouts.find(nOccNo);
	if (it == m_mapAouts.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

DOUT* CFesDBR::GetDoutByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapDouts.find(nOccNo);
	if (it == m_mapDouts.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

size_t CFesDBR::CreateAinAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAinAlarm = reinterpret_cast<AIN_ALARM*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nAinAlarmCount; i++)
	{
		Q_ASSERT(m_pAinAlarm[i].OccNo != INVALID_OCCNO && m_pAinAlarm[i].OccNo <= MAX_OCCNO);
		if (m_pAinAlarm[i].OccNo == INVALID_OCCNO || m_pAinAlarm[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain_alarm's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAinAlarm[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrAinAlarms.push_back(&m_pAinAlarm[i]);
			continue;
		}
		m_arrAinAlarms.push_back(&m_pAinAlarm[i]);
		m_mapAinAlarms.insert(std::make_pair(m_pAinAlarm[i].OccNo ,&m_pAinAlarm[i]));
	}
	return m_nAinAlarmCount * sizeof AIN_ALARM;
}

size_t CFesDBR::CreateAinLimitAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAinAlarmLimit = reinterpret_cast<AIN_ALARM_LIMIT*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nAinAlarmLimitCount; i++)
	{
		Q_ASSERT(m_pAinAlarmLimit[i].OccNo != INVALID_OCCNO && m_pAinAlarmLimit[i].OccNo <= MAX_OCCNO);
		if (m_pAinAlarmLimit[i].OccNo == INVALID_OCCNO || m_pAinAlarmLimit[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAinAlarmLimit[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrAinAlarmLimits.push_back(&m_pAinAlarmLimit[i]);
			continue;
		}
		m_arrAinAlarmLimits.push_back(&m_pAinAlarmLimit[i]);
		m_mapAinAlarmLimits.insert(std::make_pair(m_pAinAlarmLimit[i].OccNo, &m_pAinAlarmLimit[i]));
	}
	return m_nAinAlarmLimitCount * sizeof AIN_ALARM_LIMIT;
}

size_t CFesDBR::CreateDinAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDinAlarm = reinterpret_cast<DIN_ALARM*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nDinAlarmCount; i++)
	{
		Q_ASSERT(m_pDinAlarm[i].OccNo != INVALID_OCCNO && m_pDinAlarm[i].OccNo <= MAX_OCCNO);
		if (m_pDinAlarm[i].OccNo == INVALID_OCCNO || m_pDinAlarm[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDinAlarm[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrDinAlarms.push_back(&m_pDinAlarm[i]);
			continue;
		}
		m_arrDinAlarms.push_back(&m_pDinAlarm[i]);
		m_mapDinAlarms.insert(std::make_pair(m_pDinAlarm[i].OccNo, &m_pDinAlarm[i]));
	}
	return m_nDinAlarmCount * sizeof DIN_ALARM;
}

size_t CFesDBR::CreateDinLimitAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDinAlarmLimit = reinterpret_cast<DIN_ALARM_LIMIT*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nDinAlarmLimitCount; i++)
	{
		Q_ASSERT(m_pDinAlarmLimit[i].OccNo != INVALID_OCCNO && m_pDinAlarmLimit[i].OccNo <= MAX_OCCNO);
		if (m_pDinAlarmLimit[i].OccNo == INVALID_OCCNO || m_pDinAlarmLimit[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDinAlarmLimit[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrDinAlarmLimits.push_back(&m_pDinAlarmLimit[i]);
			continue;
		}
		m_arrDinAlarmLimits.push_back(&m_pDinAlarmLimit[i]);
		m_mapDinAlarmLimits.insert(std::make_pair(m_pDinAlarmLimit[i].OccNo, &m_pDinAlarmLimit[i]));
	}
	return m_nDinAlarmLimitCount* sizeof DIN_ALARM_LIMIT;
}

VARDATA * CFesDBR::GetUserVarByIndex(int nIndex)
{
	if (nIndex >= m_nUserVarCount || nIndex < 0)
	{
		return nullptr;
	}
	return &m_pUserVars[nIndex];
}

VARDATA * CFesDBR::GetSysVarByIndex(int nIndex)
{
	if (nIndex >= m_nSysVarCount || nIndex < 0)
	{
		return nullptr;
	}
	return &m_pSysVars[nIndex];
}

