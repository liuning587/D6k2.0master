
#if _MSC_VER >= 1600
#include "vld.h"
#endif

#include "osc_module.h"
#define MODULE_LIB
#include "devicestudio/module_export.h"
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "devicestudio/treewidget.h"

//#include "devicestudio/treeitem_def.h"
#include "devicestudio/tabwidget.h" 

#include "osc_treeitem_def.h"
#include "childfrm.h"
#include "wave_info.h"

#include "screenshot.h"

#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QApplication>
#include <QTextCodec>
#include <QToolButton>
#include <QMessageBox>
 
 
COscillogramModule::COscillogramModule()
{
	m_pToolBar = nullptr;

	m_pOpenAct = nullptr;
	m_pCloseAct = nullptr;
	m_pOpenSpcAct = nullptr;

	m_pSaveAsAct = nullptr;
	m_pPrintAct = nullptr;
	m_pPrintHdrAct = nullptr;
	m_pPrintActionInforAct = nullptr;
	m_pActPdf = nullptr;
	m_pActAIPdf = nullptr;
	m_pActABPdf = nullptr;
	m_pPrintActionBehaviorAct = nullptr;
	m_pPrintPreviewAct = nullptr;
	m_pPrintSetAct = nullptr;
	m_pQuitAct = nullptr;

	m_pToolBarAct = nullptr;
	m_pStatusBarAct = nullptr;
	m_pSeparateAct = nullptr;

	m_pZoomInAct = nullptr;
	m_pZoomOutAct = nullptr;
	m_pRecoverAct = nullptr;
	m_pHStretchAct = nullptr;
	m_pVZoomAct = nullptr;
	m_pCopyCurveAct = nullptr;
	m_pExchangeCurveAct = nullptr;
	m_pAmplitudeAct = nullptr;
	m_pCurveColorAct = nullptr;
}

void COscillogramModule::Init(IMainModule *pMainModule)
{
	Q_ASSERT(pMainModule);
	if (pMainModule == nullptr)
		return;

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

	QString szLog(tr("osc plugin load success!"));
	pMainModule->LogString("OSC", szLog.toStdString().c_str(), 1);

	CreateActions();
	CreateToolBar();
}

COscillogramModule::~COscillogramModule()
{
	Destroy();
}

void COscillogramModule::UnInit()
{

}

void COscillogramModule::Destroy()
{
	if (m_pToolBar)
	{
		delete m_pToolBar;
		m_pToolBar = nullptr;
	}
	
	if (m_pOpenAct)
	{
		delete m_pOpenAct;
		m_pOpenAct = nullptr;
	}

	if (m_pCloseAct)
	{
		delete m_pCloseAct;
		m_pCloseAct = nullptr;
	}

	if (m_pOpenSpcAct)
	{
		delete m_pOpenSpcAct;
		m_pOpenSpcAct = nullptr;
	} 

	if (m_pSaveAsAct)
	{
		delete m_pSaveAsAct;
		m_pSaveAsAct = nullptr;
	}
	 
	if (m_pPrintAct)
	{
		delete m_pPrintAct;
		m_pPrintAct = nullptr;
	}
	 
	if (m_pPrintHdrAct)
	{
		delete m_pPrintHdrAct;
		m_pPrintHdrAct = nullptr;
	}

	if (m_pPrintActionInforAct)
	{
		delete m_pPrintActionInforAct;
		m_pPrintActionInforAct = nullptr;
	}

	if (m_pActPdf)
	{
		delete m_pActPdf;
		m_pActPdf = nullptr;
	}

	if (m_pActAIPdf)
	{
		delete m_pActAIPdf;
		m_pActAIPdf = nullptr;
	}

	if (m_pActABPdf)
	{
		delete m_pActABPdf;
		m_pActABPdf = nullptr;
	}

	if (m_pPrintActionBehaviorAct)
	{
		delete m_pPrintActionBehaviorAct;
		m_pPrintActionBehaviorAct = nullptr;
	}

	if (m_pPrintPreviewAct)
	{
		delete m_pPrintPreviewAct;
		m_pPrintPreviewAct = nullptr;
	}

	if (m_pPrintSetAct)
	{
		delete m_pPrintSetAct;
		m_pPrintSetAct = nullptr;
	}

	if (m_pQuitAct)
	{
		delete m_pQuitAct;
		m_pQuitAct = nullptr;
	} 

	if (m_pToolBarAct)
	{
		delete m_pToolBarAct;
		m_pToolBarAct = nullptr;
	}

	if (m_pStatusBarAct)
	{
		delete m_pStatusBarAct;
		m_pStatusBarAct = nullptr;
	}

	if (m_pSeparateAct)
	{
		delete m_pSeparateAct;
		m_pSeparateAct = nullptr;
	} 

	if (m_pZoomInAct)
	{
		delete m_pZoomInAct;
		m_pZoomInAct = nullptr;
	}

	if (m_pZoomOutAct)
	{
		delete m_pZoomOutAct;
		m_pZoomOutAct = nullptr;
	}

	if (m_pRecoverAct)
	{
		delete m_pRecoverAct;
		m_pRecoverAct = nullptr;
	}

	if (m_pHStretchAct)
	{
		delete m_pHStretchAct;
		m_pHStretchAct = nullptr;
	}

	if (m_pVZoomAct)
	{
		delete m_pVZoomAct;
		m_pVZoomAct = nullptr;
	}

	if (m_pCopyCurveAct)
	{
		delete m_pCopyCurveAct;
		m_pCopyCurveAct = nullptr;
	}

	if (m_pExchangeCurveAct)
	{
		delete m_pExchangeCurveAct;
		m_pExchangeCurveAct = nullptr;
	}
	 
	if (m_pAmplitudeAct)
	{
		delete m_pAmplitudeAct;
		m_pAmplitudeAct = nullptr;
	}

	if (m_pCurveColorAct)
	{
		delete m_pCurveColorAct;
		m_pCurveColorAct = nullptr;
	}

// 	if (m_pToolButton)
// 	{
// 		delete m_pToolButton;
// 		m_pToolButton = nullptr;
// 	}	 

	m_arrWaves.clear();
}

