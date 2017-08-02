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
#include "protectdevwgt.h"
#include "sysdevwgt.h"
#include "diwdiget.h"
#include "dowidget.h"
#include "debugwidget.h"
#include "softwidget.h"
#include "soewgt.h"
#include "realtimediwgt.h"
#include "realtimedowgt.h"
#include "realtimeactionwgt.h"
#include "realtimeabnormalwgt.h"
#include "realtimesoftwgt.h"
#include "datatimeeditwgt.h"
#include "soehistorywgt.h"
#include "soe_clearwgt.h"
#include "logic_manager.h"

#include <QStandardItemModel>
#include <QTimer>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMessageBox>
#include <QSettings>

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

	
	m_nRemoterControl = 0;
	m_nRemoteStauts = 0;
	m_pPluginTopItem = nullptr;

	m_pPointTable = new CAnalysePointTable;

	m_pPointTable->AnalyseFile(qApp->applicationDirPath() + POINT_TABLE_FILE_NAME);
	m_pPointTable->AnalyseRealFile(qApp->applicationDirPath() + POINT_REALTIME_FILE_NAME);

	m_pMainModule = pMainModule;
	//��ȡ���������
	m_pMainWindow = pMainModule->GetMainWindow();


	Q_ASSERT(m_pMainWindow);
	if (!m_pMainWindow)
	{
		return;
	}


	//��ʱ��
	m_pTimer = new QTimer(this);
	m_pTimer->setInterval(TIMER_INTERVAL_GET_MEAS);
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(Slot_TimerSendRequest()));

	m_pDebugTimer = new QTimer(this);
	m_pDebugTimer->setInterval(TIMER_INTERVAL_GET_DEBUG);
	connect(m_pDebugTimer, SIGNAL(timeout()), this, SLOT(Slot_DebugSendRequest()));

	//��ʼ�����ݽṹ
	CreateTreeItem();
	connect(m_pMainWindow->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_ClickLeftTreeItem(const QModelIndex &)));
	connect(m_pMainWindow->GetTabWidget(), SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_CloseOneTabWidget(int)));


	m_pNetManager = new CNetManager;
	//����ֵ
	m_pAnalogWgt = new CAnalogWgt;
	//������Ϣ
	m_pDebug = new CDebugWidget(m_pNetManager);
	//������ֵ
	m_pProtectDev = new CProtectDevWgt(m_pNetManager);;
	//ϵͳ����
	m_pSysDev = new CSysDevWgt(m_pNetManager);
	//������
	m_pDi = new CDiWdiget(m_pNetManager);
	//������
	m_pDo = new CDoWidget(m_pNetManager);
	//��ѹ��
	m_pSoft = new CSoftWidget(m_pNetManager);
	//soe
	m_pSoe = new CSoeWgt(m_pNetManager);

	//ʵʱdi
	m_RealDiWgt = new CRealTimeDiWgt;
	//ʵʱdo
	m_RealDoWgt = new CRealtimeDoWgt;
	//ʵʱaction
	m_pActionWgt = new CRealtimeActionWgt;
	//ʵʱabnormal
	m_pAbnormalWgt = new CRealtimeAbnormalWgt;
	//real soft
	m_pRealSoftWgt = new CRealtimeSoftWgt;
	//soehistory
	m_pSoeHistory = new CSoeHistoryWgt(m_pNetManager);;

	m_pSoeClearWgt = new CSoeClearWgt;

	connect(m_pNetManager->GetSocket(),SIGNAL(Signal_ConnectSuccess()),this,SLOT(Slot_SocketConnectSuccess()));
	connect(m_pNetManager->GetSocket(), SIGNAL(Signal_SocketError(QString)), this, SLOT(Slot_SocketError(QString)));
	connect(m_pNetManager->GetRecver(),SIGNAL(Signal_SysInfo(DBG_GET_SYS_INFO&)),this,SLOT(Slot_RecvSysInfo(DBG_GET_SYS_INFO&)));
	//����ֵ
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_Analog(DBG_GET_MEAS&)), m_pAnalogWgt, SLOT(Slot_RecvNewRealTimeData(DBG_GET_MEAS&)));
	//������Ϣ
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_Debug(DBG_GET_MEAS&)), m_pDebug, SLOT(Slot_RecvNewRealTimeData(DBG_GET_MEAS&)));
	//������ֵ
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_ProtectDev(DBG_GET_MEAS&)), m_pProtectDev, SLOT(Slot_RecvNewRealTimeData(DBG_GET_MEAS&)));
	//ϵͳ����
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_SystemDev(DBG_GET_MEAS&)), m_pSysDev, SLOT(Slot_RecvNewRealTimeData(DBG_GET_MEAS&)));
	//������
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_DiInfo(DBG_GET_MEAS&)), m_pDi, SLOT(Slot_RecvNewRealTimeData(DBG_GET_MEAS&)));
	//������
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_DOInfo(DEG_GET_MSG2&)), m_pDo, SLOT(Slot_RecvNewRealTimeData(DEG_GET_MSG2&)));
	//��ѹ��
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_SoftDev(DEG_SOFT_INFO&)), m_pSoft, SLOT(Slot_RecvNewRealTimeData(DEG_SOFT_INFO&)));

	//soe
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_SoeDetailInfo(DEG_SOE_DETAIL)), m_pSoe, SLOT(Slot_RecvNewRealTimeData(DEG_SOE_DETAIL)));
	connect(m_pAnalogWgt,SIGNAL(Signal_SeoInfo(int)),m_pSoe,SLOT(Slot_SoeUpdate(int)));

	//soehisoty
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_SoeDetailInfo(DEG_SOE_DETAIL)), m_pSoeHistory, SLOT(Slot_RecvNewRealTimeData(DEG_SOE_DETAIL)));


	//ң��
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_RemoteControlAck()), this, SLOT(Slot_RemoteContrExec()));

	//soe��ղ���
	connect(m_pNetManager->GetRecver(), SIGNAL(Signal_SeoClearSuccess()), this, SLOT(Slot_SoeClearAck()));
	//��ʼ������
	InitConnectData();
	InitMenu();
	//չ����
	m_pMainWindow->GetLeftTree()->expandAll();
}

