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

	qDebug() << QByteArray((char*)(pRequest), sizeof(DBG_HEADER)).toHex();
	//·¢ËÍÊý¾Ý
	return m_pSocketConnect->WriteSocketData((char*)(pRequest),sizeof(DBG_HEADER));

}
