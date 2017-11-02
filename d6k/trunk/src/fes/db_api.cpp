/*! @file db_api.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  db_api.cpp
文件实现功能 :  内存数据库服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库服务
*  @author  LiJin
*  @version 1.0
*  @date    2016.12.01
*******************************************************************************/

#include "fes.h" 
#include "db_svc.h" 
#include "mail/mail.h"

#include "../fespub/tag_att.h"
#include "../fespub/data_size.h"

#include <QDebug>
 
/*! \fn CHANNEL* CDbSvc::GetChannelByIdx(int32u nIndex)
*********************************************************************************************************
** \brief CDbSvc::GetChannelByIdx
** \details 根据索引序号获取场站信息
** \param nIndex
** \return CHANNEL*
** \author xingzhibing
** \date 2017年9月14日
** \note
********************************************************************************************************/
CHANNEL* CDbSvc::GetChannelByIndex(int32u nIndex)
{
	Q_ASSERT(nIndex < MAX_OCCNO && nIndex < m_nChannelCount);
	if (nIndex >= m_nChannelCount || nIndex >= MAX_OCCNO)
		return nullptr;

	return &m_pChannels[nIndex];
}
/*! \fn DEVICE* CDbSvc::GetDeviceByIdx(int32u nIndex)
*********************************************************************************************************
** \brief CDbSvc::GetDeviceByIdx
** \details 根据设备序号获取设备信息
** \param nIndex
** \return DEVICE*
** \author xingzhibing
** \date 2017年9月14日
** \note
********************************************************************************************************/
DEVICE* CDbSvc::GetDeviceByIndex(int32u nIndex)
{
	Q_ASSERT(nIndex < MAX_OCCNO && nIndex < m_nDeviceCount);
	if (nIndex >= m_nDeviceCount || nIndex >= MAX_OCCNO)
		return nullptr;

	return &m_pDevices[nIndex];
}
/*! \fn AIN* CDbSvc::GetAinByIndex(int32u nIdx)
********************************************************************************************************* 
** \brief CDbSvc::GetAinByIndex 
** \details  根据序号获取模拟量功能块
** \param nIdx 
** \return AIN* 
** \author LiJin 
** \date 2016年11月30日 
** \note 
********************************************************************************************************/
AIN* CDbSvc::GetAinByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nAinCount);
	if (nIdx >= m_nAinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return & m_pAins[nIdx];
}

DIN*CDbSvc::GetDinByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nDinCount);
	if (nIdx >= m_nDinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pDins[nIdx];
}


VARDATA * CDbSvc::GetUserVarByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nUserVariableCount);
	if (nIdx >= m_nUserVariableCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pUserVariable[nIdx];
}

VARDATA * CDbSvc::GetSysVarByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nSystemVariableCount);
	if (nIdx >= m_nSystemVariableCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pSystemVariable[nIdx];
}

AOUT*CDbSvc::GetAoutByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nAoutCount);
	if (nIdx >= m_nAinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pAouts[nIdx];
}

DOUT*CDbSvc::GetDoutByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nDoutCount);
	if (nIdx >= m_nAinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pDouts[nIdx];
}

NODE * CDbSvc::GetNodeByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pNodes[nOccNo - 1];
}

CHANNEL* CDbSvc::GetChannelByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nChannelCount);
	if (nOccNo > m_nChannelCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pChannels[nOccNo-1];
}

DEVICE* CDbSvc::GetDeviceByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nDeviceCount);
	if (nOccNo > m_nDeviceCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pDevices[nOccNo - 1];
}

AIN* CDbSvc::GetAinByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pAins[nOccNo - 1];
}

DIN* CDbSvc::GetDinByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pDins[nOccNo - 1];
}

AOUT* CDbSvc::GetAoutByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pAouts[nOccNo - 1];
}

DOUT* CDbSvc::GetDoutByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nDoutCount);
	if (nOccNo > m_nDoutCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pDouts[nOccNo - 1];
}

VARDATA * CDbSvc::GetUserVarByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pUserVariable[nOccNo - 1];
}

