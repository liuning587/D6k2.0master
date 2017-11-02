/*! @file message.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  message.cpp
�ļ�ʵ�ֹ��� :  ��Ϣ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��Ϣ����
*  @author  LiJin
*  @version 1.0
*  @date    2017.09.13
*******************************************************************************/
#include "message_executer.h"

#include "log/log.h"
#include "mail/mail.h"

#include "db_svc.h"

#include <QObject> 
#include <QString>  
 

CMessageExecutor::CMessageExecutor()
{
	m_pDMsg = nullptr;
}

CMessageExecutor::CMessageExecutor(DMSG *pMsg):m_pDMsg(pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;


}

bool CMessageExecutor::Run(const MsgFunctor & fnHandler)
{
	Q_ASSERT(m_pDMsg);
	if (m_pDMsg == nullptr)
		return false;

	bool bRet = false;
	//! ���� ��ֵ�����ģ�ת��
	bRet = fnHandler(m_pDMsg);

	return true;
}


bool CMessageExecutor::Run(CDbSvc *pDbSvc,DMSG *pDMsg)
{
	Q_ASSERT(pDMsg);
	if (pDMsg == nullptr)
		return false;

	// ��������
	if (pDMsg->Type == MSG_EVT_SETVAL)
	{// �������ֵ����
		SETVAL_MSG *pSetValMsg = reinterpret_cast<SETVAL_MSG*>(pDMsg->Buff);
		Q_ASSERT(pSetValMsg->MsgCode == MSG_SETVAL);
		Q_ASSERT(pSetValMsg->Len == sizeof SETVAL_MSG);

		if (pSetValMsg->MsgCode != MSG_SETVAL)
		{//LOG

			return false;
		}

		if(pSetValMsg->Len != sizeof SETVAL_MSG)
		{// LOG

			return false;
		}

		bool bRet = false;

		switch (pSetValMsg->IddType)
		{
			case  IDD_NODE:
				bRet = pDbSvc->FesSetNodeValueImpl(pDMsg);
				break;
			case  IDD_CHANNEL:
				bRet = pDbSvc->FesSetChannelValueImpl(pDMsg);
				break;
			case  IDD_DEVICE:
				bRet = pDbSvc->FesSetDeviceValueImpl(pDMsg);
				break;
			case  IDD_AIN:
				bRet = pDbSvc->FesSetAinValueImpl(pDMsg);
				break;
			case  IDD_DIN:
				bRet = pDbSvc->FesSetDinValueImpl(pDMsg);
				break;
			case  IDD_AOUT:
				bRet = pDbSvc->FesSetAoutValueImpl(pDMsg);
				break;
			case  IDD_DOUT:
				bRet = pDbSvc->FesSetDoutValueImpl(pDMsg);
				break;
			case  IDD_USERVAR:
				bRet = pDbSvc->FesSetNodeValueImpl(pDMsg);
				break;
			case  IDD_SYSVAR:
				bRet = pDbSvc->FesSetNodeValueImpl(pDMsg);
				break;
			default:
				Q_ASSERT(false);
				break;
		}

		 

	}


	return true;
}

/** @}*/
