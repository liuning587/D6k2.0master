#include "breaker_module.h"
#define MODULE_LIB
#include "devicestudio/module_export.h"
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "devicestudio/treewidget.h"
#include "configdatawgt.h"
#include "breaker_data.h"
#include "netmanager.h"
#include "socketconnect.h"
#include "breakrecver.h"
#include "breaksender.h"
#include "devicestudio/tabwidget.h"
#include "analysepointtable.h"
#include "analogwgt.h"

#include <QStandardItemModel>


static CBreakerModule *s_ptrModule = nullptr;

extern "C" DS_EXPORT IPluginModule *CreateModule()
{
	s_ptrModule = new CBreakerModule;
	return s_ptrModule;
	//	return  s_ptrModule.get();
}

CBreakerModule * GetBreakerModuleApi()
{
	return s_ptrModule;
}

CAnalysePointTable * GetPointTable()
{
	return s_ptrModule->GetPointTable();
}


CBreakerModule::CBreakerModule()
{
}

CBreakerModule::~CBreakerModule()
{

}

void CBreakerModule::Init(IMainModule *pMainModule)
{
	Q_ASSERT(pMainModule);
	if (!pMainModule)
	{
		return;
	}

	m_pPluginTopItem = nullptr;

	m_pPointTable = new CAnalysePointTable;
	m_pAnalogWgt = new CAnalogWgt;

	m_pPointTable->AnalyseFile(qApp->applicationDirPath() + POINT_TABLE_FILE_NAME);

	m_pMainModule = pMainModule;
	//获取主界面对象
	m_pMainWindow = pMainModule->GetMainWindow();


	Q_ASSERT(m_pMainWindow);
	if (!m_pMainWindow)
	{
		return;
	}

	//初始化数据结构
	CreateTreeItem();
	connect(m_pMainWindow->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_ClickLeftTreeItem(const QModelIndex &)));
	connect(m_pMainWindow->GetTabWidget(), SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_CloseOneTabWidget(int)));


	m_pNetManager = new CNetManager;
	connect(m_pNetManager->GetSocket(),SIGNAL(Signal_ConnectSuccess()),this,SLOT(Slot_SocketConnectSuccess()));
	connect(m_pNetManager->GetRecver(),SIGNAL(Signal_SysInfo(DBG_GET_SYS_INFO&)),this,SLOT(Slot_RecvSysInfo(DBG_GET_SYS_INFO&)));

	//初始化数据
	InitConnectData();
}

void CBreakerModule::UnInit()
{
	m_pNetManager->deleteLater();
	m_pAnalogWgt->deleteLater();
	delete m_pPointTable;
}

void CBreakerModule::CreateTreeItem()
{
	//breaker
	QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pMainWindow->GetLeftTree()->model();
	QStandardItem *pTopItem = (QStandardItem*)pTreeModel->item(0, 0);

	int iDevice_num = 1;
	for (int i = 0; i < pTopItem->rowCount(); i++)
	{
		if (pTopItem->child(i, 0)->text().contains("Breaker", Qt::CaseInsensitive))
		{
			iDevice_num++;
		}
	}

	QString strDeviceName = "Breaker_" + QString::number(iDevice_num);

	QStandardItem *pBreakerItem = new QStandardItem(strDeviceName);
	m_pPluginTopItem = pBreakerItem;
	pBreakerItem->setData(TREE_ITEM_BREAKER, Qt::UserRole+1);
	//pFtuItem->setIcon(QIcon(":/icons/ftu.png"));
	pTopItem->appendRow(pBreakerItem);

	//测量值
	QStandardItem *pAnalogItem = new QStandardItem(tr("测量值"));
	pAnalogItem->setData(TREE_ITEM_BREAKER_ANALOG, Qt::UserRole+1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pBreakerItem->appendRow(pAnalogItem);

}

void CBreakerModule::SetLoadDeviceName(const QString &strDeviceName)
{
	Q_UNUSED(strDeviceName);
}

void CBreakerModule::InitConnectData()
{
	//初始化数据
	m_pNetData = std::make_shared<CBreakInitData>();

	//初始化连接数据
	CConfigDataWgt *pDataConfigWgt = new CConfigDataWgt;
	if (pDataConfigWgt->exec())
	{
		m_pNetData->SetIpAddress(pDataConfigWgt->GetIpAddress());
		m_pNetData->SetPort(pDataConfigWgt->GetPort());

		//连接网络
		m_pNetManager->ConnectToServer(m_pNetData->GetIpAddress(), m_pNetData->GetPort());
	}

	pDataConfigWgt->deleteLater();
}

bool CBreakerModule::CommMsgLog(const char * pszModuleName, const char * pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char * pSource, const char * pDesc)
{

	return m_pMainModule->CommMsgLog(pszModuleName, pMsg, nMsgLen, nMsgType, pMsgSubType, pSource, pDesc);

}

void CBreakerModule::WriteRunLog(const char *pModelName, const char *LogText, int nLevel)
{
	m_pMainModule->LogString(pModelName, LogText, nLevel);
}

void CBreakerModule::SendRequest(int nRequestType)
{
	DBG_HEADER dbgHeader;
	dbgHeader.header0 = 0xAA;
	dbgHeader.header1 = 0x55;

	dbgHeader.msgLeg.SetAddr(1);
	//获取系统信息
	dbgHeader.type = nRequestType;

	m_pNetManager->GetSender()->OnSendRequest(&dbgHeader);
}

void CBreakerModule::Slot_SocketConnectSuccess()
{
	SendRequest(DBG_CODE_GET_SYS_INFO);

	WriteRunLog("Breaker", "Request System Data .....", 1);
}

void CBreakerModule::Slot_RecvSysInfo(DBG_GET_SYS_INFO& sysInfo)
{
	QString strSysInfo = tr("Version:%1   Translate Time:%2").arg(sysInfo.version).arg(sysInfo.time.Dump());
	WriteRunLog("Breaker",strSysInfo.toStdString().c_str(),1);
}

void CBreakerModule::Slot_ClickLeftTreeItem(const QModelIndex &qIndex)
{
	int nType = qIndex.data(Qt::UserRole+1).toInt();

	if (nType == TREE_ITEM_BREAKER_ANALOG)
	{
		//测量值
		if (m_WgtIndex.contains(m_pAnalogWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentIndex(m_WgtIndex[m_pAnalogWgt]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pAnalogWgt, tr("测量值"), "AnalogValue");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pAnalogWgt);
			m_WgtIndex.insert(m_pAnalogWgt, m_pMainWindow->GetTabWidget()->currentIndex());

			m_IndexWgt.insert(m_pMainWindow->GetTabWidget()->currentIndex(), m_pAnalogWgt);
		}
	}
}

void CBreakerModule::Slot_CloseOneTabWidget(int nIndex)
{
	if (m_IndexWgt.contains(nIndex))
	{
		m_WgtIndex.remove(m_IndexWgt[nIndex]);
		m_IndexWgt.remove(nIndex);
	}
	m_pMainWindow->GetTabWidget()->removeTab(nIndex);
	
}

