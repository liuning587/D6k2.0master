#pragma once

#include "breaker_global.h"

#include "devicestudio/plugin_module.h"
#include <memory>
#include "breaker_data.h"

class IMainWindow;
class CBreakInitData;
class CBreakerModule;
class CNetManager;
class QStandardItem;
class CAnalysePointTable;
class CAnalogWgt;
class QTimer;
class CProtectDevWgt;
class CSysDevWgt;
class CDiWdiget;
class CDoWidget;
class CDebugWidget;
class CSoftWidget;
class CSoeWgt;
class CRealTimeDiWgt;
class CRealtimeDoWgt;
class CRealtimeActionWgt;
class CRealtimeAbnormalWgt;
class CRealtimeSoftWgt;

#define  POINT_TABLE_FILE_NAME  "/ini/breaker/DA-R791-Point.txt"
#define  POINT_REALTIME_FILE_NAME "/ini/breaker/Real-Time.ini"

#define  TIMER_INTERVAL_GET_MEAS 1000   //��ʱ���Ͳ�ѯ����

CBreakerModule *GetBreakerModuleApi();

CAnalysePointTable *GetPointTable();

class BREAKER_EXPORT CBreakerModule : public IPluginModule
{
	Q_OBJECT
public:
	CBreakerModule();
	~CBreakerModule();
public:
	virtual void Init(IMainModule *pCore);
	virtual void UnInit();
	//�����ṹ�в�������
	void CreateTreeItem();
	//��ʼ���˵���
	void InitMenu();

	//���ü��صĲ��������
	virtual void SetLoadDeviceName(const QString &strDeviceName);
	//��ʼ����������
	void InitConnectData();
	//���ļ��
	bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);
	//д��־
	void WriteRunLog(const char *pModelName, const char *LogText, int nLevel);
	//��ȡ�汾��
	void SendRequest(int nRequestType);
	//��ȡ�������
	CAnalysePointTable *GetPointTable()
	{
		return m_pPointTable;
	}
	//��ȡʵʱ������
	CRealTimeDiWgt *GetRealDiWgt()
	{
		return m_RealDiWgt;
	}

	//��ȡʵʱ������
	CRealtimeDoWgt *GetRealDoWgt()
	{
		return m_RealDoWgt;
	}

	//��ȡaction
	CRealtimeActionWgt *GetRealAction()
	{
		return m_pActionWgt;
	}

	//��ȡabnormla
	CRealtimeAbnormalWgt *GetAbnormalWgt()
	{
		return m_pAbnormalWgt;
	}

	//��ȡsoft
	CRealtimeSoftWgt *GetRealSoftWgt()
	{
		return m_pRealSoftWgt;
	}

public slots:
    void Slot_SocketConnectSuccess();
	//
	void Slot_SocketError(QString errString);
	//��ʱ����
	void Slot_TimerSendRequest();
	//ϵͳ��Ϣ
	void Slot_RecvSysInfo(DBG_GET_SYS_INFO& sysInfo);

	void Slot_ClickLeftTreeItem(const QModelIndex &index);

	void Slot_CloseOneTabWidget(int nIndex);
	//����ʱ��
	void Slot_SetDeviceTime();
	//ң��
	void SLot_RemoteControl();
	//ң�ط���
	void Slot_RemoteContrExec();
	//����
	void Slot_ConnectToSocket();
	//����
	void Slot_DisConnectSocket();

private:
	//ģ����ع���
	IMainModule *m_pMainModule;
	//��������ؿؼ�����
	IMainWindow *m_pMainWindow;
	//��������
	std::shared_ptr<CBreakInitData> m_pNetData;
	//����
	CNetManager *m_pNetManager;
	//��ǰ�����Ľڵ�
	QStandardItem *m_pPluginTopItem;
	//�������
	CAnalysePointTable *m_pPointTable;
	//����ֵʵʱ����
	CAnalogWgt *m_pAnalogWgt;
	//������ֵ
	CProtectDevWgt *m_pProtectDev;
	//ϵͳ����
	CSysDevWgt *m_pSysDev;
	//������
	CDiWdiget *m_pDi;
	//������
	CDoWidget *m_pDo;
	//������Ϣ
	CDebugWidget *m_pDebug;
	//��ѹ��
	CSoftWidget *m_pSoft;
	//soe
	CSoeWgt *m_pSoe;
	//realtime di
	CRealTimeDiWgt *m_RealDiWgt;
	//real do
	CRealtimeDoWgt *m_RealDoWgt;
	//real action
	CRealtimeActionWgt *m_pActionWgt;
	//real abnormal
	CRealtimeAbnormalWgt *m_pAbnormalWgt;
	//real soft
	CRealtimeSoftWgt *m_pRealSoftWgt;

	QMap<int, QWidget*> m_IndexWgt;
	//
	QMap<QWidget*, int> m_WgtIndex;
	//��ʱ��ȡ����
	QTimer *m_pTimer;
	//ң��״̬  0  1
	int m_nRemoterControl;
	//ң��ִ��  ��  ��
	int m_nRemoteStauts;
};
