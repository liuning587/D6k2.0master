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
	//������Ϣ
	void OnRecvDebugInfo(char *pBuff, int nLen);
	//������ֵ
	void OnRecvProtectDevInfo(char *pBuff, int nLen);
	//ϵͳ����
	void OnRecvSystemDevInfo(char *pBuff, int nLen);
	//������
	void OnRecvDiInfo(char *pBuff, int nLen);
	//������
	void OnRecvDOInfo(char *pBuff, int nLen);
	//��ѹ��
	void OnRecvSoftDevInfo(char *pBuff, int nLen);
	//soe
	void OnRecvSoeInfo(char *pBuff, int nLen);
	//ʱ�����÷��ر���
	void OnRecvTimeInfo(char *pBuff, int nLen);
	//���ñ�����ֵ���ر���
	void OnRecvProtectAck(char *pBuff, int nLen);
	//ϵͳ��ֵ
	void OnRcvSysAck(char *pBuff, int nLen);
	//
	void OnRecvRemoteContrlAck(char *pBuff, int nLen);
signals:
	void Signal_SysInfo(DBG_GET_SYS_INFO&);
	//����ֵ
	void Signal_Analog(DBG_GET_MEAS&);
	//������Ϣ
	void Signal_Debug(DBG_GET_MEAS&);
	//������ֵ
	void Signal_ProtectDev(DBG_GET_MEAS&);
	//ϵͳ������ֵ
	void Signal_SystemDev(DBG_GET_MEAS&);
	//������
	void Signal_DiInfo(DBG_GET_MEAS&);
	//������
	void Signal_DOInfo(DEG_GET_MSG2&);
	//��ѹ��
	void Signal_SoftDev(DEG_SOFT_INFO&);
	//soe��ϸ��Ϣ
	void Signal_SoeDetailInfo(DEG_SOE_DETAIL&);
	//ң�سɹ���Ӧ
	void Signal_RemoteControlAck();
private:
	CDbgCache m_dbgCache;

};