void CBreakerModule::UnInit()
{
	m_pNetManager->deleteLater();
	m_pAnalogWgt->deleteLater();
	delete m_pPointTable;
	m_pTimer->deleteLater();
	m_pDebugTimer->deleteLater();
	m_pProtectDev->deleteLater();
	m_pSysDev->deleteLater();
	m_pDi->deleteLater();
	m_RealDiWgt->deleteLater();
	m_RealDoWgt->deleteLater();
	m_pActionWgt->deleteLater();
	m_pAbnormalWgt->deleteLater();
	m_pRealSoftWgt->deleteLater();
	m_pSoeHistory->deleteLater();
	m_pSoeClearWgt->deleteLater();
}

void CBreakerModule::CreateTreeItem()
{
	//breaker
	QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pMainWindow->GetLeftTree()->model();
	QStandardItem *pTopItem = (QStandardItem*)pTreeModel->item(0, 0);

	int iDevice_num = 1;
	for (int i = 0; i < pTopItem->rowCount(); i++)
	{
		if (pTopItem->child(i, 0)->text().contains(tr("��ѹֱ����·��������_"), Qt::CaseInsensitive))
		{
			iDevice_num++;
		}
	}

	QString strDeviceName = tr("��ѹֱ����·��������_") + QString::number(iDevice_num);

	QStandardItem *pBreakerItem = new QStandardItem(strDeviceName);
	m_pPluginTopItem = pBreakerItem;
	pBreakerItem->setData(TREE_ITEM_BREAKER, Qt::UserRole+1);
	//pFtuItem->setIcon(QIcon(":/icons/ftu.png"));
	pTopItem->appendRow(pBreakerItem);



	//����ֵ
	QStandardItem *pAnalogItem = new QStandardItem(tr("��ֵ��Ϣ"));
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pBreakerItem->appendRow(pAnalogItem);

	//��������
	//����ֵ
	QStandardItem *pAnalogDataItem = new QStandardItem(tr("��������"));
	pAnalogDataItem->setData(TREE_ITEM_BREAKER_ANALOG, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pAnalogDataItem);

	//������Ϣ
	QStandardItem *pDebugItem = new QStandardItem(tr("��������"));
	pDebugItem->setData(TREE_ITEM_DEBUG, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pDebugItem);
	
	//������
	QStandardItem *pRealtimeDIItem = new QStandardItem(tr("������"));
	pRealtimeDIItem->setData(TREE_ITEM_REALTIME_DI, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pRealtimeDIItem);

	//������v
	QStandardItem *pRealtimeDoItem = new QStandardItem(tr("������"));
	pRealtimeDoItem->setData(TREE_ITEM_REALTIME_DO, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pRealtimeDoItem);
	//�����ź�
	QStandardItem *pRealtimActionItem = new QStandardItem(tr("�����ź�"));
	pRealtimActionItem->setData(TREE_ITEM_REALTIME_ACTION, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pRealtimActionItem);

	//�쳣�ź�
	QStandardItem *pRealtimDisNormalItem = new QStandardItem(tr("�쳣�ź�"));
	pRealtimDisNormalItem->setData(TREE_ITEM_REALTIME_DISNORMAL, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pRealtimDisNormalItem);

	//��ѹ��״̬
	QStandardItem *pRealtimSoftItem = new QStandardItem(tr("��ѹ��״̬"));
	pRealtimSoftItem->setData(TREE_ITEM_REALTIME_SOFT, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pAnalogItem->appendRow(pRealtimSoftItem);


	//������Ϣ 
	QStandardItem *pParamItem = new QStandardItem(tr("������Ϣ"));
	//pDebugItem->setData(TREE_ITEM_DEBUG, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pBreakerItem->appendRow(pParamItem);


	
	//������ֵ
	QStandardItem *pProtectDevItem = new QStandardItem(tr("������ֵ"));
	pProtectDevItem->setData(TREE_ITEM_PROTECT_DEV, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pParamItem->appendRow(pProtectDevItem);
	//ϵͳ����
	QStandardItem *pSysDevDevItem = new QStandardItem(tr("ϵͳ����"));
	pSysDevDevItem->setData(TREE_ITEM_SYSTEM_DEV, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pParamItem->appendRow(pSysDevDevItem);

	//������
	QStandardItem *pDiItem = new QStandardItem(tr("�������"));
	pDiItem->setData(TREE_ITEM_DI_IFNO, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pParamItem->appendRow(pDiItem);

	//������
	QStandardItem *pDoItem = new QStandardItem(tr("��������"));
	pDoItem->setData(TREE_INFO_DO_INFO, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pParamItem->appendRow(pDoItem);

	//��ѹ��
	QStandardItem *pSoftItem = new QStandardItem(tr("��ѹ��Ͷ��"));
	pSoftItem->setData(TREE_INFO_SOFT, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pParamItem->appendRow(pSoftItem);

    //soe
	QStandardItem *pSoeItem = new QStandardItem(tr("SOE�¼�"));
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pBreakerItem->appendRow(pSoeItem);

	//SOE
	QStandardItem *pRealTimeSoeItem = new QStandardItem(tr("ʵʱ�¼�"));
	pRealTimeSoeItem->setData(TREE_INFO_SOE, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pSoeItem->appendRow(pRealTimeSoeItem);
	//��ʷsoe
	QStandardItem *pHistoryTimeSoeItem = new QStandardItem(tr("��ʷ�¼�"));
	pHistoryTimeSoeItem->setData(TREE_ITEM_HIOSTORY_SOE, Qt::UserRole + 1);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pSoeItem->appendRow(pHistoryTimeSoeItem);

}

//��ʼ���˵���
void CBreakerModule::InitMenu()
{
	QMenu *pPlcMenu = m_pMainWindow->menuBar()->addMenu(tr("װ�ò���(C)"));

	//����
	QAction *pConnectAct = new QAction(tr("����(C)"), pPlcMenu);
	pPlcMenu->addAction(pConnectAct);

	//����
	QAction *pDisConnectAct = new QAction(tr("����(D)"), pPlcMenu);
	pPlcMenu->addAction(pDisConnectAct);

	pPlcMenu->addSeparator();
	//����ʱ��
	QAction *pTimeAct = new QAction(tr("����ʱ��(S)"), pPlcMenu);
	pPlcMenu->addAction(pTimeAct);

	//ң��
	QAction *pRemoteControlAct = new QAction(tr("ң��(R)"), pPlcMenu);
	pPlcMenu->addAction(pRemoteControlAct);

	//�źŸ���
	QAction *pResetAct = new QAction(tr("�źŸ���(G)"), pPlcMenu);
	pPlcMenu->addAction(pResetAct);
	//���
	QAction *pClearAce = new QAction(tr("���SOE�¼�(E)"), pPlcMenu);
	pPlcMenu->addAction(pClearAce);

	QMenu *pSysMenu = m_pMainWindow->menuBar()->addMenu(tr("ϵͳ����(S)"));
	QAction *pLoginAct = new QAction(tr("���õ�¼����(P)"), pSysMenu);
	pSysMenu->addAction(pLoginAct);

	connect(pLoginAct, SIGNAL(triggered()), this, SLOT(Slot_LoginManager()));

	QMenu *pHelpMenu = m_pMainWindow->menuBar()->addMenu(tr("����(H)"));
	QAction *pHelpAct = new QAction(tr("����(A)"), pHelpMenu);
	pHelpMenu->addAction(pHelpAct);



	connect(pTimeAct, SIGNAL(triggered()), this, SLOT(Slot_SetDeviceTime()));
	connect(pRemoteControlAct, SIGNAL(triggered()), this, SLOT(SLot_RemoteControl()));
	connect(pResetAct, SIGNAL(triggered()), this, SLOT(Slot_Reset()));
	connect(pClearAce, SIGNAL(triggered()), this, SLOT(Slot_SoeClear()));

	connect(pConnectAct, SIGNAL(triggered()), this, SLOT(Slot_ConnectToSocket()));
	connect(pDisConnectAct, SIGNAL(triggered()), this, SLOT(Slot_DisConnectSocket()));

	connect(pHelpAct, SIGNAL(triggered()), this, SLOT(Slot_Help()));
}

void CBreakerModule::SetLoadDeviceName(const QString &strDeviceName)
{
	Q_UNUSED(strDeviceName);
}

void CBreakerModule::InitConnectData()
{
	//��ʼ������
	m_pNetData = std::make_shared<CBreakInitData>();

	//��ȡ�����ļ�����
	QSettings *configIniRead = new QSettings(qApp->applicationDirPath() + BASE_INFO_FILE_NAME, QSettings::IniFormat);
	//
	QString strIp = configIniRead->value("Socket/ip").toString();
	QString port = configIniRead->value("Socket/port").toString();

	QString strFtpUserName = configIniRead->value("FTP/username").toString();
	QString strFtpPassWd = configIniRead->value("FTP/password").toString();
	QString strFtpDir = configIniRead->value("FTP/ftpdir").toString();

	//��ʼ����������
	CConfigDataWgt *pDataConfigWgt = new CConfigDataWgt;
	pDataConfigWgt->SetIpAddress(strIp);
	pDataConfigWgt->SetPort(port);

	if (pDataConfigWgt->exec())
	{
		m_pNetData->SetIpAddress(pDataConfigWgt->GetIpAddress());
		m_pNetData->SetPort(pDataConfigWgt->GetPort());

		configIniRead->setValue("Socket/ip", pDataConfigWgt->GetIpAddress());
		configIniRead->setValue("Socket/port", pDataConfigWgt->GetPort());
		//��������
		m_pNetManager->ConnectToServer(m_pNetData->GetIpAddress(), m_pNetData->GetPort());
	}

	//������������
	m_pSoeHistory->SetFtpIp(strIp);
	m_pSoeHistory->SetFtpUserName(strFtpUserName);
	m_pSoeHistory->SetFtpPasswd(strFtpPassWd);
	m_pSoeHistory->SetFtpDir(strFtpDir);

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
	//��ȡϵͳ��Ϣ
	dbgHeader.type = nRequestType;

	m_pNetManager->GetSender()->OnSendRequest(&dbgHeader);
}

void CBreakerModule::StopTimer()
{
	m_pTimer->stop();
	m_pDebugTimer->stop();
}

void CBreakerModule::StartTimer()
{
	m_pTimer->start();
	m_pDebugTimer->start();
}

void CBreakerModule::Slot_SocketConnectSuccess()
{
	//�������
	m_pSoeHistory->ClearNums();
	m_ClearSoeLst.clear();

	SendRequest(DBG_CODE_GET_SYS_INFO);

	m_pNetManager->GetSender()->SetSoeType(-1);

	QString strInfo = tr("���ͻ�ȡϵͳ��Ϣ���� .....");
	
	WriteRunLog("Breaker", strInfo.toLocal8Bit().data(), 1);
	m_pTimer->start();
	m_pDebugTimer->start();
}

void CBreakerModule::Slot_SocketError(QString errString)
{
	Q_UNUSED(errString);
	m_pTimer->stop();
}

void CBreakerModule::Slot_TimerSendRequest()
{
	SendRequest(DBG_CODE_GET_MEAS);
}

void CBreakerModule::Slot_DebugSendRequest()
{
	SendRequest(DBG_CODE_GET_DEBUG_DATA);
}

void CBreakerModule::Slot_RecvSysInfo(DBG_GET_SYS_INFO& sysInfo)
{
	QString strSysInfo = tr("�汾:%1   ����ʱ��:%2").arg(sysInfo.version).arg(sysInfo.time.Dump());
	WriteRunLog("Breaker",strSysInfo.toLocal8Bit().data(),1);
}

void CBreakerModule::Slot_ClickLeftTreeItem(const QModelIndex &qIndex)
{
	int nType = qIndex.data(Qt::UserRole+1).toInt();

	if (nType == TREE_ITEM_BREAKER_ANALOG)
	{
		//����ֵ
		if (m_WgtIndex.contains(m_pAnalogWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pAnalogWgt);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pAnalogWgt, tr("��������"), "AnalogValue");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pAnalogWgt);
			m_WgtIndex.insert(m_pAnalogWgt, TREE_ITEM_BREAKER_ANALOG);

			m_IndexWgt.insert(TREE_ITEM_BREAKER_ANALOG, m_pAnalogWgt);
		}
	}
	else if (nType == TREE_ITEM_DEBUG)
	{
		//������Ϣ
		if (m_WgtIndex.contains(m_pDebug))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pDebug);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pDebug, tr("��������"), "Debug");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pDebug);
			m_WgtIndex.insert(m_pDebug, TREE_ITEM_DEBUG);

			m_IndexWgt.insert(TREE_ITEM_DEBUG, m_pDebug);
		}
	}
	else if (nType == TREE_ITEM_REALTIME_DI)
	{
		//ʵʱ����
		if (m_WgtIndex.contains(m_RealDiWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_RealDiWgt);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_RealDiWgt, tr("������"), "RealDi");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_RealDiWgt);
			m_WgtIndex.insert(m_RealDiWgt, TREE_ITEM_REALTIME_DI);

			m_IndexWgt.insert(TREE_ITEM_REALTIME_DI, m_RealDiWgt);
		}
	}
	else if (nType == TREE_ITEM_REALTIME_DO)
	{
		//ʵʱ����
		if (m_WgtIndex.contains(m_RealDoWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_RealDoWgt);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_RealDoWgt, tr("������"), "RealDo");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_RealDoWgt);
			m_WgtIndex.insert(m_RealDoWgt, TREE_ITEM_REALTIME_DO);

			m_IndexWgt.insert(TREE_ITEM_REALTIME_DO, m_RealDoWgt);
		}
	}
	else if (nType == TREE_ITEM_REALTIME_ACTION)
	{
		//ʵʱaction
		if (m_WgtIndex.contains(m_pActionWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pActionWgt);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pActionWgt, tr("�����ź�"), "RealAction");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pActionWgt);
			m_WgtIndex.insert(m_pActionWgt, TREE_ITEM_REALTIME_ACTION);

			m_IndexWgt.insert(TREE_ITEM_REALTIME_ACTION, m_pActionWgt);
		}
	}
	else if (nType == TREE_ITEM_REALTIME_DISNORMAL)
	{
		//ʵʱabnormal
		if (m_WgtIndex.contains(m_pAbnormalWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pAbnormalWgt);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pAbnormalWgt, tr("�쳣�ź�"), "RealAbnormal");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pAbnormalWgt);
			m_WgtIndex.insert(m_pAbnormalWgt, TREE_ITEM_REALTIME_DISNORMAL);

			m_IndexWgt.insert(TREE_ITEM_REALTIME_DISNORMAL, m_pAbnormalWgt);
		}
	}
	else if (nType == TREE_ITEM_REALTIME_SOFT)
	{
		//ʵʱsoft
		if (m_WgtIndex.contains(m_pRealSoftWgt))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pRealSoftWgt);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pRealSoftWgt, tr("��ѹ��״̬"), "RealSoft");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pRealSoftWgt);
			m_WgtIndex.insert(m_pRealSoftWgt, TREE_ITEM_REALTIME_SOFT);

			m_IndexWgt.insert(TREE_ITEM_REALTIME_SOFT, m_pRealSoftWgt);
		}
	}
	else if (nType == TREE_ITEM_PROTECT_DEV)
	{
		//������ֵ
		if (m_WgtIndex.contains(m_pProtectDev))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pProtectDev);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pProtectDev, tr("������ֵ"), "ProtectDev");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pProtectDev);
			m_WgtIndex.insert(m_pProtectDev, TREE_ITEM_PROTECT_DEV);

			m_IndexWgt.insert(TREE_ITEM_PROTECT_DEV, m_pProtectDev);
		}
	}
	else if (nType == TREE_ITEM_SYSTEM_DEV)
	{
		//ϵͳ����
		if (m_WgtIndex.contains(m_pSysDev))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSysDev);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pSysDev, tr("ϵͳ����"), "SystemDev");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSysDev);
			m_WgtIndex.insert(m_pSysDev, TREE_ITEM_SYSTEM_DEV);

			m_IndexWgt.insert(TREE_ITEM_SYSTEM_DEV, m_pSysDev);
		}
	}
	else if (nType == TREE_ITEM_DI_IFNO)
	{
		//������
		if (m_WgtIndex.contains(m_pDi))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pDi);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pDi, tr("�������"), "Di");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pDi);
			m_WgtIndex.insert(m_pDi, TREE_ITEM_DI_IFNO);

			m_IndexWgt.insert(TREE_ITEM_DI_IFNO, m_pDi);
		}
	}
	else if (nType == TREE_INFO_DO_INFO)
	{
		//������
		if (m_WgtIndex.contains(m_pDo))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pDo);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pDo, tr("��������"), "Do");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pDo);
			m_WgtIndex.insert(m_pDo, TREE_INFO_DO_INFO);

			m_IndexWgt.insert(TREE_INFO_DO_INFO, m_pDo);
		}
	}
	else if (nType == TREE_INFO_SOFT)
	{
		//��ѹ��
		if (m_WgtIndex.contains(m_pSoft))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSoft);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pSoft, tr("��ѹ��Ͷ��"), "Soft");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSoft);
			m_WgtIndex.insert(m_pSoft, TREE_INFO_SOFT);

			m_IndexWgt.insert(TREE_INFO_SOFT, m_pSoft);
		}
	}
	else if (nType == TREE_INFO_SOE)
	{
		//soe
		if (m_WgtIndex.contains(m_pSoe))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSoe);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pSoe, tr("ʵʱ�¼�"), "soe");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSoe);
			m_WgtIndex.insert(m_pSoe, TREE_INFO_SOE);

			m_IndexWgt.insert(TREE_INFO_SOE, m_pSoe);
		}
	}
	else if (nType == TREE_ITEM_HIOSTORY_SOE)
	{
		//soe history
		if (m_WgtIndex.contains(m_pSoeHistory))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSoeHistory);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pSoeHistory, tr("��ʷ�¼�"), "historysoe");
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_pSoeHistory);
			m_WgtIndex.insert(m_pSoeHistory, TREE_ITEM_HIOSTORY_SOE);

			m_IndexWgt.insert(TREE_ITEM_HIOSTORY_SOE, m_pSoeHistory);
		}
	}

}