void COscillogramModule::SetLoadDeviceName(const QString &strDeviceName)
{
	std::ignore = strDeviceName;
}

void COscillogramModule::CreateTreeItem()
{
	QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pMainWindow->GetLeftTree()->model();
	QStandardItem *pTopItem = (QStandardItem*)pTreeModel->item(0, 0);

	QStandardItem *pOscRootItem = new QStandardItem(("ComtradeViewer"));
	pTopItem->appendRow(pOscRootItem);
	pOscRootItem->setEditable(false);
	pOscRootItem->setData(TREE_PLUGIN_ROOT, Qt::UserRole);
	
	QStandardItem *pAnaItem = new QStandardItem( tr("Analyze oscillograms list") );
	//pModbusItem->setData(TREE_PLUGIN_ROOT, TREE_LEVEL_DEF);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pAnaItem->setData(TI_COIL, Qt::UserRole);
	pOscRootItem->appendRow(pAnaItem);
	pAnaItem->setEditable(false);

	QStandardItem *pOpenItem = new QStandardItem(tr("Opened oscillograms list"));
	pOscRootItem->appendRow(pOpenItem);

	pOpenItem->setEditable(false);

	QStandardItem *pTestItem = new QStandardItem(tr("xxxxx"));
	pOpenItem->appendRow(pTestItem);

}

void COscillogramModule::Slot_ClickLeftTreeItem(const QModelIndex &qIndex)
{
	int nType = qIndex.data(Qt::UserRole).toInt();

	if (nType == TI_COIL)
	{
		if (m_mapIndexTab.contains(TI_COIL))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_mapIndexTab[TI_COIL]);
		}
		else
		{
// 			CChildFrm *pChildFrm = new CChildFrm( m_pMainWindow );
// 			m_pMainWindow->GetTabWidget()->AddTab(pChildFrm, tr("Wave"), "Wave");
// 			m_mapIndexTab.insert(TI_COIL, pChildFrm);
// 			m_mapTabIndex.insert(pChildFrm, TI_COIL);
			//遥信实时数据
			// 			QWidget *testWgt = new QWidget;
			// 			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥信实时数据"), tr("yxsssj"));
			// 			m_MIndexTab.insert(TREE_REMOTE_SIGNALITEM, testWgt);
			// 			m_MTabIndex.insert(testWgt, TREE_REMOTE_SIGNALITEM);
		}

	}
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

