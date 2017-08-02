/*! @file
<PRE>
********************************************************************************
模块名       :  标准对话框
文件名       :  channelview.cpp
文件实现功能 :  标准对话框
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 通道 装置
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author  xingzhibing
*  @version 1.0
*  @date    2015.11.17
*/ 

#include "mainfrm.h"
#include "graph_file.h"
#include "realtime_scene.h"
#include "realtime_view.h"
#include "realtime_graph.h"
#include "colour_define.h"
#include "scadaapi/scdsvcapi.h"
#include "log/log.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QDockWidget>
#include <QToolBar>
#include <QPushButton>
#include <QMessageBox>
#include <QLibrary>

typedef QWidget *(*predict_widget)();
typedef void(*predict_close)();

CMainFrame::CMainFrame(QWidget *pParent) :  QMainWindow(pParent)
{ 
	m_pTabWidget = new QTabWidget(nullptr);
	
	m_pDockWidget = new QDockWidget(nullptr);
	m_pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea |Qt::BottomDockWidgetArea);
	
	m_pTreeWidget = new QTreeWidget(m_pDockWidget);
	m_pLeftTabWidget = new QTabWidget;
	m_pPredictTreeWidget = new QTreeWidget;

	m_pLeftTabWidget->setTabPosition(QTabWidget::South);
	m_pLeftTabWidget->addTab(m_pTreeWidget, tr("View"));
	m_pLeftTabWidget->addTab(m_pPredictTreeWidget, tr("Predict"));

	m_pDockWidget->setWidget(m_pLeftTabWidget);
	
	InitToolBar();
	
	InitMainWindow();

	QObject::connect(m_pTreeWidget,&QTreeWidget::itemDoubleClicked,this,&CMainFrame::slot_OnTreeDbClicked);
	QObject::connect(m_pPredictTreeWidget, &QTreeWidget::itemDoubleClicked, this, &CMainFrame::slot_OnPredictTreeDbClicked);

	
}

bool CMainFrame::LoadAppDlls()
{
  	QLibrary libs("predictwgt");
	if (libs.load())
	{
		predict_widget nPred = (predict_widget)libs.resolve("GetMainWgt");
		Q_ASSERT(nPred);
		m_funPrecictMainWidget = nPred;

		predict_close nClose = (predict_close)libs.resolve("DestroyWgt");
		Q_ASSERT(nClose);
		m_funcPredictCloseWindow = nClose;
	}
	else 
	{
		QMessageBox::warning(this, tr("load predict dll error"), libs.errorString());

		return true;
	}

	/*pWidget = m_funPrecictMainWidget();
	Q_ASSERT(pWidget);

	m_pTabWidget->addTab(pWidget, tr("predict"));*/
	
	return true;   
}
/*! \fn CGraphTabView::~CGraphTabView()
********************************************************************************************************* 
** \brief CGraphTabView::~CGraphTabView
** \details 
** \return  
** \author gw
** \date 2015年11月18日 
** \note 
********************************************************************************************************/
CMainFrame::~CMainFrame()
{ 	
//	this->deleteLater();
} 
/*! \fn void CGraphTabView::closeEvent( QCloseEvent * event )
********************************************************************************************************* 
** \brief CGraphTabView::closeEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月20日 
** \note 
********************************************************************************************************/
void CMainFrame::closeEvent(QCloseEvent * event)
{	
	Q_UNUSED(event);
	for (auto iter:m_arrTasks)
	{
		iter->Shutdown();
		delete iter;
		iter = nullptr;
	}
	ShutDown();
}

bool CMainFrame::Initalize(std::vector < SIM_GRAPH_DEF >& arrFiles)
{
	Q_ASSERT(!arrFiles.empty());

	::ConnectScada(GetProjPath().toStdString().c_str(), "RT_GRAPH",1);

	::CreateLog("RT_GRAPH");

	m_arrFiles = arrFiles;

	if (arrFiles.empty())
	{
		return false;
	}
	else
	{
		InitTreeWidget();
	}

	InitPredictTreeView();

	return true;
}

