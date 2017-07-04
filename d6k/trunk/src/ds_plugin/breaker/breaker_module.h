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
	//�����ṹ�в�������
	void CreateTreeItem();

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
public slots:
    void Slot_SocketConnectSuccess();
	//ϵͳ��Ϣ
	void Slot_RecvSysInfo(DBG_GET_SYS_INFO& sysInfo);

	void Slot_ClickLeftTreeItem(const QModelIndex &index);

	void Slot_CloseOneTabWidget(int nIndex);

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
	//
	QMap<int, QWidget*> m_IndexWgt;
	//
	QMap<QWidget*, int> m_WgtIndex;
};
