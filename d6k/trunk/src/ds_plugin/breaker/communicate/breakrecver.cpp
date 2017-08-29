#include "breakrecver.h"
#include "socketconnect.h"
#include "breaker_data.h"
#include "breaker_module.h"

CBreakRecver::CBreakRecver(CSocketConnect *pSocket, QObject *parent)
	: QObject(parent)
{
	Q_ASSERT(pSocket);
	if (pSocket == nullptr)
	{
		return;
	}
	qRegisterMetaType<DBG_GET_SYS_INFO>("DBG_GET_SYS_INFO&");
	qRegisterMetaType<DBG_GET_MEAS>("DBG_GET_MEAS&");
	qRegisterMetaType<DEG_SOE_DETAIL>("DEG_SOE_DETAIL");
}

CBreakRecver::~CBreakRecver()
{
}

bool CBreakRecver::OnReceive(char *pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	qDebug() << QByteArray(pBuff,nLen).toHex();

	if (!m_dbgCache.IsEmpty())
	{
		if (m_dbgCache.GetCurByteLength() == 2)//有两个启动符
		{
			m_dbgCache.CopyData(pBuff, 2);//复制长度域
			pBuff = pBuff + 2;
			nLen = nLen - 2;;
		}

		int nNeedBytes = m_dbgCache.GetNeedByteLength();
		if (nNeedBytes <= 0)
		{
			return false;
		}

		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_dbgCache.CopyData(pBuff, nLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_dbgCache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;

			bool ISOK = AnalyseDbg(m_dbgCache.GetDataBuff(), m_dbgCache.GetDataLen());
			m_dbgCache.Clear();
			if (!ISOK)
			{
				return false;
			}

			//解析剩余的部分
			return OnReceive(pBuff, nLen);
		}
	}
	else//缓冲区为空
	{
		//在接收区寻找启动字符
		char cStartChar = DBG_HEADER0;
		char cStartChar2 = DBG_HEADER1;
		char* pNewBuff = std::find(pBuff, pBuff + nLen, cStartChar);
		char *pNewBuff2 = std::find(pBuff, pBuff + nLen, cStartChar2);

		if (pNewBuff == pBuff + nLen)
			return false;//没有找到启动字符

		if (pNewBuff2 == pBuff + nLen)
		{
			return false;
		}

		if (pNewBuff2 != pNewBuff +1)
		{
			return false;
		}

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//复制启动字符至缓冲区  两个启动字符
		m_dbgCache.CopyData(pBuff, 2);

		pBuff += 2;
		nLen -= 2;

		return OnReceive(pBuff, nLen);
	}

	return true;
}

