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
class CSoeHistoryWgt;
class CSoeClearWgt;

//���
#define  POINT_TABLE_FILE_NAME  "/ini/breaker/DA-R791-Point.txt"
//��ӳ��
#define  POINT_REALTIME_FILE_NAME "/ini/breaker/Real-Time.ini"
//������Ϣ����
#define  BASE_INFO_FILE_NAME    "/ini/breaker/baseInfo.ini"
//¼���ļ�Ŀ¼
#define  LB_FILE_DIR "/ini/breaker/lbFile/"
//¼����ִ���ļ�
#define  LB_EXEC_DIR "/ini/breaker/lbexe/"
#define  LB_EXEC_FILE "/ini/breaker/lbexe/NWA.exe"
#define  LB_HSITORY_DIR "/ini/breaker/lbHistory/"

#define  TIMER_INTERVAL_GET_MEAS 1000   //��ʱ���Ͳ�ѯ����
#define  TIMER_INTERVAL_GET_DEBUG 3000   //��ʱ���͵�������

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

	//��ȡsoe����
	QString GetSoeDestr(int nType);

	//ֹͣ��ʱ��
	void StopTimer();

	void StartTimer();


	//
	void SendSoeClearRequest(int nType);

public slots:
    void Slot_SocketConnectSuccess();
	//
	void Slot_SocketError(QString errString);
	//��ʱ����
	void Slot_TimerSendRequest();
	//��ʱ���͵�������
	void Slot_DebugSendRequest();
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
	//help
	void Slot_Help();
	//�źŸ���
	void Slot_Reset();
	//soe  clear
	void Slot_SoeClear();
	//soe��ղ���
	void Slot_SoeClearAck();
	//login����
	void Slot_LoginManager();

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
	//soe history
	CSoeHistoryWgt *m_pSoeHistory;

	QMap<int, QWidget*> m_IndexWgt;
	//
	QMap<QWidget*, int> m_WgtIndex;
	//��ʱ��ȡ����  ��������
	QTimer *m_pTimer;
	//��ʱ��ȡ����  ��������
	QTimer *m_pDebugTimer;
	//ң��״̬  0  1
	int m_nRemoterControl;
	//ң��ִ��  ��  ��
	int m_nRemoteStauts;
	//
	CSoeClearWgt *m_pSoeClearWgt;
	//
	QList<int> m_ClearSoeLst;
	//
	int m_nRemoterControlObj;
};
