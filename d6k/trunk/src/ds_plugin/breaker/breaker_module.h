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

#define  TIMER_INTERVAL_GET_MEAS 1000   //定时发送查询数据

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
	//往树结构中插入数据
	void CreateTreeItem();
	//初始化菜单栏
	void InitMenu();

	//设置加载的插件的名称
	virtual void SetLoadDeviceName(const QString &strDeviceName);
	//初始化连接数据
	void InitConnectData();
	//报文监控
	bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);
	//写日志
	void WriteRunLog(const char *pModelName, const char *LogText, int nLevel);
	//获取版本号
	void SendRequest(int nRequestType);
	//获取点表数据
	CAnalysePointTable *GetPointTable()
	{
		return m_pPointTable;
	}
	//获取实时开入量
	CRealTimeDiWgt *GetRealDiWgt()
	{
		return m_RealDiWgt;
	}

	//获取实时开出量
	CRealtimeDoWgt *GetRealDoWgt()
	{
		return m_RealDoWgt;
	}

	//获取action
	CRealtimeActionWgt *GetRealAction()
	{
		return m_pActionWgt;
	}

	//获取abnormla
	CRealtimeAbnormalWgt *GetAbnormalWgt()
	{
		return m_pAbnormalWgt;
	}

	//获取soft
	CRealtimeSoftWgt *GetRealSoftWgt()
	{
		return m_pRealSoftWgt;
	}

public slots:
    void Slot_SocketConnectSuccess();
	//
	void Slot_SocketError(QString errString);
	//定时发送
	void Slot_TimerSendRequest();
	//系统信息
	void Slot_RecvSysInfo(DBG_GET_SYS_INFO& sysInfo);

	void Slot_ClickLeftTreeItem(const QModelIndex &index);

	void Slot_CloseOneTabWidget(int nIndex);
	//设置时间
	void Slot_SetDeviceTime();
	//遥控
	void SLot_RemoteControl();
	//遥控返回
	void Slot_RemoteContrExec();
	//连接
	void Slot_ConnectToSocket();
	//断连
	void Slot_DisConnectSocket();

private:
	//模块加载工具
	IMainModule *m_pMainModule;
	//主界面加载控件对象
	IMainWindow *m_pMainWindow;
	//连接数据
	std::shared_ptr<CBreakInitData> m_pNetData;
	//网络
	CNetManager *m_pNetManager;
	//当前新增的节点
	QStandardItem *m_pPluginTopItem;
	//点表数据
	CAnalysePointTable *m_pPointTable;
	//测量值实时数据
	CAnalogWgt *m_pAnalogWgt;
	//保护定值
	CProtectDevWgt *m_pProtectDev;
	//系统参数
	CSysDevWgt *m_pSysDev;
	//开入量
	CDiWdiget *m_pDi;
	//开出量
	CDoWidget *m_pDo;
	//调试信息
	CDebugWidget *m_pDebug;
	//软压板
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
	//定时获取数据
	QTimer *m_pTimer;
	//遥控状态  0  1
	int m_nRemoterControl;
	//遥控执行  分  和
	int m_nRemoteStauts;
};
