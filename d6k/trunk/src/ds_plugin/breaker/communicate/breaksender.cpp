#include "breaksender.h"
#include "socketconnect.h"
#include "breaker_data.h"
#include "breaker_module.h"
#include <QDebug>

CBreakSender::CBreakSender(CSocketConnect *pSocket, QObject *parent)
	: QObject(parent)
{
	Q_ASSERT(pSocket);
	if (pSocket == nullptr)
	{
		return;
	}

	m_nSoeRequestType = -1;

	m_pSocketConnect = pSocket;

}

CBreakSender::~CBreakSender()
{
}

bool CBreakSender::OnSendRequest(DBG_HEADER *pRequest)
{
	Q_ASSERT(pRequest);

	if (pRequest == nullptr)
	{
		return false;
	}

	//·¢ËÍÊý¾Ý
	return m_pSocketConnect->WriteSocketData((char*)(pRequest),sizeof(DBG_HEADER));

}

bool CBreakSender::OnSendTimeSet(TIME_SET_INFO * pTimeSet)
{
	Q_ASSERT(pTimeSet);

	if (pTimeSet == nullptr)
	{
		return false;
	}

	//pTimeSet
	return m_pSocketConnect->WriteSocketData((char*)(pTimeSet), sizeof(TIME_SET_INFO));
}

bool CBreakSender::OnSend4Data(DBG_GET_MEAS & dbgData)
{
	return m_pSocketConnect->WriteSocketData((char*)(&dbgData), dbgData.msgLeg.GetAddr() + 4);
}

bool CBreakSender::OnSend2Data(DEG_GET_MSG2 & dgbData)
{
	return m_pSocketConnect->WriteSocketData((char*)(&dgbData), dgbData.msgLeg.GetAddr() + 4);
}

bool CBreakSender::OnSendRemoteControl(REMOTE_CONTROM_SET & dgControl)
{
	return m_pSocketConnect->WriteSocketData((char*)(&dgControl), dgControl.msgLeg.GetAddr()+4);
}

bool CBreakSender::OnSendSoftData(DEG_SOFT_INFO & soft)
{
	return m_pSocketConnect->WriteSocketData((char*)(&soft), soft.msgLeg.GetAddr() + 4);
}

bool CBreakSender::OnSendSoeRequest(DEG_GET_SOE * pSeoRequest)
{
	Q_ASSERT(pSeoRequest);

	if (pSeoRequest == nullptr)
	{
		return false;
	}


	return m_pSocketConnect->WriteSocketData((char*)(pSeoRequest), sizeof(DEG_GET_SOE));

}
