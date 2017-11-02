#ifndef LOW_BUS_MODULE_H
#define LOW_BUS_MODULE_H


#include "ds_global.h"
#include "devicestudio/plugin_module.h"

#include <QMap>

#include <memory>

class IMainWindow;

class DS_SHARED_EXPORT CLowBusModule : public IPluginModule
{
	Q_OBJECT
public:
	CLowBusModule();
	//�����ṹ�в�������
	void CreateTreeItem();

	bool LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel);
	bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);

public:
	virtual void Init(IMainModule *pCore);
	virtual void UnInit();
	//���ü��صĲ��������
	virtual void SetLoadDeviceName(const QString &strDeviceName);
protected:
	void ShowMenu(QModelIndex &index);
	void ShowRootMenu(QModelIndex &index);

	public slots:
	void Slot_ClickLeftTreeItem(const QModelIndex &mIndex);
	void Slot_CloseOneTabWidget(int iIndex);
	void showMouseRightButton(const QPoint &point);

private:
	//ģ����ع���
	IMainModule *m_pMainModule;
	//��������ؿؼ�����
	IMainWindow *m_pMainWindow;
	//���ṹ ���  ��Ӧ��tabҳ
	QMap<int, QWidget*> m_mapIndexTab;
	//���ṹ tabҳ  ���
	QMap<QWidget*, int> m_mapTabIndex;

//	std::shared_ptr<CModbusSvc> m_pMBSvc;
};


#endif // MBSIM_H
