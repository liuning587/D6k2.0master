/*! @file client_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  client_db.cpp
文件实现功能 :  客户端节点的内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   客户端节点的内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "client_db.h"
#include "log/log.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QObject> 
#include <QString>  


typedef TRANSFORM_NONLINEAR::LINEAR_ITEM DataItem;

bool SortLFunction(DataItem& data1, DataItem& data2);
/*
{
	return data1.RawValue < data2.RawValue;
}*/


CClientDB::CClientDB( )
{
	m_bStopFlag = false;
	m_nEstimateSize = 0;
	InitFuncArrary();
}

CClientDB::~CClientDB(void)
{
	
}

bool CClientDB::Initialize(const char *pszDataPath, unsigned int nMode, int32u nOccNo)
{
	QString szLog;

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	m_nOccNo = nOccNo;

	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}

	LogMsg(szLog.toStdString().c_str(), 0);

	return true;
}

void CClientDB::Run()
{

}

void CClientDB::Shutdown()
{

}

size_t CClientDB::LoadMem(unsigned char * pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return false;
	}
	m_pMagicMem = (HEAD_MEM*)(pAddr);

	Q_ASSERT(m_pMagicMem->MagicHead1 == MAGIC_HEAD
		&& m_pMagicMem->MagicHead2 == MAGIC_HEAD);
	if (m_pMagicMem->MagicHead1 != MAGIC_HEAD || m_pMagicMem->MagicHead2 != MAGIC_HEAD)
	{
		return false;
	}
	m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	m_nAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;
	m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;
	m_nLinearCount = m_pMagicMem->LinearCount;
	m_nNonLinearCount = m_pMagicMem->NonLinearCount;
	m_nSystemVariableCount = m_pMagicMem->SystemVariableCount;
	m_nUserVariableCount = m_pMagicMem->UserVariableCount;

	pAddr += sizeof HEAD_MEM;

	size_t nSize = 0;

	nSize = CreateAinAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAinLimitAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDinAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDinLimitAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateTransFormLinear((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateTransFormNonLinear((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateSystemVariable((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateUserVariable((unsigned char*)pAddr);
	pAddr += nSize;

	m_nEstimateSize = sizeof HEAD_MEM + sizeof AIN_ALARM * m_nAinAlarmCount +
		sizeof AIN_ALARM_LIMIT* m_nAinAlarmLimitCount +
		sizeof DIN_ALARM * m_nDinAlarmCount +
		sizeof DIN_ALARM_LIMIT* m_nDinAlarmLimitCount +
		sizeof TRANSFORM_LINEAR * m_nLinearCount +
		sizeof TRANSFORM_NONLINEAR* m_nNonLinearCount +
		sizeof VARDATA * m_nSystemVariableCount +
		sizeof VARDATA * m_nUserVariableCount;

	return m_nEstimateSize;
}

bool CClientDB::GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;
	switch (nIddType)
	{
	case IDD_SYSVAR:
	{
		Q_ASSERT(m_arrGetSysVarRTDataFuncs[nFiledID]);
		if (m_arrGetSysVarRTDataFuncs[nFiledID])
		{
			m_arrGetSysVarRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_USERVAR:
	{
		Q_ASSERT(m_arrGetUserVarRTDataFuncs[nFiledID]);
		if (m_arrGetUserVarRTDataFuncs[nFiledID])
		{
			m_arrGetUserVarRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	default:
		Q_ASSERT(false);
		bRet = false;
		break;
	}
	return true;
}

bool CClientDB::PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;
	switch (nIddType)
	{
	case IDD_SYSVAR:
	{
		Q_ASSERT(m_arrSysVarSetFunctions[nFiledID]);
		if (m_arrSysVarSetFunctions[nFiledID])
		{
			m_arrSysVarSetFunctions[nFiledID](nOccNo, pData, pExt, pSrc);
		}
		break;
	}
	case IDD_USERVAR:
	{
		Q_ASSERT(m_arrUserVarSetFunctions[nFiledID]);
		if (m_arrUserVarSetFunctions[nFiledID])
		{
			m_arrUserVarSetFunctions[nFiledID](nOccNo, pData, pExt, pSrc);
		}
		break;
	}
	default:
		Q_ASSERT(false);
		bRet = false;
		break;
	}
	return true;
}

size_t CClientDB::CreateAinAlarm(unsigned char* pAddr)
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
	}
	return m_nAinAlarmCount * sizeof AIN_ALARM;
}

size_t CClientDB::CreateAinLimitAlarm(unsigned char* pAddr)
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
	}
	return m_nAinAlarmLimitCount * sizeof AIN_ALARM_LIMIT;
}

size_t CClientDB::CreateDinAlarm(unsigned char* pAddr)
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
	}
	return m_nDinAlarmCount * sizeof DIN_ALARM;
}

size_t CClientDB::CreateDinLimitAlarm(unsigned char* pAddr)
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
	}
	return m_nDinAlarmLimitCount* sizeof DIN_ALARM_LIMIT;
}