/*! \fn void CGraphTabView::OnDelete()
********************************************************************************************************* 
** \brief CGraphTabView::OnDelete 
** \details 删除
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CMainFrame::OnDelete()
{

}


void CMainFrame::InitToolBar()
{
	QToolBar * pToolBar = addToolBar("toolBar");

	m_pShowViewAction = new QAction(tr("ShowView"));
	m_pShowViewAction->setToolTip(tr("ShowView"));
	m_pShowViewAction->setIcon(QIcon(":/images/eye_on.png"));
	connect(m_pShowViewAction,SIGNAL(triggered()),this,SLOT(slot_OnShowView()));
	pToolBar->addAction(m_pShowViewAction);

	m_pHideViewAction = new QAction(tr("HideView"));
	m_pHideViewAction->setToolTip(tr("HideView"));
	m_pHideViewAction->setIcon(QIcon(":/images/eye_off.png"));
	connect(m_pHideViewAction, SIGNAL(triggered()), this, SLOT(slot_OnHideView()));
	pToolBar->addAction(m_pHideViewAction);

	m_pFullScreenAction = new QAction(tr("FullScreen"));
	m_pFullScreenAction->setToolTip(tr("FullScreen"));
	m_pFullScreenAction->setIcon(QIcon(":/images/fullscreen.png"));
	connect(m_pFullScreenAction, SIGNAL(triggered()), this, SLOT(slot_FullScreenView()));
	pToolBar->addAction(m_pFullScreenAction);

	m_pNomalScreenAction = new QAction(tr("NomalScreen"));
	m_pNomalScreenAction->setToolTip(tr("NomalScreen"));
	m_pNomalScreenAction->setIcon(QIcon(":/images/normalscreen.png"));
	connect(m_pNomalScreenAction, SIGNAL(triggered()), this, SLOT(slot_NormalScreenView()));	
	pToolBar->addAction(m_pNomalScreenAction);

}

void CMainFrame::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
	/*if (m_pTabWidget)
	{
		if (m_pTabWidget->currentWidget())
		{
			QSize nSize = m_pTabWidget->currentWidget()->sizeHint();

			CRealTimeView * pView = reinterpret_cast<CRealTimeView*>(m_pTabWidget->currentWidget());

			pView->scene()->setSceneRect(pView->scene()->sceneRect().left(), pView->scene()->sceneRect().top(),nSize.width(),nSize.height());
		}		
	}	*/
	//m_pGraphScene->setSceneRect(0, 0, this->width(), this->height());
}
/*! \fn void  CMainFrame::AddView(CGraphFile *pGraphFile)
********************************************************************************************************* 
** \brief CMainFrame::AddView 
** \details 打开视图
** \param pGraphFile 
** \return void 
** \author LiJin 
** \date 2017年2月9日 
** \note 
********************************************************************************************************/
void  CMainFrame::AddView(CGraphFile * pGraphFile)
{
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	CRealTimeView *pView = new CRealTimeView(pGraphFile->GetScene(), this);

	pView->setScene(pGraphFile->GetScene());

	CRealTimeTask * pTask = new CRealTimeTask(pGraphFile);

	m_arrTasks.push_back(pTask);

	pTask->Initialize(pGraphFile, 0);

	pTask->Run();

	pGraphFile->GetScene()->setSceneRect(pGraphFile->GetBackgroundRect());

	m_pTabWidget->addTab(pView, QString::fromLocal8Bit(pGraphFile->m_szGraphName.c_str()));

}

void CMainFrame::InitTreeWidget()
{
	m_pTreeWidget->setColumnCount(1);
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem(m_pTreeWidget);
	pRootItem->setText(0, "root");
	m_pTreeWidget->addTopLevelItem(pRootItem);

	for (auto iter:m_arrFiles)
	{
		auto pItem = new QTreeWidgetItem(pRootItem);
		pItem->setText(0,QString::fromStdString(iter->szFileName));
		pItem->setData(0, USER_FILE_ROLE, QString::fromStdString(iter->szFilePath));
		pRootItem->addChild(pItem);
	}
}