VARDATA * CDbSvc::GetSysVarByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount || nOccNo > MAX_OCCNO || nOccNo == INVALID_OCCNO)
		return nullptr;

	return &m_pSystemVariable[nOccNo - 1];
}
/*! \fn int32s CDbSvc::GetChannelMailBoxID(int32u nChannelOccNo)
*********************************************************************************************************
** \brief CDbSvc::GetChannelMailBoxID
** \details 通过通道排行号，获取该通道的邮箱ID
** \param nChannelOccNo
** \return int
** \author LiJin
** \date 2016年12月12日
** \note
********************************************************************************************************/
int32s CDbSvc::GetChannelMailBoxID(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	if (nChannelOccNo > m_nChannelCount)
	{
		return 0;
	}

	Q_ASSERT(m_pChannels[nChannelOccNo - 1].MailBoxID);
	return m_pChannels[nChannelOccNo - 1].MailBoxID;
}
/*! \fn int32u CDbSvc::GetMyHostState()const
*********************************************************************************************************
** \brief CDbSvc::GetMyHostState
** \details
** \return int32u
** \author LiJin
** \date 2016年12月1日
** \note
********************************************************************************************************/
int32u CDbSvc::GetMyHostState() const
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return STATE_UNKOWN;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (m_pMagicMem->MyNodeOccNo == INVALID_OCCNO || m_pMagicMem->MyNodeOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	return m_pNodes[m_pMagicMem->MyNodeOccNo - 1].State;
}

int8u  CDbSvc::GetMyHostScanEnable()const
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return STATE_UNKOWN;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (m_pMagicMem->MyNodeOccNo == INVALID_OCCNO || m_pMagicMem->MyNodeOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	return m_pNodes[m_pMagicMem->MyNodeOccNo - 1].ScanEnable;
}
/*! \fn bool  CDbSvc::GetDoutBlockState(DOUT & dout)
*********************************************************************************************************
** \brief CDbSvc::GetDoutBlockState
** \details 检查开出闭锁状态
** \param dout
** \return bool
** \author LiJin
** \date 2017年2月17日
** \note
********************************************************************************************************/
void  CDbSvc::CheckDoutBlockState(DOUT & dout)
{
	if (dout.BlockingProgramOccNo != INVALID_OCCNO)
	{// 执行下开出闭锁程序，发信号给内核，由内核执行

	}
	else
	{
		if (dout.BlockingSignalOccNo != INVALID_OCCNO)
		{// 只有闭锁信号，没有闭锁程序
			int8u nValue = 0;
			int8u nQuality = 0;
			bool bRet = IoGetDinValue(dout.BlockingSignalOccNo, nValue, nQuality);
			Q_ASSERT(bRet);
			if (dout.BlockSignalType == NORMAL_CLOSE)
			{// 常闭信号，值为1
				if (nValue == 1)
				{
					dout.CheckOK = 1;
				}
				else
				{
					dout.CheckOK = 0;
				}
			}
			else
			{
				if (nValue == 0)
				{
					dout.CheckOK = 1;
				}
				else
				{
					dout.CheckOK = 0;
				}
			}
		}
	}
	return;
}
/*! \fn bool  CDbSvc::IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const
*********************************************************************************************************
** \brief CDbSvc::IoGetDinValue
** \details 获取DIN的测值以及品质
** \param nOccNo
** \param nValue
** \param nQuality
** \return bool
** \author LiJin
** \date 2016年10月9日
** \note
********************************************************************************************************/
bool  CDbSvc::IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDinCount)
	{
		return false;
	}

	DIN *pFB = & m_pDins[nOccNo - 1];

	nValue = pFB->Value;
	nQuality = pFB->Quality;

	return true;
}

