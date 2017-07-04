#include "netmanager.h"
#include "breakrecver.h"
#include "breaksender.h"
#include "socketconnect.h"

CNetManager::CNetManager(QObject *parent)
	: QObject(parent)
{
	m_pSocket = new CSocketConnect;
	m_pSender = new CBreakSender(m_pSocket);
	m_pRecver = new CBreakRecver(m_pSocket);

	m_pSocket->SetBreakerRecver(m_pRecver);
}

CNetManager::~CNetManager()
{
	m_pSender->deleteLater();
	m_pRecver->deleteLater();
	m_pSocket->deleteLater();
}


void CNetManager::ConnectToServer(const QString &strIp, int nPort)
{
	m_pSocket->ConnectSocket(strIp, nPort);
}