void CBreakerModule::Slot_CloseOneTabWidget(int iIndex)
{
	QWidget *pCloseWgt = m_pMainWindow->GetTabWidget()->widget(iIndex);

	if (pCloseWgt != NULL)
	{
		int iIndex2 = m_WgtIndex[pCloseWgt];
		m_IndexWgt.remove(iIndex2);
		m_WgtIndex.remove(pCloseWgt);

		m_pMainWindow->GetTabWidget()->removeTab(iIndex);

	}
	
}

//����ʱ��
void CBreakerModule::Slot_SetDeviceTime()
{
	CDataTimeEditWgt *pTimeEdWgt = new CDataTimeEditWgt;

	if (pTimeEdWgt->exec())
	{
		TIME_SET_INFO dbgTimeSet;
		dbgTimeSet.header0 = 0xAA;
		dbgTimeSet.header1 = 0x55;

		dbgTimeSet.msgLeg.SetAddr(8);
		//
		dbgTimeSet.type = DBG_CODE_SET_TIME;

		dbgTimeSet.m_time.year.SetAddr(pTimeEdWgt->GetCurrentTime().date().year());
		dbgTimeSet.m_time.month = pTimeEdWgt->GetCurrentTime().date().month();
		dbgTimeSet.m_time.day = pTimeEdWgt->GetCurrentTime().date().day();

		dbgTimeSet.m_time.hour = pTimeEdWgt->GetCurrentTime().time().hour();
		dbgTimeSet.m_time.minute = pTimeEdWgt->GetCurrentTime().time().minute();
		dbgTimeSet.m_time.ms = pTimeEdWgt->GetCurrentTime().time().second();

		m_pNetManager->GetSender()->OnSendTimeSet(&dbgTimeSet);

	}
	QString strValue = tr("����ʱ��Ϊ:%1").arg(pTimeEdWgt->GetCurrentTime().toString("yyyy-MM-dd hh:mm:ss"));;

	WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);

	pTimeEdWgt->deleteLater();
}