void CMainFrame::InitPredictTreeView()
{
	m_pPredictTreeWidget->setColumnCount(1);
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setText(0, tr("PowerPredict"));
	m_pPredictTreeWidget->addTopLevelItem(pRootItem);

	//预测
	QTreeWidgetItem* pPretItem = new QTreeWidgetItem();
	pPretItem->setText(0, tr("PredictFunc"));
	pRootItem->addChild(pPretItem);

	QTreeWidgetItem* pUltraShortPretItem = new QTreeWidgetItem();
	pUltraShortPretItem->setText(0, tr("UltraShortPret"));
	pUltraShortPretItem->setData(0, Qt::UserRole, PREDICT_Ultra_SHORT_CURVE);
	pPretItem->addChild(pUltraShortPretItem);

	QTreeWidgetItem* pShortPretItem = new QTreeWidgetItem();
	pShortPretItem->setText(0, tr("ShortPre"));
	pShortPretItem->setData(0, Qt::UserRole, PREDICT_SHORT_CURVE);
	pPretItem->addChild(pShortPretItem);

	//历史曲线
	QTreeWidgetItem* pHisCurveItem = new QTreeWidgetItem();
	pHisCurveItem->setText(0, tr("HisCurveFunc"));
	pRootItem->addChild(pHisCurveItem);

	QTreeWidgetItem* ppHisCurveItem = new QTreeWidgetItem();
	ppHisCurveItem->setText(0, tr("HisCurve"));
	ppHisCurveItem->setData(0, Qt::UserRole, PREDICT_HIS_CURVE);
	pHisCurveItem->addChild(ppHisCurveItem);

	//设置部分
	QTreeWidgetItem* pSetItem = new QTreeWidgetItem();
	pSetItem->setText(0, tr("PredictSettingFunc"));
	pRootItem->addChild(pSetItem);

	QTreeWidgetItem* pLimitPower = new QTreeWidgetItem();
	pLimitPower->setText(0, tr("LimitPower"));
	pLimitPower->setData(0, Qt::UserRole, PREDICT_LimitPower);
	pSetItem->addChild(pLimitPower);

	QTreeWidgetItem* pStatics = new QTreeWidgetItem();
	pStatics->setText(0, tr("STATICS"));
	pStatics->setData(0, Qt::UserRole, PREDICT_STATICS);
	pSetItem->addChild(pStatics);

	QTreeWidgetItem* pStaticsVariance = new QTreeWidgetItem();
	pStaticsVariance->setText(0, tr("StaticsVariance"));
	pStaticsVariance->setData(0, Qt::UserRole, PREDICT_VARIANCE);
	pSetItem->addChild(pStaticsVariance);
}

void CMainFrame::Run()
{

}

void CMainFrame::ShutDown()
{
	::DisconnectScada("","");
	::DestroyLog("RT_GRAPH");
}


void CMainFrame::slot_OnTreeDbClicked(QTreeWidgetItem *item, int column)
{
	Q_ASSERT(item && column >= 0);
	if (!item || column < 0 )
	{
		return;
	}
	auto pFile = std::make_shared<GraphFileDataUnit>();
	pFile->szFileName = item->text(0).toStdString();
	pFile->szFilePath = item->data(0, USER_FILE_ROLE).toString().toStdString();
	 
	SetCurrentFile(QString::fromStdString(pFile->szFilePath));
}

void CMainFrame::slot_OnPredictTreeDbClicked(QTreeWidgetItem *item, int column)
{
	Q_ASSERT(item);
	if (!item)
	{
		return;    
	}

	int nType = item->data(0, Qt::UserRole).toInt();

	QString szName = NULL;

	QWidget* pWidget = nullptr;
	 
	switch (nType)
	{
	case PREDICT_SHORT_CURVE: 
		break;
	case PREDICT_Ultra_SHORT_CURVE:
		break;
	case PREDICT_HIS_CURVE:
		break;
	case PREDICT_LimitPower:
		break;
	case PREDICT_STATICS:
		break;
	case PREDICT_VARIANCE:
		break;
	default:
		break;
	}

}

void CMainFrame::slot_OnShowView()
{
	if (!m_pDockWidget->isVisible())
	{
		m_pDockWidget->setVisible(true);
	}
}

void CMainFrame::slot_OnHideView()
{
	if (m_pDockWidget->isVisible())
	{
		m_pDockWidget->setVisible(false);
	}
}

void CMainFrame::slot_FullScreenView()
{
	this->showFullScreen();
}

void CMainFrame::slot_NormalScreenView()
{
	this->showNormal();
}

void CMainFrame::InitMainWindow()
{
	QSplitter* pSplitter = new QSplitter(Qt::Horizontal);
	//pSplitter->addWidget(m_pDockWidget);
	pSplitter->addWidget(m_pTabWidget);

	addDockWidget(Qt::LeftDockWidgetArea,m_pDockWidget);

	setCentralWidget(pSplitter);
}

void CMainFrame::SetCurrentFile(const QString& szName)
{
	Q_ASSERT(!szName.isEmpty() && szName.length()>0);
	if (szName.isEmpty() || szName.length()==0)
	{
		return;
	}

	int nCount = 0;
	for (int i = 0; i < m_pTabWidget->count();++i)
	{
		QString szGetName = m_pTabWidget->tabText(i);
		
		if (szGetName ==szName)
		{
			m_pTabWidget->setCurrentIndex(i);

			return;
		}
	}

	CGraphFile* pGraphFile = new CGraphFile();

	pGraphFile->LoadFile(szName);

	if (pGraphFile->GetScene()==nullptr)
	{
		m_pCurScene = new CGraphScene(pGraphFile,nullptr);
		pGraphFile->SetScene(m_pCurScene);
	}
	else
	{
		m_pCurScene = pGraphFile->GetScene();
		m_pCurScene->setParent(this);
	}
	m_pGraphFile = pGraphFile;

	AddView(pGraphFile);
}