bool  CDbSvc::IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAinCount)
	{
		return false;
	}

	AIN *pFB = &m_pAins[nOccNo - 1];

	fValue = pFB->Value;
	nQuality = pFB->Quality;

	return true;
}
/*! \fn bool CDbSvc::FesSetDoutValue(int32u nOccNo, int8u Value, const char *pszAppTagName)
********************************************************************************************************* 
** \brief CDbSvc::FesSetDoutValue 
** \details 供应用层调用的接口函数
** \param nOccNo 遥控点排行号
** \param Value  遥控设值
** \param pszAppTagName 遥控来源
** \return bool 
** \author LiJin 
** \date 2017年9月18日 
** \note 
********************************************************************************************************/
bool CDbSvc::FesSetDoutValue(int32u nOccNo, int8u Value, const char *pszAppTagName)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nDoutCount)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nSourceAppOccNo = 0;
	int32u nSourceAppType = 0;

	if (pszAppTagName == nullptr || strlen(pszAppTagName) == 0)
	{// 如果没有注明遥控来源，则log
	
				
	}
	else
	{//! todo 获取APP的occno
	//	nSourceAppOccNo 
	}

	// 如果本机状态是非主机状态，则不执行 
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 组织下发报文给DB_SVC
	DOUT *pFB = &m_pDouts[nOccNo - 1];
	
	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
		CheckDoutBlockState(*pFB);
		return false;
	}
 
	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_DOUT;
	pSetValeEvt->Att = ATTW_DOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
	pSetValeEvt->Datatype = DT_BOOLEAN;
	pSetValeEvt->SourcAppOccNo = nSourceAppOccNo;
	pSetValeEvt->SourceAppIddType = nSourceAppType;

	S_BOOL(&pSetValeEvt->Value[0], &Value);

	//! 再次封装成邮件发送MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	Q_ASSERT(GetMailBoxID() != 0);
	dmsg.RecverID = GetMailBoxID();

	bool bRet = SendMail("FES", &dmsg, 0);
	 
	if(bRet == false)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}
	return bRet;
#if 0
	//! 判断是否关联遥信，如果有关联遥信，则发送到db_svc，由db_svc去做处理，此处需要注意超时处理
	if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
	{
		// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
		// 		dmsg.RecverID = m_nDbSvcMailBoxID;
	}
	else
	{// 否则直接下发给通道驱动
		if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
		else
		{
			dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
			if (dmsg.RecverID == 0)
			{
				szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
				LogMsg(szLog.toStdString().c_str(), 1);

				return false;
			}
		}
	}
#endif

	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}


	return true;
}

bool CDbSvc::FesSetAoutValue(int32u nOccNo, fp64 Value, const char *pszAppTagName)
{
	Q_ASSERT(false);
	return false;
}

