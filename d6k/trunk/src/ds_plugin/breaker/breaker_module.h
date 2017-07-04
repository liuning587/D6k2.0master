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

#define  POINT_TABLE_FILE_NAME  "/ini/breaker/DA-R791-Point.txt"

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
public slots:
    void Slot_SocketConnectSuccess();
	//系统信息
	void Slot_RecvSysInfo(DBG_GET_SYS_INFO& sysInfo);

	void Slot_ClickLeftTreeItem(const QModelIndex &index);

	void Slot_CloseOneTabWidget(int nIndex);

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
	//
	QMap<int, QWidget*> m_IndexWgt;
	//
	QMap<QWidget*, int> m_WgtIndex;
};
