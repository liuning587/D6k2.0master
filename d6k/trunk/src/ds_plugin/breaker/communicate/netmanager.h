#pragma once

#include <QObject>
#include <memory>

class CBreakSender;
class CBreakRecver;
class CSocketConnect;
class CNetManager : public QObject
{
	Q_OBJECT

public:
	CNetManager(QObject *parent=0);
	~CNetManager();
public:
	//��������
	void ConnectToServer(const QString &strIp, int nPort);
	//��ȡsocket
	CSocketConnect *GetSocket()
	{
		return m_pSocket;
	}
	//��ȡsender
	CBreakSender *GetSender()
	{
		return m_pSender;
	}

	//��ȡrecver
	CBreakRecver *GetRecver()
	{
		return m_pRecver;
	}
private:
	CSocketConnect *m_pSocket;
	CBreakSender *m_pSender;
	CBreakRecver *m_pRecver;
};