void CBreakerModule::SLot_RemoteControl()
{
	m_nRemoterControl = 0;
	QDialog *pContrlDlg = new QDialog;
	QComboBox *pCombox = new QComboBox;
	pCombox->addItem(tr("��"));
	pCombox->addItem(tr("��"));

	QPushButton *pComform = new QPushButton;
	pComform->setText(tr("ȷ��"));
	connect(pComform,SIGNAL(clicked()),pContrlDlg,SLOT(accept()));
	QHBoxLayout *btnLayout = new QHBoxLayout;
	btnLayout->addStretch();
	btnLayout->addWidget(pComform);

	QVBoxLayout *totalLayout = new QVBoxLayout(pContrlDlg);
	totalLayout->addWidget(pCombox);
	totalLayout->addLayout(btnLayout);

	if (pContrlDlg->exec())
	{
		REMOTE_CONTROM_SET dbgControl;
		dbgControl.header0 = 0xAA;
		dbgControl.header1 = 0x55;

		dbgControl.msgLeg.SetAddr(4);
		dbgControl.type = DBG_CODE_RM_CTRL;

		dbgControl.m_ControlType = 0;
		dbgControl.m_ControlObj = 0;
		dbgControl.m_Order = pCombox->currentIndex();
		m_nRemoteStauts = pCombox->currentIndex();

		m_pNetManager->GetSender()->OnSendRemoteControl(dbgControl);
		QString strValue = tr("����ң��ѡ���");

		WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);


	}
	pContrlDlg->deleteLater();

}

