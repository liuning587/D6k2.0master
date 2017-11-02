#include "surface3D_wgt.h"
#include "widget_factory.h"
#include  "graph_module.h"
#include "surface3dmodel.h"
#include "colour_define.h"

#include <QPainter>
#include <QGraphicsProxyWidget>
#include <QtQml/QQmlEngine>
#include <QQmlContext>

CSurface3DWidget::CSurface3DWidget()
{
}


CSurface3DWidget::CSurface3DWidget(QRectF rcPos, THREE_D_TYPE shape): CBaseWidget(rcPos)
{
	m_nObjType = 0;
	m_Shape = shape;

	m_rcPos = rcPos;
	m_rcMovingPos = rcPos;

	//默认为5
	m_nInterval = 0;

	m_pQuickView = nullptr;
	m_p3dModel = nullptr;
	m_pProxyWgt = new QGraphicsProxyWidget(this);


	//
	//setFlag(QGraphicsItem::ItemIsMovable, true);
// 	setFlag(QGraphicsItem::ItemIsSelectable, true);
// 	setFlag(QGraphicsItem::ItemIsFocusable, true);


	m_eRotateCenter = ROTATE_MIDCENTER;

	switch (shape)
	{
	case THREE_D_TYPE::SURFACE_3D:
	{
		//曲面图
		m_szName = "Surface3D";
		m_nObjType = DRAW_3D_SURFACE;
		m_rcPos.setSize(QSizeF(600, 400));
		m_rcMovingPos.setSize(QSizeF(600, 400));


		m_pQuickView = new QQuickWidget;

		m_pProxyWgt->setWidget(m_pQuickView);

		m_p3dModel = new CSurface3dModel;
		//测试用数据
		InitModelData(m_p3dModel);

		m_pQuickView->rootContext()->setContextProperty("cityModel", m_p3dModel);

		m_pQuickView->setSource(QUrl("qrc:qml/Erirst3D.qml"));
		m_pQuickView->setResizeMode(QQuickWidget::SizeRootObjectToView);
		
		break;
	}

	default:
		break;
	}
}

void CSurface3DWidget::InitModelData(CSurface3dModel *tModel)
{
	std::shared_ptr<CItemSurface>ptrLog1 = std::make_shared<CItemSurface>();
	ptrLog1->m_fxData = 20;
	ptrLog1->m_fyData = 10;
	ptrLog1->m_fzData = 4.75;

	std::shared_ptr<CItemSurface>ptrLog2 = std::make_shared<CItemSurface>();
	ptrLog2->m_fxData = 21;
	ptrLog2->m_fyData = 10;
	ptrLog2->m_fzData = 3.00;

	std::shared_ptr<CItemSurface>ptrLog3 = std::make_shared<CItemSurface>();
	ptrLog3->m_fxData = 22;
	ptrLog3->m_fyData = 10;
	ptrLog3->m_fzData = 1.24;

	std::shared_ptr<CItemSurface>ptrLog4 = std::make_shared<CItemSurface>();
	ptrLog4->m_fxData = 23;
	ptrLog4->m_fyData = 10;
	ptrLog4->m_fzData = 2.53;

	std::shared_ptr<CItemSurface>ptrLog5 = std::make_shared<CItemSurface>();
	ptrLog5->m_fxData = 20;
	ptrLog5->m_fyData = 11;
	ptrLog5->m_fzData = 2.55;

	std::shared_ptr<CItemSurface>ptrLog6 = std::make_shared<CItemSurface>();
	ptrLog6->m_fxData = 21;
	ptrLog6->m_fyData = 11;
	ptrLog6->m_fzData = 2.03;

	std::shared_ptr<CItemSurface>ptrLog7 = std::make_shared<CItemSurface>();
	ptrLog7->m_fxData = 22;
	ptrLog7->m_fyData = 11;
	ptrLog7->m_fzData = 3.46;

	std::shared_ptr<CItemSurface>ptrLog8 = std::make_shared<CItemSurface>();
	ptrLog8->m_fxData = 23;
	ptrLog8->m_fyData = 11;
	ptrLog8->m_fzData = 5.12;

	std::shared_ptr<CItemSurface>ptrLog9 = std::make_shared<CItemSurface>();
	ptrLog9->m_fxData = 20;
	ptrLog9->m_fyData = 12;
	ptrLog9->m_fzData = 1.37;

	std::shared_ptr<CItemSurface>ptrLog10 = std::make_shared<CItemSurface>();
	ptrLog10->m_fxData = 21;
	ptrLog10->m_fyData = 12;
	ptrLog10->m_fzData = 2.98;

	std::shared_ptr<CItemSurface>ptrLog11 = std::make_shared<CItemSurface>();
	ptrLog11->m_fxData = 22;
	ptrLog11->m_fyData = 12;
	ptrLog11->m_fzData = 3.33;

	std::shared_ptr<CItemSurface>ptrLog11x = std::make_shared<CItemSurface>();
	ptrLog11x->m_fxData = 23;
	ptrLog11x->m_fyData = 12;
	ptrLog11x->m_fzData = 3.23;

	std::shared_ptr<CItemSurface>ptrLog12 = std::make_shared<CItemSurface>();
	ptrLog12->m_fxData = 20;
	ptrLog12->m_fyData = 13;
	ptrLog12->m_fzData = 4.34;

	std::shared_ptr<CItemSurface>ptrLog13 = std::make_shared<CItemSurface>();
	ptrLog13->m_fxData = 21;
	ptrLog13->m_fyData = 13;
	ptrLog13->m_fzData = 3.54;

	std::shared_ptr<CItemSurface>ptrLog14 = std::make_shared<CItemSurface>();
	ptrLog14->m_fxData = 22;
	ptrLog14->m_fyData = 13;
	ptrLog14->m_fzData = 1.65;

	std::shared_ptr<CItemSurface>ptrLog15 = std::make_shared<CItemSurface>();
	ptrLog15->m_fxData = 23;
	ptrLog15->m_fyData = 13;
	ptrLog15->m_fzData = 2.67;




	tModel->AddItem(ptrLog1);
	tModel->AddItem(ptrLog2);
	tModel->AddItem(ptrLog3);
	tModel->AddItem(ptrLog4);
	tModel->AddItem(ptrLog5);

	tModel->AddItem(ptrLog6);
	tModel->AddItem(ptrLog7);
	tModel->AddItem(ptrLog8);
	tModel->AddItem(ptrLog9);
	tModel->AddItem(ptrLog10);

	tModel->AddItem(ptrLog11);
	tModel->AddItem(ptrLog11x);
	tModel->AddItem(ptrLog12);

	tModel->AddItem(ptrLog13);
	tModel->AddItem(ptrLog14);

	tModel->AddItem(ptrLog15);
}

