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
	//����ʱ������
	bool OnSendTimeSet(TIME_SET_INFO *pTimeSet);
	//�������ֽ�����
	bool OnSend4Data(DBG_GET_MEAS &dbgData);
	//�������ֽ�����
	bool OnSend2Data(DEG_GET_MSG2 &dgbData);
	//����ң��
	bool OnSendRemoteControl(REMOTE_CONTROM_SET &dgControl);
	//������ѹ��
	bool OnSendSoftData(DEG_SOFT_INFO &soft);
	//����soe��������
	bool OnSendSoeRequest(DEG_GET_SOE *pSeoRequest);
	//����SOe���
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
	//soe����״̬   SoeType==0xFFFF  SoeType==0XAA55 ACTION...
	int m_nSoeRequestType;
};