bool CBreakRecver::AnalyseDbg(char *pBuff, int nLen)
{
	DBG_HEADER *pDbg = (DBG_HEADER*)(pBuff);

	switch (pDbg->type)
	{
	case DBG_CODE_GET_SYS_INFO:
	{
		//获取系统信息
		OnRecvSysInfo(pBuff,nLen);
		break;
	}
	case DBG_CODE_GET_MEAS:
	{
		//获取测量值信息
		OnRecvAnalogInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_DEBUG_DATA:
	{
		//获取调试信息
		OnRecvDebugInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SETTING:
	{
		//获取保护定值
		OnRecvProtectDevInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SYSCFG:
	{
		//获取系统参数
		OnRecvSystemDevInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_DICFG:
	{
		//开入量
		OnRecvDiInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_DOCFG:
	{
		//开出两
		OnRecvDOInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SOFTSTRAP:
	{
		//软压板定值
		OnRecvSoftDevInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SOE:
	{
		//soe解析
		OnRecvSoeInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_TIME:
	{
		//时间设置
		OnRecvTimeInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_SETTING:
	{
		//保护定值
		OnRecvProtectAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_SYSCFG:
	{
		//保护定值
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_DICFG:
	{
		//设置开入参数
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_DOCFG:
	{
		//设置开出参数
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_RM_CTRL:
	{
		//设置开出参数
		OnRecvRemoteContrlAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_SOFTSTRAP:
	{
		//设置软压板定值
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SIGNALRESET:
	{
		//信号复归
		OnRecvReset(pBuff, nLen);
		break;
	}
	case DBG_CODE_ClearSOE:
	{
		//清空soe
		OnRecvClearSoeAck(pBuff, nLen);
		break;
	}
	default:
		break;
		
	}

	return true;
}

void CBreakRecver::OnRecvSysInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_SYS_INFO *pSysInfo = (DBG_GET_SYS_INFO*)(pBuff);
	emit Signal_SysInfo(*pSysInfo);
}

//测量值信息
void CBreakRecver::OnRecvAnalogInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_Analog(*pMeas);
}

//调试信息
void CBreakRecver::OnRecvDebugInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_Debug(*pMeas);

}

void CBreakRecver::OnRecvProtectDevInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_ProtectDev(*pMeas);

}

void CBreakRecver::OnRecvSystemDevInfo(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_SystemDev(*pMeas);
}

void CBreakRecver::OnRecvDiInfo(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_DiInfo(*pMeas);

}

void CBreakRecver::OnRecvDOInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DEG_GET_MSG2 *pMeas = (DEG_GET_MSG2 *)(pBuff);
	emit Signal_DOInfo(*pMeas);

}

//软压板定值
void CBreakRecver::OnRecvSoftDevInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DEG_SOFT_INFO *pMeas = (DEG_SOFT_INFO *)(pBuff);
	emit Signal_SoftDev(*pMeas);
}

//soe处理
void CBreakRecver::OnRecvSoeInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DEG_SOE_DETAIL *pSoe = (DEG_SOE_DETAIL *)(pBuff);

	emit Signal_SoeDetailInfo(*pSoe);
}

//时间设置返回报文
void CBreakRecver::OnRecvTimeInfo(char * pBuff, int nLen)
{
	//写日志
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//成功
		GetBreakerModuleApi()->WriteRunLog("Breaker","时间设置成功",1);
	}
	else
	{
		//失败
		QString strValue = QStringLiteral("时间设置失败,错误码:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(),1 );
	}
	
}

void CBreakRecver::OnRecvProtectAck(char * pBuff, int nLen)
{
	//写日志
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//成功
		GetBreakerModuleApi()->WriteRunLog("Breaker", "定值设置成功", 1);
	}
	else
	{
		//失败
		QString strValue = QStringLiteral("定值设置失败,错误码:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

void CBreakRecver::OnRcvSysAck(char *pBuff, int nLen)
{
	//写日志
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//成功
		GetBreakerModuleApi()->WriteRunLog("Breaker", "定值设置成功", 1);
	}
	else
	{
		//失败
		QString strValue = QStringLiteral("定值设置失败,错误码:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

void CBreakRecver::OnRecvRemoteContrlAck(char * pBuff, int nLen)
{
	//
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//成功
		GetBreakerModuleApi()->WriteRunLog("Breaker", "操作成功", 1);
		emit Signal_RemoteControlAck();;
	}
	else
	{
		//失败
		QString strValue = QStringLiteral("操作失败,错误码:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

//信号复归
void CBreakRecver::OnRecvReset(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//成功
		GetBreakerModuleApi()->WriteRunLog("Breaker", "信号复归成功", 1);
	}
	else
	{
		//失败
		QString strValue = QStringLiteral("信号复归失败,错误码:%1").arg(pTimeAck->m_AddResult);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

void CBreakRecver::OnRecvClearSoeAck(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);

	qDebug() << QByteArray(pBuff,nLen).toHex();
	if (pTimeAck->m_Result == 0)
	{
		//成功
		GetBreakerModuleApi()->WriteRunLog("Breaker", "操作成功", 1);
		emit Signal_SeoClearSuccess();
	}
	else
	{
		//失败
		QString strValue = QStringLiteral("操作失败,错误码:%1").arg(pTimeAck->m_AddResult);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}