void COscillogramModule::Slot_CloseOneTabWidget(int iIndex)
{
	//
	Q_ASSERT(m_pMainWindow);
	if (m_pMainWindow == nullptr)
		return;

	QWidget *pCloseWgt = m_pMainWindow->GetTabWidget()->widget(iIndex);
	
	if (pCloseWgt != nullptr)
	{
		int nTabIndex = m_mapTabIndex[pCloseWgt];
		m_mapIndexTab.remove(nTabIndex);
		m_mapTabIndex.remove(pCloseWgt);
		m_pMainWindow->GetTabWidget()->removeTab(iIndex);
	}
}

bool COscillogramModule::CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc)
{
	Q_ASSERT(m_pMainModule);
	if (m_pMainModule == nullptr)
		return false;
	return m_pMainModule->CommMsgLog(pszModuleName, pMsg, nMsgLen,nMsgType,pMsgSubType,pSource,pDesc);
}

bool COscillogramModule::LogString(const char *pszMoudleName,const char *szLogTxt, int nLevel)
{
	Q_ASSERT(m_pMainModule);
	if (m_pMainModule == nullptr)
		return false;
	return m_pMainModule->LogString(pszMoudleName, szLogTxt, nLevel);
}

void COscillogramModule::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pMainWindow->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(TREE_LEVEL_DEF).toInt();

//	if (nType == TREE_PLUGIN_ROOT)
	{
		ShowMenu(indexSelect);
	}
}

void COscillogramModule::ShowMenu(QModelIndex &index)
{
	int nRet = index.data(TREE_LEVEL_DEF).toInt();

//	if (nRet == TREE_PLUGIN_ROOT)
	{
		ShowRootMenu(index);
	}
}

void COscillogramModule::ShowRootMenu(QModelIndex &index)
{
	Q_UNUSED(index);
	QMenu *pMenu = new QMenu(nullptr);

	QAction *pActOpen = new QAction(tr("Open"), pMenu);
	// 	pActionGraph->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pActOpen);

// 	QAction *pActionStop = new QAction(tr("Stop"), pMenu);
// 	pMenu->addAction(pActionStop);


	QAction* pSelItem = pMenu->exec(QCursor::pos());

	if (pSelItem == pActOpen)
	{
		QString szPathName;

		QString szFileName = QFileDialog::getOpenFileName(m_pMainWindow, tr("Open File"), szPathName, tr("Comtrade Data Files (*.dat)"));
		if (!szFileName.isEmpty())
		{
			if (m_pMainWindow)
			{
				m_pMainWindow->setFocus();

				OpenDataFile(szFileName);
			}
		}
	}
	 
	pMenu->deleteLater();
}


