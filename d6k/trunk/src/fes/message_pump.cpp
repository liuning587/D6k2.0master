

#include "message_pump.h" 
#include "message_executer.h"

#include "mail/mail.h"

CMessagePump::CMessagePump(void)
{

}

CMessagePump::CMessagePump(int nRecvMailBoxID):m_nDBSvcMailBoxID(nRecvMailBoxID)
{
	Q_ASSERT(nRecvMailBoxID != 0);

}

CMessagePump::~CMessagePump(void)
{

}

/*! \fn bool CMessagePump::Run()
********************************************************************************************************* 
** \brief CMessagePump::Run 
** \details 从DB_SVC接收消息，然后转发到executer上
** \return bool 
** \author LiJin 
** \date 2017年9月20日 
** \note 
********************************************************************************************************/
bool CMessagePump::RunInDbSvc(CDbSvc *pDbSvc)
{
	Q_ASSERT(pDbSvc);
	if (pDbSvc == nullptr)
		return false;

	DMSG msg;
	memset(&msg, 0, sizeof(DMSG));

	Q_ASSERT(m_nDBSvcMailBoxID != 0);

	msg.RecverID = m_nDBSvcMailBoxID;

	bool bRet = false;

	while (true)
	{
		//
		bRet = RecvMail("FES", &msg, 0);
		if (bRet == false)
			break;
		// 收到属于DB_SVC的邮件
		if (bRet)
		{
			bool bRet2 = false;
			bRet2 = m_pExecutor->Run(pDbSvc, &msg);


		}

	}


	return true;
}
