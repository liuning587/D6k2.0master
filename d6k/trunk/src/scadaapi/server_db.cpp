/*! @file server_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  server_db.cpp
文件实现功能 :  服务器节点的内存数据库，主要是系统变量以及用户变量的管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   服务器节点的内存数据库
*  @author  xingzhibing
*  @version 1.0
*  @date    2017.06.07
*******************************************************************************/
#include "server_db.h"
#include "fesapi/fesdatadef.h"
#include "log/log.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QObject> 
#include <QString>  


template<typename T>
bool SortFuction(const T& T1, const T& T2)
{
	return T1->OccNo < T2->OccNo;
}

typedef TRANSFORM_NONLINEAR::LINEAR_ITEM DataItem;

bool SortLFunction(DataItem& data1, DataItem& data2)
{
	return data1.RawValue < data2.RawValue;
}



CServerDB::CServerDB()
{
	m_bStopFlag = false;
	m_nEstimateSize = 0;
	InitFuncArrary();
}

CServerDB::CServerDB(CScadaApi *pScada) :CScadaDB(pScada)
{
	m_bStopFlag = false;
	m_nEstimateSize = 0;
	InitFuncArrary();
}

CServerDB::~CServerDB(void)
{

}

bool CServerDB::Initialize(const char *pszDataPath, unsigned int nMode, int32u nOccNo)
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

void CServerDB::Run()
{

}

void CServerDB::Shutdown()
{

}

size_t CServerDB::LoadMem(unsigned char * pAddr)
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

bool CServerDB::GetUserVarByOccNo(int32u nOccNo, VARDATA **pUserVar)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}
	Q_ASSERT(&m_pUserVariable[nOccNo]);

	*pUserVar = &m_pUserVariable[nOccNo];

	return true;
}

bool CServerDB::GetSysVarByOccNo(int32u nOccNo, VARDATA** pSysVar)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}
	Q_ASSERT(&m_pSystemVariable[nOccNo]);

	*pSysVar = &m_pSystemVariable[nOccNo];

	return true;
}

bool CServerDB::GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData)
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

