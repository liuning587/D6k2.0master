/*! @file db_api.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  db_api.cpp
�ļ�ʵ�ֹ��� :  �ڴ����ݿ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ڴ����ݿ����
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
** \details ����������Ż�ȡ��վ��Ϣ
** \param nIndex
** \return CHANNEL*
** \author xingzhibing
** \date 2017��9��14��
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
** \details �����豸��Ż�ȡ�豸��Ϣ
** \param nIndex
** \return DEVICE*
** \author xingzhibing
** \date 2017��9��14��
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
** \details  ������Ż�ȡģ�������ܿ�
** \param nIdx 
** \return AIN* 
** \author LiJin 
** \date 2016��11��30�� 
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
** \details ͨ��ͨ�����кţ���ȡ��ͨ��������ID
** \param nChannelOccNo
** \return int
** \author LiJin
** \date 2016��12��12��
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
** \date 2016��12��1��
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
** \details ��鿪������״̬
** \param dout
** \return bool
** \author LiJin
** \date 2017��2��17��
** \note
********************************************************************************************************/
void  CDbSvc::CheckDoutBlockState(DOUT & dout)
{
	if (dout.BlockingProgramOccNo != INVALID_OCCNO)
	{// ִ���¿����������򣬷��źŸ��ںˣ����ں�ִ��

	}
	else
	{
		if (dout.BlockingSignalOccNo != INVALID_OCCNO)
		{// ֻ�б����źţ�û�б�������
			int8u nValue = 0;
			int8u nQuality = 0;
			bool bRet = IoGetDinValue(dout.BlockingSignalOccNo, nValue, nQuality);
			Q_ASSERT(bRet);
			if (dout.BlockSignalType == NORMAL_CLOSE)
			{// �����źţ�ֵΪ1
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
** \details ��ȡDIN�Ĳ�ֵ�Լ�Ʒ��
** \param nOccNo
** \param nValue
** \param nQuality
** \return bool
** \author LiJin
** \date 2016��10��9��
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
** \details ��Ӧ�ò���õĽӿں���
** \param nOccNo ң�ص����к�
** \param Value  ң����ֵ
** \param pszAppTagName ң����Դ
** \return bool 
** \author LiJin 
** \date 2017��9��18�� 
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
	{// ���û��ע��ң����Դ����log
	
				
	}
	else
	{//! todo ��ȡAPP��occno
	//	nSourceAppOccNo 
	}

	// �������״̬�Ƿ�����״̬����ִ�� 
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// ������ڵ��˳����У���ִ��
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// ��֯�·����ĸ�DB_SVC
	DOUT *pFB = &m_pDouts[nOccNo - 1];
	
	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
		CheckDoutBlockState(*pFB);
		return false;
	}
 
	//! ��֯��ֵ���ģ������͸� db_svc
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

	//! �ٴη�װ���ʼ�����MSG_EVT_SETVAL
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
	//! �ж��Ƿ����ң�ţ�����й���ң�ţ����͵�db_svc����db_svcȥ�������˴���Ҫע�ⳬʱ����
	if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
	{
		// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
		// 		dmsg.RecverID = m_nDbSvcMailBoxID;
	}
	else
	{// ����ֱ���·���ͨ������
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

	//! ������ķ��ͳɹ����������澯��¼
	if (bRet)
	{// ���ø澯�ӿ�


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
	// �������״̬�Ƿ�����״̬����ִ��
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
	// �������״̬�Ƿ�����״̬����ִ��
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
** \details ģ�����������˹�ǿ�ƣ�Ͷ��......
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2017��9��27�� 
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
	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set AIN value [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	AIN *pFB = &m_pAins[nOccNo - 1];

	//! ע���˹���ֵ   ��Ҫ���˳����У�Ȼ������˹���ֵ״̬
	// �������ֵ�����Ҳ�����˳�״̬���˹���ֵ״̬Ϊfalse
	if (pSetValMsg->Att == ATTW_VALUE && pFB->ScanEnable == SCAN_OUT && pFB->ManSet == NORMAL)
	{
		pFB->ManSet = MANSET;
	}
	else if(pSetValMsg->Att == ATTW_IN_OUT && pFB->ScanEnable == SCAN_IN && pFB->ManSet == MANSET)
	{// ����·���ָ����Ͷ��ָ��������ڵĲ������Ͷ��״̬�������ǽ����˹�����̬
		pFB->ManSet = NORMAL;
	}

	bool bRet = SetAinAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);

	return bRet;
}
/*! \fn bool CDbSvc::FesSetDinValueImpl(DMSG *pMsg)
********************************************************************************************************* 
** \brief CDbSvc::FesSetDinValueImpl 
** \details ���������˹�ǿ�ƣ�Ͷ��......
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2017��9��27�� 
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
	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set DIN value [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	DIN *pFB = &m_pDins[nOccNo - 1];

	//! ע���˹���ֵ   ��Ҫ���˳����У�Ȼ������˹���ֵ״̬
	// �������ֵ�����Ҳ�����˳�״̬���˹���ֵ״̬Ϊfalse
	if (pSetValMsg->Att == ATTW_VALUE && pFB->ScanEnable == SCAN_OUT && pFB->ManSet == NORMAL)
	{//������ �˳� ���� �����˹���ֵ ʱ(����ʱ������м�̬) ���Զ����������Ϊ�˹���ֵ״̬��
		pFB->ManSet = MANSET;
	}
	else if (pSetValMsg->Att == ATTW_IN_OUT && pFB->ScanEnable == SCAN_IN && pFB->ManSet == MANSET)
	{// ����·���ָ����Ͷ��ָ��������ڵĲ������Ͷ��״̬�������ǽ����˹�����̬ �����˹���ֵ��־λ��0
		pFB->ManSet = NORMAL;
	}

	bool bRet = SetDinAttrValue(nOccNo - 1, pSetValMsg->IddType, pSetValMsg->Value[0]);

	// !todo �ı�״̬����Ҫ����֪ͨ�����ڵ�

	return bRet;
}
/*! \fn bool CDbSvc::FesSetDoutValueImpl(DMSG *pDMsg)
*********************************************************************************************************
** \brief CDbSvc::FesSetDoutValueImpl
** \details  ִ��DB_SVC�·�������ң��������͵�ң��ͨ���У�
** \param pDMsg
** \return bool
** \author LiJin
** \date 2017��9��22��
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
	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// ������ڵ��˳����У���ִ��
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

		// ���������������Ʊ���û��ͨ������澯
		if (pFB->CheckOK == 0)
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Because the remotecontrol's bloking condition is't ready.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);
			// �澯-��ʾ������Ա�� ң�ر����������ʧ�ܡ�

			return false;
		}
		//! ��������������� ת����Ϣ�� ˳��ģ�飨֧�������
		if (pFB->StartProgramOccNo != INVALID_OCCNO)
		{

		}

		//! �ж��Ƿ����ң�ţ�����й���ң�ţ����͵�db_svc����db_svcȥ�������˴���Ҫע�ⳬʱ����
		if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
		{
			// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
			// 		dmsg.RecverID = m_nDbSvcMailBoxID;
		}
		else
		{// ����ֱ���·���ͨ������
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
		//! ������ķ��ͳɹ����������澯��¼
		if (bRet)
		{// ���ø澯�ӿ�


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
	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// ������ڵ��˳����У���ִ��
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


		// ����ֱ���·���ͨ������
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
		//! ������ķ��ͳɹ����������澯��¼
		if (bRet)
		{// ���ø澯�ӿ�


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
** \details �����û�����
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2017��9��28�� 
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
	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set User Var [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// ������ڵ��˳����У���ִ��
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
** \details ���ýڵ���������ֵ
** \param nOccNo
** \param nFiledId
** \param varVal
** \return bool
** \author LiJin
** \date 2017��2��21��
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
	{// �����л�
		if (m_pNodes[nOccNo - 1].IsDefined == FES_YES && m_pNodes[nOccNo - 1].ScanEnable == SCAN_IN)
		{// �л�--�����л�����

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

	//!todo  ʵ�ֲ��ֻ��� CTagAttMgr ׷��
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
** \details  ������Ľӿڣ����ڽ�ɾ��
** \param nOccNo 
** \param Value 
** \param nSource 
** \return bool 
** \author LiJin 
** \date 2017��9��28�� 
** \note 
********************************************************************************************************/
bool CDbSvc::FesSetDoutValueImpl(int32u nOccNo, int8u Value, int8u nSource)
{
	//! �ж� OccNo�Ƿ���Ч
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

	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// ������ڵ��˳����У���ִ��
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

	// ���������������Ʊ���û��ͨ������澯
	if (pFB->CheckOK == 0)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Because the remotecontrol's bloking condition is't ready.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		// �澯-��ʾ������Ա�� ң�ر����������ʧ�ܡ�

		return false;
	}
	//! ��������������� ת����Ϣ�� ˳��ģ�飨֧�������
	if (pFB->StartProgramOccNo != INVALID_OCCNO)
	{

	}
	//! ��֯��ֵ���ģ������͸� db_svc
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

	//! �ٴη�װ���ʼ�����MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	//! �ж��Ƿ����ң�ţ�����й���ң�ţ����͵�db_svc����db_svcȥ�������˴���Ҫע�ⳬʱ����
	if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
	{
// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
// 		dmsg.RecverID = m_nDbSvcMailBoxID;
	}
	else
	{// ����ֱ���·���ͨ������
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
	//! ������ķ��ͳɹ����������澯��¼
	if (bRet)
	{// ���ø澯�ӿ�


	}
	else
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}

	return bRet;
}

// дֵ
bool CDbSvc::FesSetAoutValueImpl(int32u nOccNo, fp64 Value, int32u nSource)
{
	//! �ж� OccNo�Ƿ���Ч
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

	// �������״̬�Ƿ�����״̬����ִ��
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// ������ڵ��˳����У���ִ��
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
	//! ��ȡ�����С��Χ�����������Χ���򱨾���ʾ�����Ҳ���ֵ��ȥ
	if (Value < pFB->RangeL || Value > pFB->RangeH)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. The value is wrong.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}

	//! ��֯��ֵ���ģ������͸� db_svc
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
	//! ������ķ��ͳɹ����������澯��¼
	if (bRet)
	{// ���ø澯�ӿ�


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