bool  CDbSvc::FesSetNodeValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set Value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set Value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}

	if (pSetValMsg->IddType != IDD_NODE)
	{
		szLog = QObject::tr("Set Value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}





	return true;
}

bool CDbSvc::FesSetChannelValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set channel value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set channel value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_CHANNEL)
	{
		szLog = QObject::tr("Set channel value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nChannelCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set channel value [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set channel value [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	bool bRet = SetIoChannelAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);
	return bRet;
}

bool CDbSvc::FesSetDeviceValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set device value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set device value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_DEVICE)
	{
		szLog = QObject::tr("Set device value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nDeviceCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set device value [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set device value [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	bool bRet = SetDeviceAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);
	return bRet;
}
/*! \fn bool CDbSvc::FesSetAinValueImpl(DMSG *pMsg)
********************************************************************************************************* 
** \brief CDbSvc::FesSetAinValueImpl 
** \details 模拟输入量，人工强制，投退......
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2017年9月27日 
** \note 
********************************************************************************************************/
bool CDbSvc::FesSetAinValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set AIN value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set AIN value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_AIN)
	{
		szLog = QObject::tr("Set AIN value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nAinCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set AIN value [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set AIN value [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	AIN *pFB = &m_pAins[nOccNo - 1];

	//! 注意人工设值   需要先退出运行，然后进入人工设值状态
	// 如果是设值，而且测点在退出状态，人工设值状态为false
	if (pSetValMsg->Att == ATTW_VALUE && pFB->ScanEnable == SCAN_OUT && pFB->ManSet == NORMAL)
	{
		pFB->ManSet = MANSET;
	}
	else if(pSetValMsg->Att == ATTW_IN_OUT && pFB->ScanEnable == SCAN_IN && pFB->ManSet == MANSET)
	{// 如果下发的指令是投退指令，而且现在的测点是在投入状态，而且是进入人工设置态
		pFB->ManSet = NORMAL;
	}

	bool bRet = SetAinAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);

	return bRet;
}
/*! \fn bool CDbSvc::FesSetDinValueImpl(DMSG *pMsg)
********************************************************************************************************* 
** \brief CDbSvc::FesSetDinValueImpl 
** \details 开入量，人工强制，投退......
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2017年9月27日 
** \note 
********************************************************************************************************/
bool CDbSvc::FesSetDinValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set DIN value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set DIN value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_DIN)
	{
		szLog = QObject::tr("Set DIN value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nAinCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set DIN value [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set DIN value [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	DIN *pFB = &m_pDins[nOccNo - 1];

	//! 注意人工设值   需要先退出运行，然后进入人工设值状态
	// 如果是设值，而且测点在退出状态，人工设值状态为false
	if (pSetValMsg->Att == ATTW_VALUE && pFB->ScanEnable == SCAN_OUT && pFB->ManSet == NORMAL)
	{//如果测点 退出 而且 不是人工设值 时(即此时测点在中间态) 则自动将测点设置为人工设值状态。
		pFB->ManSet = MANSET;
	}
	else if (pSetValMsg->Att == ATTW_IN_OUT && pFB->ScanEnable == SCAN_IN && pFB->ManSet == MANSET)
	{// 如果下发的指令是投退指令，而且现在的测点是在投入状态，而且是进入人工设置态 ，则将人工设值标志位清0
		pFB->ManSet = NORMAL;
	}

	bool bRet = SetDinAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);

	// !todo 改变状态后，需要立即通知其他节点

	return bRet;
}
/*! \fn bool CDbSvc::FesSetDoutValueImpl(DMSG *pDMsg)
*********************************************************************************************************
** \brief CDbSvc::FesSetDoutValueImpl
** \details  执行DB_SVC下发过来的遥控命令（发送到遥控通道中）
** \param pDMsg
** \return bool
** \author LiJin
** \date 2017年9月22日
** \note
********************************************************************************************************/
bool CDbSvc::FesSetDoutValueImpl(DMSG *pDMsg)
{
	Q_ASSERT(pDMsg);
	if (pDMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pDMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set dout Value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set dout Value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_DOUT)
	{
		szLog = QObject::tr("Set dout Value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nDoutCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	DOUT *pFB = &m_pDouts[nOccNo - 1];

	if (pSetValMsg->Att == ATTW_DOUT)
	{
		if (pFB->IsSBO)
		{// SBO


		 //		return false;
		}

		if (pFB->Init != INITED)
		{
			pFB->Init = INITED;
			CheckDoutBlockState(*pFB);
			return false;
		}

		// 检查条件，如果控制闭锁没有通过，则告警
		if (pFB->CheckOK == 0)
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Because the remotecontrol's bloking condition is't ready.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);
			// 告警-提示运行人员： 遥控闭锁条件检查失败。

			return false;
		}
		//! 如果是启动程序，则 转发消息到 顺控模块（支持异机）
		if (pFB->StartProgramOccNo != INVALID_OCCNO)
		{

		}

		//! 判断是否关联遥信，如果有关联遥信，则发送到db_svc，由db_svc去做处理，此处需要注意超时处理
		if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
		{
			// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
			// 		dmsg.RecverID = m_nDbSvcMailBoxID;
		}
		else
		{// 否则直接下发给通道驱动
			if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
			{
				szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
				LogMsg(szLog.toStdString().c_str(), 1);
				return false;
			}
			else
			{
				pDMsg->RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
				if (pDMsg->RecverID == 0)
				{
					szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
					LogMsg(szLog.toStdString().c_str(), 1);
					return false;
				}
			}
		}
		bool bRet = SendMail("FES", pDMsg, 0);
		//! 如果报文发送成功，则启动告警记录
		if (bRet)
		{// 调用告警接口


		}
		else
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);
		}

		return bRet;
	}
	else
	{
		bool bRet = SetDoutAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);
		return bRet;
	}

	return false;
}

