/*! @file message.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  message.cpp
文件实现功能 :  消息定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   消息定义
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
	//! 解析 设值类信文，转发
	bRet = fnHandler(m_pDMsg);

	return true;
}


bool CMessageExecutor::Run(CDbSvc *pDbSvc,DMSG *pDMsg)
{
	Q_ASSERT(pDMsg);
	if (pDMsg == nullptr)
		return false;

	// 解析报文
	if (pDMsg->Type == MSG_EVT_SETVAL)
	{// 如果是设值报文
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