void COscillogramModule::CreateActions()
{
	Q_ASSERT(m_pMainWindow);

	m_pOpenAct = new QAction(QIcon(":/images/open2.png"), tr("&Open..."), m_pMainWindow);
	m_pOpenAct->setShortcut(tr("Ctrl+O"));
	m_pOpenAct->setStatusTip(tr("Open wave data file"));

	m_pOpenSpcAct = new QAction(QIcon(":/images/open2.png"), tr("Open Select..."), this);
	m_pOpenSpcAct->setShortcut(tr("Ctrl+L"));
	m_pOpenSpcAct->setStatusTip(tr("Open wave data file"));
	//add new connect here
	connect(m_pOpenAct, SIGNAL(triggered()), this, SLOT(SlotOpenFile()));
//	connect(m_pOpenSpcAct, SIGNAL(triggered()), this, SLOT(openSpc()));


	m_pCloseAct = new QAction(QIcon(":/images/close.png"), tr("Clo&se..."), m_pMainWindow);
	m_pCloseAct->setShortcut(tr("Ctrl+S"));
	m_pCloseAct->setStatusTip(tr("Close wave data file"));
 	connect(m_pCloseAct, SIGNAL(triggered()), this, SLOT(SlotCloseCurFile()));
	//add new connect here


	m_pSaveAsAct = new QAction(QIcon(":/images/save.png"), tr("Save&As..."), this);
	m_pSaveAsAct->setShortcut(tr("Ctrl+A"));
	m_pSaveAsAct->setStatusTip(tr("Save as the current file"));
//	connect(m_pSaveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	m_pPrintAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
	m_pPrintAct->setShortcut(tr("Ctrl+P"));
	m_pPrintAct->setStatusTip(tr("Print the current file"));
//	connect(m_pPrintAct, SIGNAL(triggered()), this, SLOT(printSlot()));

	m_pPrintHdrAct = new QAction(QIcon(":/images/print.png"), tr("Print &HDR..."), this);
	m_pPrintHdrAct->setShortcut(tr("Ctrl+H"));
	m_pPrintHdrAct->setStatusTip(tr("Print the current hdr file"));
//	connect(printHdrAct, SIGNAL(triggered()), this, SLOT(printHdrSlot()));

	m_pPrintActionInforAct = new QAction(QIcon(":/images/print.png"), tr("Print &ActionInfor..."), this);
	m_pPrintActionInforAct->setStatusTip(tr("Print the current Action Infor"));
//	connect(printActionInforAct, SIGNAL(triggered()), this, SLOT(printActionInfor()));

	m_pActPdf = new QAction(QIcon(":/images/pdfprint.png"), tr("Print Pdf..."), this);
	m_pActPdf->setStatusTip(tr("Print to pdf"));
//	connect(actPdf, SIGNAL(triggered()), this, SLOT(printPdf()));

	m_pActAIPdf = new QAction(QIcon(":/images/pdfprint.png"), tr("Print AI Pdf..."), this);
	m_pActAIPdf->setStatusTip(tr("Print the current Action Infor with pdf"));
//	connect(actAIPdf, SIGNAL(triggered()), this, SLOT(printAIPdf()));

	m_pActABPdf = new QAction(QIcon(":/images/pdfprint.png"), tr("Print AB Pdf..."), this);
	m_pActABPdf->setStatusTip(tr("Print the current Action Behavior with pdf"));
//	connect(actABPdf, SIGNAL(triggered()), this, SLOT(printABPdf()));

	m_pPrintActionBehaviorAct = new QAction(QIcon(":/images/print.png"), tr("Print &ActionBehavior..."), this);
	m_pPrintActionBehaviorAct->setStatusTip(tr("Print the current Action Behavior"));
//	connect(printActionBehaviorAct, SIGNAL(triggered()), this, SLOT(printActionBehavior()));

	m_pPrintPreviewAct = new QAction(QIcon(":/images/printpreview.png"), tr("PrintPre&view..."), this);
	m_pPrintPreviewAct->setEnabled(false);
	m_pPrintPreviewAct->setShortcut(tr("Ctrl+V"));
	m_pPrintPreviewAct->setStatusTip(tr("PrintPreview the current file"));
//	connect(printPreviewAct, SIGNAL(triggered()), this, SLOT(printPreviewSlot()));
	//add new connect here

	m_pPrintSetAct = new QAction(QIcon(":/images/printset.png"), tr("PrintSet..."), this);
	m_pPrintSetAct->setShortcut(tr("Ctrl+E"));
	m_pPrintSetAct->setStatusTip(tr("Set attributes for printing"));
//	connect(printSetAct, SIGNAL(triggered()), this, SLOT(setPrinterSlot()));

	m_pQuitAct = new QAction(tr("&Quit"), this);
	m_pQuitAct->setShortcut(tr("Ctrl+Q"));
	m_pQuitAct->setStatusTip(tr("Quit the application"));
	connect(m_pQuitAct, SIGNAL(triggered()), this, SLOT(close()));
	/********************/

	m_pToolBarAct = new QAction(tr("&ToolBar"), this);
	m_pToolBarAct->setCheckable(true);
	m_pToolBarAct->setShortcut(tr("Ctrl+T"));
	m_pToolBarAct->setStatusTip(tr("The toolbar"));
//	connect(m_pToolBarAct, SIGNAL(triggered()), this, SLOT(toolBarSlot()));

// 	statusBarAct = new QAction(tr("Status&Bar"), this);
// 	statusBarAct->setCheckable(true);
// 	statusBarAct->setShortcut(tr("Ctrl+B"));
// 	statusBarAct->setStatusTip(tr("The status bar"));
// 	connect(statusBarAct, SIGNAL(triggered()), this, SLOT(statusBarSlot()));

	m_pSeparateAct = new QAction(tr("se&parate"), this);
	//separateAct->setCheckable(true);
	m_pSeparateAct->setShortcut(tr("Ctrl+P"));
	m_pSeparateAct->setStatusTip(tr("To separate left or right part of the main window"));
//	connect(separateAct, SIGNAL(triggered()), this, SLOT(separateSlot()));

	/**************************/
	m_pZoomInAct = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom In Curve"), this);
//	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(setZoomInSlot()));
	m_pZoomOutAct = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom Out Curve"), this);
//	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(setZoomOutSlot()));
	m_pRecoverAct = new QAction(QIcon(":/images/recover.png"), tr("Recover Curve"), this);
//	connect(recoverAct, SIGNAL(triggered()), this, SLOT(resetZoomSlot()));

	m_pHStretchAct = new QAction(QIcon(":/images/stretch.png"), tr("Horizontally Stretch"), this);
//	connect(hStretchAct, SIGNAL(triggered()), this, SLOT(stretchCurveSlot()));
	m_pVZoomAct = new QAction(QIcon(":/images/zoom.png"), tr("Vertically Zoom"), this);
//	connect(vZoomAct, SIGNAL(triggered()), this, SLOT(vstretCurveSlot()));
	//copyCurveAct=new QAction(QIcon(":/images/copy.png"),tr("Copy Curve"),this);
	m_pCurveCascadeAct = new QAction(QIcon(":/images/cascade.png"), tr("Cascade Curves"), this);
//	connect(curveCascadeAct, SIGNAL(triggered()), this, SLOT(cascadeCurveSlot()));

	m_pExchangeCurveAct = new QAction(QIcon(":/images/exchange.png"), tr("Exchange Curve"), this);
//	connect(exchangeCurveAct, SIGNAL(triggered()), this, SLOT(exchangeCurveSlot()));

	m_pAmplitudeAct = new QAction(QIcon(":/images/amplitude.png"), tr("Amplitude Measurement"), this);
//	connect(amplitudeAct, SIGNAL(triggered()), this, SLOT(amplitudeSlot()));

	m_pCurveColorAct = new QAction(QIcon(":/images/color.png"), tr("Curve Color"), this);
//	connect(curveColorAct, SIGNAL(triggered()), this, SLOT(curveColorSlot()));
	/************************/
	m_pVectorAnalyseAct = new QAction(QIcon(":/images/vector.png"), tr("Vector Analyse"), this);
//	connect(vectorAnalyseAct, SIGNAL(triggered()), this, SLOT(vectorFormSlot()));
	m_pHarmonicAnalyseAct = new QAction(QIcon(":/images/harmonic.png"), tr("Harmonic Vector Analyse"), this);
//	connect(harmonicAnalyseAct, SIGNAL(triggered()), this, SLOT(harmonicFormSlot()));
//	sequenceAnalyseAct = new QAction(QIcon(":/images/sequence.png"), tr("Sequence Analyse"), this);
//	connect(sequenceAnalyseAct, SIGNAL(triggered()), this, SLOT(sequenceFormSlot()));

	m_pTelemeterAct = new QAction(QIcon(":/images/measurement.png"), tr("Telemeter Analyse"), this);/***未实现*************/
//	connect(telemeterAct, SIGNAL(triggered()), this, SLOT(telemeterSlot()));
	m_pTelemeterAct->setEnabled(false);
	m_pCurveAct = new QAction(QIcon(":/images/curve.png"), tr("Curve Analyse"), this);/*******未实现*********/
//	connect(curveAct, SIGNAL(triggered()), this, SLOT(curveSlot()));
	m_pCurveAct->setEnabled(false);
	m_pViewFileParamAct = new QAction(QIcon(":/images/attribute.png"), tr("View File Parameters"), this);
//	connect(viewFileParamAct, SIGNAL(triggered()), this, SLOT(attributeFormSlot()));
	m_pViewChannelParamAct = new QAction(QIcon(":/images/channel.png"), tr("View Channel Parameters"), this);
//	connect(viewChannelParamAct, SIGNAL(triggered()), this, SLOT(channelFormSlot()));

	m_pCommunicationAct = new QAction(QIcon(":/images/communication.png"), tr("Screen shot"), this); /*******未实现*********/
	connect(m_pCommunicationAct, SIGNAL(triggered()), this, SLOT(SlotScreenShot()));
	//m_pCommunicationAct->setEnabled(false);
	/***********************/
//	reLogonAct = new QAction(tr("ReLogin"), this);
//	connect(reLogonAct, SIGNAL(triggered()), this, SLOT(reLogSlot()));
//	manageUserAct = new QAction(QIcon(":/images/usermanage.png"), tr("Manage Users"), this);
//	connect(manageUserAct, SIGNAL(triggered()), this, SLOT(userManageSlot()));

	/***************************/
	m_pUsageAct = new QAction(QIcon(":/images/help.png"), tr("Usage"), this);
	m_pAboutAct = new QAction(tr("&About"), this);
	m_pAboutAct->setStatusTip(tr("Show the application's About box"));
	connect(m_pAboutAct, SIGNAL(triggered()), this, SLOT(SlotAbout()));
}

