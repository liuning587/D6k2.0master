#pragma once

#include <QObject>
#include "breaker_data.h"

class CSocketConnect;
class DBG_HEADER;
class TIME_SET_INFO;
class CBreakSender : public QObject
{
	Q_OBJECT

public:
	CBreakSender(CSocketConnect *pSocket,QObject *parent= nullptr);
	~CBreakSender();
public:
	bool OnSendRequest(DBG_HEADER *pRequest);
	//发送时间设置
	bool OnSendTimeSet(TIME_SET_INFO *pTimeSet);
	//发送四字节数据
	bool OnSend4Data(DBG_GET_MEAS &dbgData);
	//发送两字节数据
	bool OnSend2Data(DEG_GET_MSG2 &dgbData);
	//发送遥控
	bool OnSendRemoteControl(REMOTE_CONTROM_SET &dgControl);
	//发送软压板
	bool OnSendSoftData(DEG_SOFT_INFO &soft);
	//发送soe请求数据
	bool OnSendSoeRequest(DEG_GET_SOE *pSeoRequest);
	//发送SOe清空
	bool OnSendSoeClearRequestr(DEG_SOE_CLEAR &dgbData);
	//
	void SetSoeType(int nType)
	{
		m_nSoeRequestType = nType;
	}

	int GetSoeType()
	{
		return m_nSoeRequestType;
	}
private:
	CSocketConnect *m_pSocketConnect;
	//soe请求状态   SoeType==0xFFFF  SoeType==0XAA55 ACTION...
	int m_nSoeRequestType;
};