void CBreakerModule::Slot_RemoteContrExec()
{
	if (m_nRemoterControl == 0)
	{
		//ң��ѡ��
		int messReturn = QMessageBox::information(0, tr("�ɹ�"), tr("ң��ѡ��ɹ�,,�Ƿ�ִ��?"), QMessageBox::Yes, QMessageBox::No);
		if (messReturn == QMessageBox::Yes)
		{
			REMOTE_CONTROM_SET dbgControl;
			dbgControl.header0 = 0xAA;
			dbgControl.header1 = 0x55;

			dbgControl.msgLeg.SetAddr(4);
			dbgControl.type = DBG_CODE_RM_CTRL;

			dbgControl.m_ControlType = 1;
			dbgControl.m_ControlObj = 0;
			dbgControl.m_Order = m_nRemoteStauts;

			m_pNetManager->GetSender()->OnSendRemoteControl(dbgControl);
			QString strValue = tr("����ң��ִ�а�");

			WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
		}

		m_nRemoterControl = 1;

	}
}

void CBreakerModule::Slot_ConnectToSocket()
{
	m_pNetManager->GetSocket()->ConnectSocket(m_pNetManager->GetSocket()->GetIpAddress(),m_pNetManager->GetSocket()->GetPort());
}

void CBreakerModule::Slot_DisConnectSocket()
{
	m_pNetManager->GetSocket()->DisConnectSocket();
}