bool CServerDB::PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, const char * pszAppTagName, void *pExt)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	//! fixed by LiJin 2017.8.9
	int32u nMyState = GetMyHostState();

	if (nMyState != STATE_MAIN)
	{// 如果本机不是主机，则不写入数据
		QString szLog;
		szLog = QString(QObject::tr("Node [%1] is't working on master! PutRtData [ %2 ].[ %3 ].[ %4 ] failed "))
			.arg(m_nMyNodeOccNo).arg(nIddType).arg(nOccNo).arg(nFiledID);
		LogMsg(szLog.toStdString().c_str(), 0);

		return false;
	}

	bool bRet = false;
	int32u nSrcAppOccNo = 0;
	int32u nTempIddType = 0 , nTempFiledID = 0;
	bRet =  GetOccNoByTagName(pszAppTagName, nTempIddType, nSrcAppOccNo, nTempFiledID);
	Q_ASSERT(bRet);
	if (bRet == false)
	{// log

	}
	Q_ASSERT(nTempIddType == IDD_SAPP && nTempFiledID == 0);

	switch (nIddType)
	{
		case IDD_SYSVAR:
		{
			Q_ASSERT(m_arrSysVarSetFunctions[nFiledID]);
			if (m_arrSysVarSetFunctions[nFiledID])
			{
				m_arrSysVarSetFunctions[nFiledID](nOccNo, pData, nSrcAppOccNo, pExt);
			}
			break;
		}
		case IDD_USERVAR:
		{
			Q_ASSERT(m_arrUserVarSetFunctions[nFiledID]);
			if (m_arrUserVarSetFunctions[nFiledID])
			{
				m_arrUserVarSetFunctions[nFiledID](nOccNo, pData, nSrcAppOccNo, pExt);
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

void CServerDB::InitFuncArrary()
{
	m_arrGetUserVarRTDataFuncs[ATT_IN_OUT] = std::bind(&CServerDB::GetUserVarScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_QUA] = std::bind(&CServerDB::GetUserVarQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_STATE] = std::bind(&CServerDB::GetUserVaState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_VALUE] = std::bind(&CServerDB::GetUserVaValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_RAW_VALUE] = std::bind(&CServerDB::GetUserValRawValEx, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_OLD_VALUE] = std::bind(&CServerDB::GetUserNegVal, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_MAXRANGE] = std::bind(&CServerDB::GetUserVarMaxRange, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MINRANGE] = std::bind(&CServerDB::GetUserVarMinRange, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_MANSET] = std::bind(&CServerDB::GetUserVaManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CServerDB::GetUserVaLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CServerDB::GetUserVaHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_HIQUA] = std::bind(&CServerDB::GetUserVaHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_LOQUA] = std::bind(&CServerDB::GetUserVaLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CServerDB::GetUserVaDesc0, this, std::placeholders::_1, std::placeholders::_2);


	m_arrGetSysVarRTDataFuncs[ATT_IN_OUT] = std::bind(&CServerDB::GetSysVarScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_QUA] = std::bind(&CServerDB::GetSysVarQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_STATE] = std::bind(&CServerDB::GetSysVaState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_VALUE] = std::bind(&CServerDB::GetSysVaValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_RAW_VALUE] = std::bind(&CServerDB::GetSysValRawValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_OLD_VALUE] = std::bind(&CServerDB::GetSysNegVal, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MAXRANGE] = std::bind(&CServerDB::GetSysVarMaxRange, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MINRANGE] = std::bind(&CServerDB::GetSysVarMinRange, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MANSET] = std::bind(&CServerDB::GetSysVaManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CServerDB::GetSysVaLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CServerDB::GetSysVaHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_HIQUA] = std::bind(&CServerDB::GetSysVaHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_LOQUA] = std::bind(&CServerDB::GetSysVaLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetSysVarRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CServerDB::GetSysVaDesc0, this, std::placeholders::_1, std::placeholders::_2);

	m_arrUserVarSetFunctions[ATT_SETRELAYVALUE] = std::bind(&CServerDB::SetUserVarValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	m_arrSysVarSetFunctions[ATT_SETRELAYVALUE] = std::bind(&CServerDB::SetSysVarValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

bool CServerDB::GetUserVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVarQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserNegVal(int32u nOccNO, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVarMaxRange(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVarMinRange(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaValEx(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserValRawValEx(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaManSet(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetUserVaDesc0(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVarQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaValEx(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysValRawValEx(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaManSet(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVaDesc0(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysNegVal(int32u nOccNo, IO_VARIANT& RetData) const
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

bool CServerDB::GetSysVarMaxRange(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::GetSysVarMinRange(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CServerDB::SetUserVarValue(int32u nOccNo, IO_VARIANT *pData, int32u nSrcAppOccNo, void *pExt)
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

bool CServerDB::SetSysVarValue(int32u nOccNo, IO_VARIANT *pData, int32u nSrcAppOccNo, void *pExt)
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

size_t CServerDB::CreateAinAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateAinLimitAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateDinAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateDinLimitAlarm(unsigned char* pAddr)
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
	return m_nDinAlarmLimitCount * sizeof DIN_ALARM_LIMIT;
}

size_t CServerDB::CreateTransFormLinear(unsigned char* pAddr)
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
	return m_nLinearCount * sizeof TRANSFORM_LINEAR;
}

size_t CServerDB::CreateTransFormNonLinear(unsigned char* pAddr)
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
	return m_nNonLinearCount * sizeof TRANSFORM_NONLINEAR;
}

size_t CServerDB::CreateSystemVariable(unsigned char* pAddr)
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
	return m_nSystemVariableCount * sizeof VARDATA;
}

size_t CServerDB::CreateUserVariable(unsigned char* pAddr)
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
	return m_nUserVariableCount * sizeof VARDATA;
}

/** @}*/