void CSurface3DWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	pPainter->save();

	if (static_cast<int>(m_dblRotateAngle) != 0)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}

	if (static_cast<int>(m_dblRotateAngle) % 90 != 0)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}


	QPen pen;
	m_PenInfo.ToQPen(pen);
	pPainter->setPen(pen);

	QPointF ptCenter = GetRotationCenterPoint();

	QPainterPath path;
	GetPath(path);

	//填充背景颜色

	CBrushInfo* pBrush = GetBrushInfo();
	pBrush->GetColor().dark(0);
	pBrush->SetColor(QColor(255,0,0,0));
	FillPath(pPainter, path, *pBrush);



	pPainter->drawPath(path);



	pPainter->restore();


	/////////////////////////
	pPainter->save();

	QPixmap tImage;
#ifdef GRAPH_LIB
	//配置状态
	tImage = GetModuleApi()->GetGraphicPath() + GetImageInfo()->GetBackgroundImage();
#else
	tImage = GetImageInfo()->GetBackgroundImage();
#endif // GRAPH_LIB
	//GetImageInfo()->SetImageSize(tImage.size());

	if (!tImage.isNull())
	{
		if (GetImageInfo()->GetImageSize().width() == 0 || GetImageInfo()->GetImageSize().height() == 0)
		{
			//图片的实际尺寸
			GetImageInfo()->SetImageSize(tImage.size());
		}
		else
		{
			tImage = tImage.scaled(GetImageInfo()->GetImageSize());
		}

		if (GetImageInfo()->GetStretch())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			//拉伸
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage.scaled(m_rcPos.size().toSize()));
			//m_szImage = rect.size().toSize();

		}
		else if (GetImageInfo()->GetbTiling())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			//平铺
			pPainter->drawTiledPixmap(m_rcPos, tImage);
			//m_szImage = rect.size().toSize();
		}
		else if (tImage.size().width() > m_rcPos.size().width() || tImage.size().height() > m_rcPos.size().height())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage.scaled(m_rcPos.size().toSize()));
			//m_szImage = rect.size().toSize();
		}
		else
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage);
			//m_szImage = tImage.size();

		}
	}

	pPainter->restore();
	/////////////////////////
	/*if (!GetPixMap().isNull())
	{
	pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
	pPainter->rotate(GetRotateAngle());
	pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
	pPainter->drawPixmap(boundingRect().toRect(), GetPixMap());
	}*/
	// 画选择框
	DrawSelectedTracker(pPainter);

	//
	if (m_pProxyWgt != nullptr)
	{
		m_pProxyWgt->resize(m_rcPos.size().width() - 10, m_rcPos.size().height() - 10);
		m_pProxyWgt->setPos(m_rcPos.topLeft().x() + 5, m_rcPos.topLeft().y() + 5);

	}

	//m_pQuickView->resize(m_rcPos.size().width() - m_nInterval*2, m_rcPos.size().height() - m_nInterval * 2);
	//m_pQuickView->setPosition(m_rcPos.topLeft().x() + m_nInterval, m_rcPos.topLeft().y() + m_nInterval);
// 	m_pPlotWgt->UpdateChart();
}

QRectF CSurface3DWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();
	// 	double dPenWidth = m_PenInfo.GetWidth() / 2;
	// 
	// 	if (dPenWidth > 0.0f)
	// 	{ // 扩大半个线宽
	// 		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
	// 	}
	return rcBounding;
}