void CBreakerModule::Slot_Help()
{
	QMessageBox::information(0, tr("����"), tr("��ѹֱ����·�����Ƶ������V1.0"));
}

void CBreakerModule::Slot_Reset()
{
	QString strValue = tr("�����źŸ�������");

	WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);

	SendRequest(DBG_CODE_SIGNALRESET);
}

void CBreakerModule::Slot_SoeClear()
{
	m_ClearSoeLst.clear();
	//soe clear
	if (m_pSoeClearWgt->exec())
	{
		//
		if (m_pSoeClearWgt->GetAbnormal())
		{
			m_ClearSoeLst.append(SOE_ABNORMAL);
		}

		if (m_pSoeClearWgt->GetAction())
		{
			m_ClearSoeLst.append(SOE_ACTION);
		}

		if (m_pSoeClearWgt->GetDIDo())
		{
			m_ClearSoeLst.append(SOE_DIEVENT);
		}

		if (m_pSoeClearWgt->GetDebug())
		{
			m_ClearSoeLst.append(SOE_TRACE);
		}

		if (m_pSoeClearWgt->GetRun())
		{
			m_ClearSoeLst.append(SOE_RUN);
		}

		if (m_pSoeClearWgt->GetWarve())
		{
			m_ClearSoeLst.append(SOE_WAVE_FILE);
		}
	}

	if (m_ClearSoeLst.count() != 0)
	{
		//��ʼ��������
		SendSoeClearRequest(m_ClearSoeLst.first());
		QString strValue;
		strValue = GetSoeDestr(m_ClearSoeLst.first());
		m_ClearSoeLst.removeFirst();

		WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);

	}
}

