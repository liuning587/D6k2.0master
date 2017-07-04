#pragma once

#include <QObject>

class CSocketConnect;
class DBG_HEADER;
class CBreakSender : public QObject
{
	Q_OBJECT

public:
	CBreakSender(CSocketConnect *pSocket,QObject *parent= nullptr);
	~CBreakSender();
public:
	bool OnSendRequest(DBG_HEADER *pRequest);
private:
	CSocketConnect *m_pSocketConnect;
};
