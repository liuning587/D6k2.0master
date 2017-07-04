#pragma once

#include <QObject>
#include "dbg_cache.h"
#include "breaker_data.h"

class CSocketConnect;
class CBreakRecver : public QObject
{
	Q_OBJECT

public:
	CBreakRecver(CSocketConnect *pSocket, QObject *parent = nullptr);
	~CBreakRecver();
public:
	//�����յ�������
	bool OnReceive(char *pBuff, int nLen);
	//��������
	bool AnalyseDbg(char *pBuff, int nLen);
	//ϵͳ��Ϣ
	void OnRecvSysInfo(char *pBuff, int nLen);
	//����ֵ��Ϣ
	void OnRecvAnalogInfo(char *pBuff, int nLen);
signals:
	void Signal_SysInfo(DBG_GET_SYS_INFO&);
private:
	CDbgCache m_dbgCache;

};
