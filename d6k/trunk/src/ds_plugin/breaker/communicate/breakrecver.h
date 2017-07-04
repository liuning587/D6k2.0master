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
	//解析收到的数据
	bool OnReceive(char *pBuff, int nLen);
	//解析数据
	bool AnalyseDbg(char *pBuff, int nLen);
	//系统信息
	void OnRecvSysInfo(char *pBuff, int nLen);
	//测量值信息
	void OnRecvAnalogInfo(char *pBuff, int nLen);
signals:
	void Signal_SysInfo(DBG_GET_SYS_INFO&);
private:
	CDbgCache m_dbgCache;

};
