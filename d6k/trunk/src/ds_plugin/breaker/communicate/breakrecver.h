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
	//调试信息
	void OnRecvDebugInfo(char *pBuff, int nLen);
	//保护定值
	void OnRecvProtectDevInfo(char *pBuff, int nLen);
	//系统参数
	void OnRecvSystemDevInfo(char *pBuff, int nLen);
	//开入量
	void OnRecvDiInfo(char *pBuff, int nLen);
	//开出两
	void OnRecvDOInfo(char *pBuff, int nLen);
	//软压板
	void OnRecvSoftDevInfo(char *pBuff, int nLen);
	//soe
	void OnRecvSoeInfo(char *pBuff, int nLen);
	//时间设置返回报文
	void OnRecvTimeInfo(char *pBuff, int nLen);
	//设置保护定值返回报文
	void OnRecvProtectAck(char *pBuff, int nLen);
	//系统定值
	void OnRcvSysAck(char *pBuff, int nLen);
	//
	void OnRecvRemoteContrlAck(char *pBuff, int nLen);
signals:
	void Signal_SysInfo(DBG_GET_SYS_INFO&);
	//测量值
	void Signal_Analog(DBG_GET_MEAS&);
	//调试信息
	void Signal_Debug(DBG_GET_MEAS&);
	//保护定值
	void Signal_ProtectDev(DBG_GET_MEAS&);
	//系统参数定值
	void Signal_SystemDev(DBG_GET_MEAS&);
	//开入量
	void Signal_DiInfo(DBG_GET_MEAS&);
	//开出量
	void Signal_DOInfo(DEG_GET_MSG2&);
	//软压板
	void Signal_SoftDev(DEG_SOFT_INFO&);
	//soe详细信息
	void Signal_SoeDetailInfo(DEG_SOE_DETAIL&);
	//遥控成功响应
	void Signal_RemoteControlAck();
private:
	CDbgCache m_dbgCache;

};
