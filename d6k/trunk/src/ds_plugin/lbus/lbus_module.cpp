
#if _MSC_VER >= 1600
#include "vld.h"
#endif

#include "lbus_module.h"
#define MODULE_LIB
#include "devicestudio/module_export.h"
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "devicestudio/treewidget.h"

//#include "devicestudio/treeitem_def.h"
#include "devicestudio/tabwidget.h" 

#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>

 
 
CLowBusModule::CLowBusModule()
{
 
}

void CLowBusModule::Init(IMainModule *pMainModule)
{
	Q_ASSERT(pMainModule);
	if (pMainModule == nullptr)
		return;

#if 0
	m_pMainModule = pMainModule;
	//获取主界面对象
	m_pMainWindow = pMainModule->GetMainWindow();

	Q_ASSERT(m_pMainWindow);
	if (m_pMainWindow == nullptr)	
		return;	

	CreateTreeItem();

	connect(m_pMainWindow->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(m_pMainWindow->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_ClickLeftTreeItem(const QModelIndex &)));
	connect(m_pMainWindow->GetTabWidget(), SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_CloseOneTabWidget(int)));

	QString szLog(tr("mbsim plugin load success!"));
	pMainModule->LogString("MBSIM", szLog.toStdString().c_str(), 1);

#endif

}

void CLowBusModule::UnInit()
{

}

void CLowBusModule::SetLoadDeviceName(const QString &strDeviceName)
{
	std::ignore = strDeviceName;
}

void CLowBusModule::CreateTreeItem()
{
#if 0
	//FTU
	QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pMainWindow->GetLeftTree()->model();
	QStandardItem *pTopItem = (QStandardItem*)pTreeModel->item(0, 0);

	int iDevice_num = 1;
	for (int i = 0; i < pTopItem->rowCount(); i++)
	{
		if (pTopItem->child(i, 0)->text().contains("RLY", Qt::CaseInsensitive))
		{
			iDevice_num++;
		}
	}
	QString strDeviceName = "MB_" + QString::number(iDevice_num);

	QStandardItem *pModbusItem = new QStandardItem(strDeviceName);
	pModbusItem->setData(TREE_PLUGIN_ROOT, TREE_LEVEL_DEF);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pTopItem->appendRow(pModbusItem);	 

	//遥测
	QStandardItem *pCoilItem = new QStandardItem(tr("Coil"));
	pCoilItem->setData(TI_COIL, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pModbusItem->appendRow(pCoilItem);

	//遥信
	QStandardItem *pInputItem = new QStandardItem(tr("Input"));
	pInputItem->setData(TI_INPUT, Qt::UserRole);
//	pInputItem->setData(TREE_PLUGIN_CHILD_1, TREE_LEVEL_DEF);

	//newItem->setIcon(QIcon(ALARM_PNG));
	pModbusItem->appendRow(pInputItem);

	//遥控
	QStandardItem *pHoldingReg = new QStandardItem(tr("Holding Register"));
	pHoldingReg->setData(TI_HR, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pModbusItem->appendRow(pHoldingReg);

	//遥控
	QStandardItem *pInputReg = new QStandardItem(tr("Input Register"));
	pInputReg->setData(TI_IR, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pModbusItem->appendRow(pInputReg);

#endif

}

void CLowBusModule::Slot_ClickLeftTreeItem(const QModelIndex &qIndex)
{
#if 0
	int nType = qIndex.data(Qt::UserRole).toInt();

	if (nType == TI_COIL)
	{
		if (m_mapIndexTab.contains(TI_COIL))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_mapIndexTab[TI_COIL]);
		}
		else
		{
			CCoilView *pCoilView = new CCoilView(m_pMBSvc->GetMemData());
			m_pMainWindow->GetTabWidget()->AddTab(pCoilView, tr("Coil"), "Coil");
			m_mapIndexTab.insert(TI_COIL, pCoilView);
			m_mapTabIndex.insert(pCoilView, TI_COIL);
			//遥信实时数据
// 			QWidget *testWgt = new QWidget;
// 			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥信实时数据"), tr("yxsssj"));
// 			m_MIndexTab.insert(TREE_REMOTE_SIGNALITEM, testWgt);
// 			m_MTabIndex.insert(testWgt, TREE_REMOTE_SIGNALITEM);
		}

	}
 
	else if (nType == TREE_REMOTE_MEASUREITEM)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_MEASUREITEM))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_MEASUREITEM]);
		}
		else
		{
			//遥测实时数据
			QWidget *testWgt = new QWidget;
			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥测实时数据"), tr("ycsssj"));
			m_MIndexTab.insert(TREE_REMOTE_MEASUREITEM, testWgt);
			m_MTabIndex.insert(testWgt, TREE_REMOTE_MEASUREITEM);
		}

	}
	else if (nType == TREE_REMOTE_CONTROLITEM)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_CONTROLITEM))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_CONTROLITEM]);
		}
		else
		{
			//遥控实时数据
			QWidget *testWgt = new QWidget;
			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥控实时数据"), tr("yksssj"));
			m_MIndexTab.insert(TREE_REMOTE_CONTROLITEM, testWgt);
			m_MTabIndex.insert(testWgt, TREE_REMOTE_CONTROLITEM);

		}
	}
#endif
}

void CLowBusModule::Slot_CloseOneTabWidget(int iIndex)
{
	//
	QWidget *pCloseWgt = m_pMainWindow->GetTabWidget()->widget(iIndex);
	
	if (pCloseWgt != nullptr)
	{
		int iIndex = m_mapTabIndex[pCloseWgt];
		m_mapIndexTab.remove(iIndex);
		m_mapTabIndex.remove(pCloseWgt);
		m_pMainWindow->GetTabWidget()->removeTab(iIndex);
	}
}

bool CLowBusModule::CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc)
{
	Q_ASSERT(m_pMainModule);
	if (m_pMainModule == nullptr)
		return false;
	return m_pMainModule->CommMsgLog(pszModuleName, pMsg, nMsgLen,nMsgType,pMsgSubType,pSource,pDesc);
}

bool CLowBusModule::LogString(const char *pszMoudleName,const char *szLogTxt, int nLevel)
{
	Q_ASSERT(m_pMainModule);
	if (m_pMainModule == nullptr)
		return false;
	return m_pMainModule->LogString(pszMoudleName, szLogTxt, nLevel);
}

void CLowBusModule::showMouseRightButton(const QPoint &point)
{
 
}

void CLowBusModule::ShowMenu(QModelIndex &index)
{
 
}
void CLowBusModule::ShowRootMenu(QModelIndex &index)
{
	Q_UNUSED(index);
	QMenu *pMenu = new QMenu(nullptr);

 
}