size_t CClientDB::CreateTransFormLinear(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pLinear = reinterpret_cast<TRANSFORM_LINEAR*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nLinearCount; i++)
	{
		Q_ASSERT(m_pLinear[i].OccNo != INVALID_OCCNO && m_pLinear[i].OccNo <= MAX_OCCNO);
		if (m_pLinear[i].OccNo == INVALID_OCCNO || m_pLinear[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pLinear[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrLinears.push_back(&m_pLinear[i]);
			continue;
		}
		m_arrLinears.push_back(&m_pLinear[i]);
	}
	return m_nLinearCount* sizeof TRANSFORM_LINEAR;
}

size_t CClientDB::CreateTransFormNonLinear(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pNonLinear = reinterpret_cast<TRANSFORM_NONLINEAR*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nNonLinearCount; i++)
	{
		Q_ASSERT(m_pNonLinear[i].OccNo != INVALID_OCCNO && m_pNonLinear[i].OccNo <= MAX_OCCNO);
		if (m_pNonLinear[i].OccNo == INVALID_OCCNO || m_pNonLinear[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNonLinear[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrNonLinears.push_back(&m_pNonLinear[i]);
			continue;
		}
		m_arrNonLinears.push_back(&m_pNonLinear[i]);
	}
	return m_nNonLinearCount* sizeof TRANSFORM_NONLINEAR;
}

size_t CClientDB::CreateSystemVariable(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pSystemVariable = reinterpret_cast<VARDATA*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nSystemVariableCount; i++)
	{
		Q_ASSERT(m_pSystemVariable[i].OccNo != INVALID_OCCNO && m_pSystemVariable[i].OccNo <= MAX_OCCNO);
		if (m_pSystemVariable[i].OccNo == INVALID_OCCNO || m_pSystemVariable[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] systemvariable's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pSystemVariable[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrSystemVariables.push_back(&m_pSystemVariable[i]);
			continue;
		}
		m_arrSystemVariables.push_back(&m_pSystemVariable[i]);
	}
	return m_nSystemVariableCount* sizeof VARDATA;
}

size_t CClientDB::CreateUserVariable(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pUserVariable = reinterpret_cast<VARDATA*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nUserVariableCount; i++)
	{
		Q_ASSERT(m_pUserVariable[i].OccNo != INVALID_OCCNO && m_pUserVariable[i].OccNo <= MAX_OCCNO);
		if (m_pUserVariable[i].OccNo == INVALID_OCCNO || m_pUserVariable[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] uservariable's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pSystemVariable[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrUserVariables.push_back(&m_pUserVariable[i]);
			continue;
		}
		m_arrUserVariables.push_back(&m_pUserVariable[i]);
	}
	return m_nUserVariableCount* sizeof VARDATA;
}