void COscillogramModule::CreateToolBar()
{
	Q_ASSERT(m_pMainWindow);
	if (m_pMainWindow)
	{
		m_pToolBar = m_pMainWindow->addToolBar(tr("File(F)"));
		m_pToolButton = new QToolButton;
		m_pToolButton->setPopupMode(QToolButton::MenuButtonPopup);

		QMenu *pToolBtnMenu = new QMenu();
 		m_pToolButton->setMenu(pToolBtnMenu);
		pToolBtnMenu->deleteLater();
	 
 		m_pToolButton->addAction(m_pOpenAct);
		m_pToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		m_pToolBar->addWidget(m_pToolButton);

		m_pToolButton->setDefaultAction(m_pOpenAct);
 
		m_pToolBar->addAction(m_pCloseAct);
		m_pToolBar->addSeparator();

		m_pToolBar->addAction(m_pExchangeCurveAct);
		m_pToolBar->addAction(m_pCurveCascadeAct);
		m_pToolBar->addAction(m_pHStretchAct);
		m_pToolBar->addAction(m_pVZoomAct);
		m_pToolBar->addAction(m_pZoomInAct);
		m_pToolBar->addAction(m_pZoomOutAct);
		m_pToolBar->addAction(m_pRecoverAct);
		m_pToolBar->addSeparator();

		m_pToolBar->addAction(m_pCurveColorAct);
		m_pToolBar->addAction(m_pAmplitudeAct);
		m_pToolBar->addSeparator();

		m_pToolBar->addAction(m_pVectorAnalyseAct);
		m_pToolBar->addAction(m_pHarmonicAnalyseAct);
//		m_pToolBar->addAction(m_pSequenceAnalyseAct);
		m_pToolBar->addAction(m_pTelemeterAct);
		m_pToolBar->addAction(m_pCurveAct);
		m_pToolBar->addSeparator();

		m_pToolBar->addAction(m_pViewFileParamAct);
		m_pToolBar->addAction(m_pViewChannelParamAct);
		m_pToolBar->addSeparator();

		m_pToolBar->addAction(m_pCommunicationAct);
		m_pToolBar->addSeparator();

		m_pToolBar->addAction(m_pPrintPreviewAct);

		//fileToolBar->addAction(printAct);
		QMenu* pMenu2 = new QMenu();
		//menu->addAction(printHdrAct);
		pMenu2->addAction(m_pActAIPdf);
		pMenu2->addAction(m_pActABPdf);
		pMenu2->addSeparator();
		pMenu2->addAction(m_pPrintAct);
		pMenu2->addAction(m_pPrintActionInforAct);
		pMenu2->addAction(m_pPrintActionBehaviorAct);
		pMenu2->deleteLater();

		QToolButton* tbtn = new QToolButton;
		tbtn->setPopupMode(QToolButton::MenuButtonPopup);
		tbtn->setMenu(pMenu2);
		tbtn->addAction(m_pOpenAct);
		tbtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		m_pToolBar->addWidget(tbtn);
		tbtn->setDefaultAction(m_pActPdf);
// 
		m_pToolBar->addAction(m_pUsageAct);
		//editToolBar = addToolBar(tr("Edit"));
		//editToolBar->addAction(undoAct);

	}
}
/*! \fn void COscillogramModule::SlotOpenFile()
********************************************************************************************************* 
** \brief COscillogramModule::SlotOpenFile 
** \details 打开文件的槽函数
** \return void 
** \author LiJin 
** \date 2018年1月31日 
** \note 
********************************************************************************************************/
void COscillogramModule::SlotOpenFile()
{
	QString szPathName;

	QString szFileName = QFileDialog::getOpenFileName(m_pMainWindow, tr("Open File"), szPathName, tr("Comtrade Data Files (*.dat)"));
	if (!szFileName.isEmpty())
	{
		if (m_pMainWindow)
		{
			m_pMainWindow->setFocus();		
		}
	//	OpenDataFile(szFileName);
		std::shared_ptr<CComtrade> pWave = std::make_shared<CComtrade>();
		bool bRet = pWave->OpenFileGroup(szFileName);
		if (bRet)
		{
			m_arrWaves.emplace_back(pWave);

			CChildFrm *pChildFrm = new CChildFrm(pWave, m_pMainWindow);
			QString szTitle;
			if (pWave->GetComtradeName().isEmpty() ==true)
			{
				szTitle = tr("Wave %1").arg(m_arrWaves.size());
			}
			else
			{
				szTitle = pWave->GetComtradeName();
			}
			m_pMainWindow->GetTabWidget()->AddTab(pChildFrm, szTitle, "Wave");
			m_mapIndexTab.insert(TI_COIL, pChildFrm);
			m_mapTabIndex.insert(pChildFrm, TI_COIL);
		}
	}
}
/*! \fn void COscillogramModule::OpenDataFile(const QString &szFileName)
********************************************************************************************************* 
** \brief COscillogramModule::OpenDataFile 
** \details 打开数据文件
** \param szFileName 
** \return void 
** \author LiJin 
** \date 2018年1月31日 
** \note 
********************************************************************************************************/
void COscillogramModule::OpenDataFile(const QString &szFileName)
{
	QString szLog;
	Q_ASSERT(szFileName.length() > 3);
	if (szFileName.length() < 3)
	{
		szLog = tr("Open file error! The filename is invalid. name = [%1] ").arg(szFileName);
		LogString("OSC", szLog.toStdString().c_str(), 0);
		return;
	}

	QString szCfgFileName = szFileName;
	szCfgFileName.replace(szFileName.size() - 3, 3, "cfg");

	QFile cfgfile(szCfgFileName);
	// cfg文件不存在
	if (cfgfile.exists() == false)
	{
		szLog = tr("Cannot find corresponding .cfg. file= [%1] ").arg(szCfgFileName);
		LogString("OSC", szLog.toStdString().c_str(), 0);

		cfgfile.close();
		return;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	// 打开不成功
	if (cfgfile.open(QFile::ReadOnly | QFile::Text) == false)
	{
		QApplication::restoreOverrideCursor();
		szLog = tr("Cannot find corresponding .cfg. file= [%1] ").arg(szCfgFileName);
		LogString("OSC", szLog.toStdString().c_str(), 0);

		cfgfile.close();
		return;
	}

	cfgfile.close();

	m_lstOpenedFileList.push_back(szFileName);

	QApplication::restoreOverrideCursor();
}

void COscillogramModule::LoadDataFile(const QString &szFileName)
{
	QString szCfgFileName = szFileName;

	szCfgFileName.replace(szCfgFileName.size() - 3, 3, "cfg");

	QFile cfgfile(szCfgFileName);

	if (cfgfile.exists() == false)
	{
		cfgfile.close();
		return;
	}

	if (cfgfile.open(QFile::ReadOnly | QFile::Text) == false)
	{

		return;
	}

	QTextStream cfgstream(&cfgfile);

	QTextCodec *pCodec = QTextCodec::codecForName("gbk");
	cfgstream.setCodec(pCodec);

	while (!cfgstream.atEnd())
	{
		QString line_temp = cfgstream.readLine().trimmed();
		//if ((0 == line_temp.compare("ASCII")) || (0 == line_temp.compare("ascii")))
		//	m_FileType = ASCII;
		//else if ((0 == line_temp.compare("BINARY")) || (0 == line_temp.compare("binary")))
		//	m_FileType = BINARY;
	}
	cfgfile.close();
}
void COscillogramModule::SlotCloseCurFile()
{

}

void COscillogramModule::SlotScreenShot()
{
	CScreenshot::Instance();
}

void COscillogramModule::SlotAbout()
{
	QMessageBox::about(nullptr, tr("About Record Wave Analysis System"), tr("All Rights Reserved by LiJin"));
}