bool CDbSvc::FesSetAoutValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set aout Value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set aout Value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_AOUT)
	{
		szLog = QObject::tr("Set aout Value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nAoutCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	AOUT *pFB = &m_pAouts[nOccNo - 1];

	if (pSetValMsg->Att == ATTW_AOUT)
	{		

		if (pFB->Init != INITED)
		{
			pFB->Init = INITED;
		//	CheckDoutBlockState(*pFB);
			return false;
		}


		// 否则直接下发给通道驱动
		if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);
			return false;
		}
		else
		{
			pMsg->RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
			if (pMsg->RecverID == 0)
			{
				szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
				LogMsg(szLog.toStdString().c_str(), 1);
				return false;
			}
		}
		 
		bool bRet = SendMail("FES", pMsg, 0);
		//! 如果报文发送成功，则启动告警记录
		if (bRet)
		{// 调用告警接口


		}
		else
		{
			szLog = QObject::tr("Set aout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);
		}

		return bRet;
	}
	else
	{
		bool bRet = SetDoutAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);
		return bRet;
	}
	return false;
}
/*! \fn bool CDbSvc::FesSetUserVarValueImpl(DMSG *pMsg)
********************************************************************************************************* 
** \brief CDbSvc::FesSetUserVarValueImpl 
** \details 设置用户变量
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2017年9月28日 
** \note 
********************************************************************************************************/
bool CDbSvc::FesSetUserVarValueImpl(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	QString szLog;

	SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pMsg->Buff);
	Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
	Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

	if (pSetValMsg->MsgCode != MSG_SETVAL)
	{//LOG
		szLog = QObject::tr("Set User Var Value message is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->Len != sizeof SETVAL_MSG)
	{// LOG
		szLog = QObject::tr("Set User Var Value message's length is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	if (pSetValMsg->IddType != IDD_USERVAR)
	{
		szLog = QObject::tr("Set User Var Value message's idd_type is wrong.");
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	int32u nOccNo = pSetValMsg->Occno;

	if (nOccNo > m_nUserVariableCount || nOccNo == INVALID_OCCNO || nOccNo >= MAX_OCCNO)
	{
		szLog = QObject::tr("Set User Var [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set User Var [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set User Var [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}



	return true;
}


bool CDbSvc::FesSetSysVarValueImpl(DMSG *pMsg)
{
	return true;
}
/*! \fn bool CDbSvc::SetNodeAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
*********************************************************************************************************
** \brief CDbSvc::SetNodeAttrValue
** \details 设置节点的相关属性值
** \param nOccNo
** \param nFiledId
** \param varVal
** \return bool
** \author LiJin
** \date 2017年2月21日
** \note
********************************************************************************************************/
bool CDbSvc::SetNodeAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO && nOccNo <= m_nNodeCount) ;
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO || nOccNo > m_nNodeCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetNodeAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetNodeAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	if (nFiledId == ATT_REDSWITCH)
	{// 主从切换
		if (m_pNodes[nOccNo - 1].IsDefined == FES_YES && m_pNodes[nOccNo - 1].ScanEnable == SCAN_IN)
		{// 切换--调用切换函数

		}
	}
	else
	{
		unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pNodes[nOccNo - 1]);
		memcpy(&pMem[m_pTagAttrMgr->GetNodeAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));
		return true;
	}

	return false;
}

bool CDbSvc::SetIoChannelAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO && nOccNo <= m_nChannelCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO || nOccNo > m_nChannelCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetChannelAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetChannelAttrs()[nFiledId].DataType == varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pChannels[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetChannelAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));
	return true;
}

bool CDbSvc::SetFtChannelAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO && nOccNo <= m_nFTChannelCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO || nOccNo > m_nFTChannelCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	//!todo  实现部分还需 CTagAttMgr 追加
	Q_ASSERT(false);

	return false;
}


