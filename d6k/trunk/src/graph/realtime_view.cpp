/*! @file graphview.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  graphview.cpp
文件实现功能 :  图形编辑
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形编辑
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "realtime_view.h" 
#include "graphscene.h"
#include "widget_factory.h"
#include "line_widget.h"
#include "shape_widget.h"
#include "group_widget.h"
#include "background.h"
#include "graph_file.h"
#include "graphics_layer.h" 
#include "pushbtn_widget.h"
#include "axisframe.h" 

#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>
#include <QScrollBar>
#include <QCoreApplication>
#include <QDragEnterEvent>
#include <QMimeData> 

CRealTimeView::CRealTimeView(QWidget *parent) :
QGraphicsView(parent)
{
 	//setStyleSheet("background: transparent;border:0px");
 	setWindowFlags(Qt::FramelessWindowHint);	
	m_nLayerIndex = 1;
	//this->setViewportMargins(13, 13, 0, 0);
}

CRealTimeView::CRealTimeView(QGraphicsScene *pScene, QWidget * parent)
	:QGraphicsView(pScene, parent)
{
    this->setViewportMargins(13, 13, 0, 0);

    setCacheMode(CacheBackground);
	setInteractive(true);
	setRenderHint(QPainter::Antialiasing);
	setMouseTracking(true);
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

 	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
 	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

 	setStyleSheet("background: transparent;border:0px");
	setWindowFlags(Qt::FramelessWindowHint);
 //	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
 //	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      
//	m_pGraphFile = std::make_shared<CGraphFile>();

//	pScene->setSceneRect(m_pGraphFile->GetBackgroundRect());


	scale(1.0, 1.0);
// 	connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this, &CGraphView::slot_SliderMoved);
// 	connect(verticalScrollBar(),   &QScrollBar::sliderMoved, this, &CGraphView::slot_SliderMoved);

//	connect(horizontalScrollBar(),)
 

// 	connect(m_pDoubleManager,
// 		static_cast<void (QtDoublePropertyManager::*)(QtProperty *, double )> (&QtDoublePropertyManager::valueChanged),
// 		this,
// 		//&CGraphView::valueChanged);
// 		static_cast<void (CGraphView::*)(QtProperty *property, double val)> (&CGraphView::valueChanged));

// 	connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
// 		this, SLOT(valueChanged(QtProperty *, const QString &)));
// 	connect(colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)),
// 		this, SLOT(valueChanged(QtProperty *, const QColor &)));
// 	connect(fontManager, SIGNAL(valueChanged(QtProperty *, const QFont &)),
// 		this, SLOT(valueChanged(QtProperty *, const QFont &)));
// 	connect(pointManager, SIGNAL(valueChanged(QtProperty *, const QPoint &)),
// 		this, SLOT(valueChanged(QtProperty *, const QPoint &)));
// 	connect(sizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)),
// 		this, SLOT(valueChanged(QtProperty *, const QSize &)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(Slot_ScrollBarChange()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(Slot_ScrollBarChange())); 
}

CRealTimeView::~CRealTimeView()
{ 

}

CGraphScene *CRealTimeView::GetGraphicsScene() const
{
	return dynamic_cast<CGraphScene *>(scene());
} 
   
void CRealTimeView::mouseMoveEvent(QMouseEvent * e)
{
	return QGraphicsView::mouseMoveEvent(e);
}

void CRealTimeView::mousePressEvent(QMouseEvent * e)
{
	QPointF m_CurrentClickPoint = mapToScene(e->pos());

	bool m_bMovingFlag = false;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	if (e->button() == Qt::LeftButton)
	{// 如果左键
		OnLButtonDown(e);
	}
	else if (e->button() == Qt::RightButton)
	{
		
	}
	this->scene()->update();

	return QGraphicsView::mousePressEvent(e);
}

void CRealTimeView::mouseReleaseEvent(QMouseEvent * e)
{
	return QGraphicsView::mouseReleaseEvent(e);
}

void CRealTimeView::mouseDoubleClickEvent(QMouseEvent * e)
{
	return QGraphicsView::mouseDoubleClickEvent(e);
}

void CRealTimeView::OnLButtonDown(QMouseEvent * e)
{	
	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	QPoint ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());
	
	CBaseWidget *pObj = HitTest(ptTemp, GetLayerIndex());
	
	Q_ASSERT(pObj);
	if (pObj)
	{
		pObj->SetPressFlag(true);
		pObj->update(); ;
	}

	CPushBtnWidget * pBtn = dynamic_cast<CPushBtnWidget*>(pObj);
	if (pBtn)
	{
		if (pBtn->GeteExecType()== CPushBtnWidget::BTN_ON_OFF)
		{
			emit sig_BtnOrderType(pBtn->GetBtnBindData(), pBtn->GetRemoteControlType());
		}		
	}
}

CBaseWidget* CRealTimeView::HitTest(QPoint & pos, unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx == 0 || nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return nullptr;
	
	CGraphScene *pScene = GetGraphicsScene();//  dynamic_cast<CGraphScene*>(pView->scene());

	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return nullptr;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return nullptr;

	auto pLayer = pGraphFile->GetLayer(GetLayerIndex());
	Q_ASSERT(pLayer);
	if (pLayer == nullptr)
		return nullptr;

	return pLayer->HitTest(pos);
}


void CRealTimeView::drawBackground(QPainter *painter, const QRectF &rect)
{
	CGraphScene * pScene = GetGraphicsScene();
	if (pScene)
	{
		CGraphFile  *pFile = (pScene->GetGraphFile());
		if (pFile)\
		{
			pFile->DrawBackground(painter, rect);
		}
	}
}


void CRealTimeView::paintEvent(QPaintEvent *event)
{
     /*if (viewport()->width() != m_nViewPortWidth || viewport()->height() != m_nViewPortHeight)
     {
         //当视图大小发送变化时,需要重绘背景
         m_nViewPortWidth = viewport()->width();
        m_nViewPortHeight = viewport()->height();
 
         setCacheMode(CacheNone);
     }
     else
     {
         //当没有变化时
         setCacheMode(CacheBackground);
     }*/
    QGraphicsView::paintEvent(event);
}

/*
void CRealTimeView::drawBackground(QPainter *painter, const QRectF &rect)
{
	CGraphScene * pScene = GetGraphicsScene();
	if (pScene)
	{
		pScene->GetGraphFile()->DrawBackground(painter, rect);
	}
}*/

void CRealTimeView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu;
	QAction *pAction = nullptr;

	menu.exec(event->globalPos());
}




/** @}*/