void CClientDB::InitFuncArrary()
{
	m_arrGetUserVarRTDataFuncs[ATT_IN_OUT] = std::bind(&CClientDB::GetUserVarScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_QUA] = std::bind(&CClientDB::GetUserVarQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_STATE] = std::bind(&CClientDB::GetUserVaState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_VALUE] = std::bind(&CClientDB::GetUserVaValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_RAW_VALUE] = std::bind(&CClientDB::GetUserValRawValEx, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_OLD_VALUE] = std::bind(&CClientDB::GetUserNegVal, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_MAXRANGE] = std::bind(&CClientDB::GetUserVarMaxRange, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MINRANGE] = std::bind(&CClientDB::GetUserVarMinRange, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_MANSET] = std::bind(&CClientDB::GetUserVaManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CClientDB::GetUserVaLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CClientDB::GetUserVaHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_HIQUA] = std::bind(&CClientDB::GetUserVaHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_LOQUA] = std::bind(&CClientDB::GetUserVaLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CClientDB::GetUserVaDesc0, this, std::placeholders::_1, std::placeholders::_2);


	m_arrGetSysVarRTDataFuncs[ATT_IN_OUT] = std::bind(&CClientDB::GetSysVarScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_QUA] = std::bind(&CClientDB::GetSysVarQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_STATE] = std::bind(&CClientDB::GetSysVaState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_VALUE] = std::bind(&CClientDB::GetSysVaValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_RAW_VALUE] = std::bind(&CClientDB::GetSysValRawValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_OLD_VALUE] = std::bind(&CClientDB::GetSysNegVal, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MAXRANGE] = std::bind(&CClientDB::GetSysVarMaxRange, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MINRANGE] = std::bind(&CClientDB::GetSysVarMinRange, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MANSET] = std::bind(&CClientDB::GetSysVaManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CClientDB::GetSysVaLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CClientDB::GetSysVaHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_HIQUA] = std::bind(&CClientDB::GetSysVaHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_LOQUA] = std::bind(&CClientDB::GetSysVaLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CClientDB::GetSysVaDesc0, this, std::placeholders::_1, std::placeholders::_2);

	m_arrUserVarSetFunctions[ATT_SETRELAYVALUE] = std::bind(&CClientDB::SetUserVarValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	m_arrSysVarSetFunctions[ATT_SETRELAYVALUE] = std::bind(&CClientDB::SetSysVarValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

bool CClientDB::GetUserVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CClientDB::GetUserVarQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_BYTE(&RetData, &pFB->Quality);

	return true;
}

bool CClientDB::GetUserNegVal(int32u nOccNO, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNO != INVALID_OCCNO && nOccNO <= MAX_OCCNO);
	if (nOccNO == INVALID_OCCNO || nOccNO > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNO <= m_nUserVariableCount);
	if (nOccNO > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNO - 1];

	S_CHAR(&RetData, &pFB->NegValue);

	return true;
}

bool CClientDB::GetUserVarMaxRange(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->RangeH);

	return true;

}

bool CClientDB::GetUserVarMinRange(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->RangeL);

	return true;
}

bool CClientDB::GetUserVaState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;

}

bool CClientDB::GetUserVaValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	RetData = pFB->Value;

	return true;
}

bool CClientDB::GetUserValRawValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	RetData = pFB->RawValue;

	return true;
}

bool CClientDB::GetUserVaManSet(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_BYTE(&RetData, &pFB->ManSet);

	return true;
}

bool CClientDB::GetUserVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->MinRaw);

	return true;
}

bool CClientDB::GetUserVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->MaxRaw);

	return true;
}

bool CClientDB::GetUserVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->HighQua);

	return true;
}

bool CClientDB::GetUserVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->LowQua);

	return true;
}

bool CClientDB::GetUserVaDesc0(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	S_INT(&RetData, &pFB->Desc0OccNo);

	return true;
}

bool CClientDB::GetSysVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CClientDB::GetSysVarQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_BYTE(&RetData, &pFB->Quality);

	return true;
}

bool CClientDB::GetSysVaState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;
}

bool CClientDB::GetSysVaValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	RetData = pFB->Value;

	return true;
}

bool CClientDB::GetSysValRawValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	RetData = pFB->RawValue;

	return true;
}

bool CClientDB::GetSysVaManSet(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_BYTE(&RetData, &pFB->ManSet);

	return true;
}

bool CClientDB::GetSysVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->MinRaw);

	return true;
}

bool CClientDB::GetSysVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->MaxRaw);

	return true;
}

bool CClientDB::GetSysVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->HighQua);

	return true;
}

bool CClientDB::GetSysVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->LowQua);

	return true;
}

bool CClientDB::GetSysVaDesc0(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_INT(&RetData, &pFB->Desc0OccNo);

	return true;
}

bool CClientDB::GetSysNegVal(int32u nOccNo, IO_VARIANT& RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_BYTE(&RetData, &pFB->NegValue);


	return true;
}

bool CClientDB::GetSysVarMaxRange(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->RangeH);

	return true;
}

bool CClientDB::GetSysVarMinRange(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	S_FLOAT(&RetData, &pFB->RangeL);

	return true;
}

bool CClientDB::SetUserVarValue(int32u nOccNo, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pUserVariable[nOccNo - 1];

	pFB->Value = *pData;

	return true;
}

bool CClientDB::SetSysVarValue(int32u nOccNo, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA * pFB = &m_pSystemVariable[nOccNo - 1];

	pFB->Value = *pData;

	return true;
}

/** @}*/