bool CDbSvc::SetDeviceAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_DEVICE_OCCNO && nOccNo <= m_nDeviceCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO || nOccNo > m_nDeviceCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetDeviceAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetDeviceAttrs()[nFiledId].DataType == varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pDevices[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetDeviceAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CDbSvc::SetAinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nAinCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nAinCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetAinAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetAinAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pAins[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetAinAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CDbSvc::SetDinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nDinCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nDinCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetDinAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetDinAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pDins[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetDinAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CDbSvc::SetAoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nAoutCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nAoutCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetAoutAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetAoutAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pAouts[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetAoutAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CDbSvc::SetDoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nDoutCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nDoutCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetDoutAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetDoutAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pDouts[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetDoutAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CDbSvc::SetUserVarAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO &&  nOccNo <= m_nUserVariableCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nUserVariableCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;
	
	if (m_pTagAttrMgr->GetUserVariableAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetUserVariableAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pUserVariable[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetUserVariableAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CDbSvc::SetSysVarAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO && nOccNo <= m_nSystemVariableCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nSystemVariableCount)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetSysVariableAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetSysVariableAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pUserVariable[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetUserVariableAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

#if 0
/*! \fn bool CDbSvc::FesSetDoutValueImpl(int32u nOccNo, int8u Value, int8u nSource)
********************************************************************************************************* 
** \brief CDbSvc::FesSetDoutValueImpl 
** \details  拟废弃的接口，后期将删除
** \param nOccNo 
** \param Value 
** \param nSource 
** \return bool 
** \author LiJin 
** \date 2017年9月28日 
** \note 
********************************************************************************************************/
bool CDbSvc::FesSetDoutValueImpl(int32u nOccNo, int8u Value, int8u nSource)
{
	//! 判断 OccNo是否有效
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nDoutCount)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	DOUT *pFB = &m_pDouts[nOccNo - 1];

	if (pFB->IsSBO)
	{// SBO


	 //		return false;
	}

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
		CheckDoutBlockState(*pFB);
		return false;
	}

	// 检查条件，如果控制闭锁没有通过，则告警
	if (pFB->CheckOK == 0)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Because the remotecontrol's bloking condition is't ready.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		// 告警-提示运行人员： 遥控闭锁条件检查失败。

		return false;
	}
	//! 如果是启动程序，则 转发消息到 顺控模块（支持异机）
	if (pFB->StartProgramOccNo != INVALID_OCCNO)
	{

	}
	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_DOUT;
	pSetValeEvt->Att = ATTW_DOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
//	pSetValeEvt->Source1 = nSource;
	pSetValeEvt->Datatype = DT_BOOLEAN;

	S_BOOL(&pSetValeEvt->Value[0], &Value);

	//! 再次封装成邮件发送MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	//! 判断是否关联遥信，如果有关联遥信，则发送到db_svc，由db_svc去做处理，此处需要注意超时处理
	if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
	{
// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
// 		dmsg.RecverID = m_nDbSvcMailBoxID;
	}
	else
	{// 否则直接下发给通道驱动
		if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
		else
		{
			dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
			if (dmsg.RecverID == 0)
			{
				szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
				LogMsg(szLog.toStdString().c_str(), 1);

				return false;
			}
		}
	}
	bool bRet = SendMail("FES", &dmsg, 0);
	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}

	return bRet;
}

// 写值
bool CDbSvc::FesSetAoutValueImpl(int32u nOccNo, fp64 Value, int32u nSource)
{
	//! 判断 OccNo是否有效
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nAoutCount)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	AOUT *pFB = &m_pAouts[nOccNo - 1];

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;

		return false;
	}
	//! 获取最大、最小范围，如果超过范围，则报警提示，并且不设值下去
	if (Value < pFB->RangeL || Value > pFB->RangeH)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. The value is wrong.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}

	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_AOUT;
	pSetValeEvt->Att = ATTW_AOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
//	pSetValeEvt->Source1 = nSource;
	pSetValeEvt->Datatype = DT_DOUBLE;

	S_DOUBLE(&pSetValeEvt->Value[0], &Value);

	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}
	else
	{
		dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
		if (dmsg.RecverID == 0)
		{
			szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
	}

	bool bRet = SendMail("FES", &dmsg, 0);
	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}
	return bRet;
}
#endif


/** @}*/
