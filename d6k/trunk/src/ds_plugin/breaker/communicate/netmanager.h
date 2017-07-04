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
	//连接网络
	void ConnectToServer(const QString &strIp, int nPort);
	//获取socket
	CSocketConnect *GetSocket()
	{
		return m_pSocket;
	}
	//获取sender
	CBreakSender *GetSender()
	{
		return m_pSender;
	}

	//获取recver
	CBreakRecver *GetRecver()
	{
		return m_pRecver;
	}
private:
	CSocketConnect *m_pSocket;
	CBreakSender *m_pSender;
	CBreakRecver *m_pRecver;
};