QPointF CSurface3DWidget::GetRotationCenterPoint() const
{
	QPointF ptCenter;

	if (m_eRotateCenter == ROTATE_TOPLEFT)
	{
		//左上角
		ptCenter = m_rcPos.topLeft();
	}
	else if (m_eRotateCenter == ROTATE_TOPMID)
	{
		//中上
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.topLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_TOPRIGHT)
	{
		//右上
		ptCenter = m_rcPos.topRight();
	}
	else if (m_eRotateCenter == ROTATE_LEFTMID)
	{
		//左中
		ptCenter = QPointF(m_rcPos.topLeft().x(), m_rcPos.center().y());;
	}
	else if (m_eRotateCenter == ROTATE_MIDCENTER)
	{
		//中心
		ptCenter = m_rcPos.center();
	}
	else if (m_eRotateCenter == ROTATE_RIGHTMID)
	{
		//右中
		ptCenter = QPointF(m_rcPos.topRight().x(), m_rcPos.center().y());
	}
	else if (m_eRotateCenter == ROTATE_LEFTBOTTOM)
	{
		//左下角
		ptCenter = m_rcPos.bottomLeft();
	}
	else if (m_eRotateCenter == ROTATE_BOTTOMMID)
	{
		//下中
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.bottomLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_RIGHTBOTTOM)
	{
		//右下
		ptCenter = QPointF(m_rcPos.bottomRight());
	}

	return ptCenter;
}

void CSurface3DWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
{
	QRectF rcLoc = m_rcPos;
	QPointF ptTemp = ptInput;

	switch (nHandle)
	{
	case POS_TOP_LEFT:
		//  移动左上角不能超过右边及下边
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTopLeft(ptTemp);

		break;
	case POS_TOP_CENTER:
		//  移动上边不能超过下边
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTop(ptTemp.y());

		break;
	case POS_TOP_RIGHT:
		//  移动右上角 不能超过左下角
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTopRight(ptTemp);

		break;
	case POS_RIGHT_CENTER:
		// 移动右边 不能超过 左边
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setRight(ptTemp.x());

		break;
	case POS_BOTTOM_RIGHT:
		// 移动右下角 不能超过左上角
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		rcLoc.setBottomRight(ptTemp);

		break;

	case POS_BOTTOM_CENTER:
		// 移动下边 不能超过上边
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setBottom(ptTemp.y());

		break;
	case POS_BOTTOM_LEFT:
		// 移动左下角 不能超过右上角
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setBottomLeft(ptTemp);
		break;
	case POS_LEFT_CENTER:
		// 移动左边 不能超过右边
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		rcLoc.setLeft(ptTemp.x());

		break;
	}
	rcLoc = rcLoc.normalized();

	MoveTo(rcLoc);
}

void CSurface3DWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected /*= false*/)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	pPainter->save();

	if (bSingleSelected)
	{
		QPen pen(XGraphics::LightBlue);
		pen.setWidthF(m_PenInfo.GetWidth());
		pen.setStyle(Qt::DashLine);

		pPainter->setPen(pen);
	}
	else
	{// 多选的移动(或者组合图元的子图元)，原来是啥样，还是啥样
		QPen pen(Qt::darkGray);
		pen.setWidth(1);
		pen.setStyle(Qt::SolidLine);
		pPainter->setPen(pen);
	}

	QColor clBrush(XGraphics::LightBlue);
	clBrush.setAlpha(240);
	QBrush brBack;
	brBack.setColor(clBrush);
	brBack.setStyle(Qt::SolidPattern);

	//旋转坐标轴
	if (GetRotateAngle() == 0.0f)
	{

	}
	else
	{
		pPainter->setRenderHint(QPainter::Antialiasing);

		QPointF tCenter = GetRotationCenterPoint();

		pPainter->translate(GetMovingRotationCenterPoint());
		pPainter->rotate(GetRotateAngle());
		pPainter->translate(-GetMovingRotationCenterPoint());


	}


	pPainter->fillRect(m_rcMovingPos, brBack);


	pPainter->restore();

	pPainter->save();
	QPainterPath path;
	GetMovingPath(path);

	//     pPainter->fillPath(path, brBack);

	if (GetRotateAngle() != 0.0f )
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}

	pPainter->drawPath(path);
	pPainter->restore();
}
 
void CSurface3DWidget::GetPath(QPainterPath& path)
{
	QRectF rcShape = m_rcPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QPointF ptCenter = GetRotationCenterPoint();
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(ptCenter.x(), ptCenter.y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-ptCenter.x(), -ptCenter.y());

		p1.addRect(rcShape);
		path = tr_temp.map(p1);
	}
	else
	{
		path.addRect(rcInput);
	}
}

void CSurface3DWidget::GetMovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
}



CSurface3DWidget::~CSurface3DWidget()
{
	//m_pProxyWgt->deleteLater();

	m_pProxyWgt->setWidget(nullptr);
	m_p3dModel->deleteLater();

	m_pQuickView->deleteLater();


}