void CBreakerModule::Slot_SoeClearAck()
{
	//��ղ���
	if (m_ClearSoeLst.count() != 0)
	{
		//��ʼ��������
		SendSoeClearRequest(m_ClearSoeLst.first());
		QString strValue;
		strValue = GetSoeDestr(m_ClearSoeLst.first());
		m_ClearSoeLst.removeFirst();

		WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);

	}
}

void CBreakerModule::Slot_LoginManager()
{
	CLogicManager *pLogicManager = new CLogicManager;
	if (pLogicManager->exec())
	{
		//
	}
	pLogicManager->deleteLater();
}

void CBreakerModule::SendSoeClearRequest(int nType)
{
	DEG_SOE_CLEAR dbgHeader;
	dbgHeader.header0 = 0xAA;
	dbgHeader.header1 = 0x55;

	dbgHeader.msgLeg.SetAddr(2);
	//��ȡϵͳ��Ϣ
	dbgHeader.type = DBG_CODE_ClearSOE;
	//
	dbgHeader.m_SoeType = nType;

	m_pNetManager->GetSender()->OnSendSoeClearRequestr(dbgHeader);

}


//��ȡsoe����
QString CBreakerModule::GetSoeDestr(int nType)
{
	QString strDes;

	switch (nType)
	{
	case SOE_ACTION:
	{
		strDes = tr("������ն����¼�����");
		break;
	}
	case SOE_DIEVENT:
	{
		strDes = tr("������ձ�λ�¼�����");
		break;
	}
	case SOE_ABNORMAL:
	{
		strDes = tr("��������쳣�¼�����");
		break;
	}
	case SOE_RUN:
	{
		strDes = tr("������������¼�����");
		break;
	}
	case SOE_WAVE_FILE:
	{
		strDes = tr("�������¼���¼�����");
		break;
	}
	case SOE_TRACE:
	{
		strDes = tr("������յ�����Ϣ����");
		break;
	}
	default:
		break;
	}

	return strDes;

}
