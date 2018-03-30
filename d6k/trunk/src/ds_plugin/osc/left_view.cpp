

#include "left_view.h"
#include "borderlayout.h"
#include "wave_info.h"
#include "childfrm.h"
#include "channelview_info.h"

#include <QLineEdit>

CLeftView::CLeftView(std::shared_ptr<CComtrade>  pWave, QWidget *parent) : m_pWave(pWave), QWidget(parent)
{
	m_pPointNumEdit = std::make_shared<QLineEdit>();
	m_pLeftRender = std::make_shared<CLeftRenderArea>(pWave,this);

	m_pPointNumEdit->setFixedHeight(16);
	m_pPointNumEdit->setEnabled(false);
	m_pPointNumEdit->setAlignment(Qt::AlignRight);
	m_pPointNumEdit->setText("No Point");

	m_pLayout = new CBorderLayout;
	m_pLayout->setSizeConstraint(QLayout::SetMinimumSize);
	m_pLayout->addWidget(m_pLeftRender.get(), CBorderLayout::Center);
	m_pLayout->addWidget(m_pPointNumEdit.get(), CBorderLayout::South);

	setLayout(m_pLayout); 
//	connect(m_pLeftRender, SIGNAL(drawPointNumSignal(QString)), m_pPointNumEdit, SLOT(setText(QString)));
}

CLeftView::~CLeftView()
{
	Destroy();
}

void CLeftView::Destroy()
{
	if (m_pLayout)
	{
		delete m_pLayout;
	}
}

void CLeftView::Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo)
{
	Q_ASSERT(m_pLeftRender);
	if (m_pLeftRender)
	{
		m_pLeftRender->Initial(pChannelViewInfo);
	}
}

QSize CLeftView::minimumSizeHint() const
{
	return QSize(0, 400);
}

QSize CLeftView::sizeHint() const
{
	return QSize(40, 400);
}

CLeftRenderArea::CLeftRenderArea(std::shared_ptr<CComtrade> pWave, QWidget *parent) :m_pWave(pWave), QWidget(parent)
{
	setMouseTracking(true);
	m_nDivision = 100;

	m_nCurTimePos = 0;
}

void CLeftRenderArea::Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo)
{
	m_pChannelViewInfo = pChannelViewInfo;
	Q_ASSERT(pChannelViewInfo);
	if (pChannelViewInfo)
	{
		pChannelViewInfo->SetLeftRender(this);
	}
}

void CLeftRenderArea::mouseMoveEvent(QMouseEvent * pEvent)
{
	int nButtonX = pEvent->x();
	int nButtonY = pEvent->y();

	int nType = pEvent->type();

	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->LeftHighLightChannel(nButtonY,this);
	}
}

void CLeftRenderArea::resizeEvent(QResizeEvent *event)
{
	//每屏点数
	//m_nPointsPerScreen = (width() - 2) / m_nPixBetweenPoint;
	int nWidth = width();

	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->SetLeftRectSize(nWidth);
	}
}

void CLeftRenderArea::paintEvent(QPaintEvent *event)
{
	int x = (event->rect()).width();
	int y = (event->rect()).height();

	QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.fillRect(event->rect(), QBrush(Qt::white));
//	painter.fillRect(QRect(0, 0, x, y - 17), QBrush(Qt::lightGray));
	painter.setViewport(QRect(0, 0, x, y));

	DrawBackground(painter);

	// 画分隔线
//	DrawDivisionLine(painter);
	// 画下方的时间框
	DrawTimeRect(painter);

	DrawIndicateValue(painter);
	// 画指示箭头
// 	DrawTriangle(painter);
// 	if (!datFileList.empty() && datFileList.size() != 0 && !cfgFileList.empty() && cfgFileList.size() != 0)
// 		drawTimeValue(painter);
//	DrawChannelName(painter);
} 
/*! \fn void CLeftRenderArea::DrawDivisionLine(QPainter & painter)
********************************************************************************************************* 
** \brief CLeftRenderArea::DrawDivisionLine 
** \details 画分隔线
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月1日 
** \note 
********************************************************************************************************/
void CLeftRenderArea::DrawDivisionLine(QPainter & painter)
{
	QPen pen = painter.pen();
	pen.setColor(Qt::magenta);
	pen.setStyle(Qt::DashLine);
	painter.setPen(pen);
	QRect viewPort = painter.viewport();
	int x = viewPort.width();

	int nChannelCount = 7;

//	Q_ASSERT(m_pWave && m_pWave->GetConfig());
	if (m_pWave && m_pWave->GetConfig())
	{
		nChannelCount = m_pWave->GetConfig()->GetTotalChannelCount();
	}
	for (int i = 1; i <= nChannelCount; i++)
		painter.drawLine(0, i*m_nDivision, x, i*m_nDivision);
}
/*! \fn void CLeftRenderArea::DrawTimeRect(QPainter & painter)
********************************************************************************************************* 
** \brief CLeftRenderArea::DrawTimeRect 
** \details 画下方的时间框
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月1日 
** \note 
********************************************************************************************************/
void CLeftRenderArea::DrawTimeRect(QPainter & painter)
{
	QRect viewPort = painter.viewport();
	int x = viewPort.width();
	int y = viewPort.height();
	painter.setBrush(Qt::blue);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, y - 20, x, 20);
	painter.setPen(Qt::lightGray);
	painter.drawLine(0, y - 20, x, y - 20);
}
/*! \fn void CLeftRenderArea::DrawTriangle(QPainter & painter)
********************************************************************************************************* 
** \brief CLeftRenderArea::DrawTriangle 
** \details 画三角形
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月1日 
** \note 
********************************************************************************************************/
void CLeftRenderArea::DrawTriangle(QPainter & painter)
{
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
	QRect viewPort = painter.viewport();
	int x = viewPort.width();
	//QBrush brush;	
	for (int i = 0; i < 7; i++)
	{
		painter.setBrush(QBrush(m_lstColor.at(i % 7)));
		QPointF points[3] = { QPointF(x,(50 + i * 100)),QPointF((x - 10),(44 + i * 100)),QPointF((x - 10),(56 + i * 100)) };
		painter.drawPolygon(points, 3);
	}
}
/*! \fn void  CLeftRenderArea::DrawChannelName(QPainter &painter)
********************************************************************************************************* 
** \brief CLeftRenderArea::DrawChannelName 
** \details 画通道名称
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月2日 
** \note 
********************************************************************************************************/
void  CLeftRenderArea::DrawChannelName(QPainter &painter)
{
	QPen pen = painter.pen();
	pen.setColor(Qt::magenta);
	pen.setStyle(Qt::DashLine);
	painter.setPen(pen);
	QRect viewPort = painter.viewport();
	int nWidth = viewPort.width();

	int nChannelCount = 7;

	//	Q_ASSERT(m_pWave && m_pWave->GetConfig());
	if (m_pWave && m_pWave->GetConfig())
	{
		nChannelCount = m_pWave->GetConfig()->GetTotalChannelCount();
	}

	QString szText;

// 	QFont ft = painter.font();
// 	QFontMetrics fm(ft);
// 	int h = fm.height() + 1;

	int nPad = m_nDivision / 10;

	for (int i = 0; i < nChannelCount; i++)
	{
		szText = QString("Ia-%1").arg(i);

		painter.drawText(QRect(nPad, nPad + m_nDivision * i, nWidth * 4 / 5, m_nDivision - 2 * nPad), Qt::AlignVCenter | Qt::AlignLeft, szText);
	}
}

void CLeftRenderArea::DrawBackground(QPainter &painter)
{
	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->DrawLeftBackground(this, painter);
	}
}

void CLeftRenderArea::DrawIndicateValue(QPainter &painter)
{
	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		int nT1PointNum = m_nCurTimePos + m_pChannelViewInfo->GetStartPointNum();
		//m_pChannelViewInfo->DrawLeftBackground(this, painter);
		m_pChannelViewInfo->DrawLeftIndicateValue(this, painter, nT1PointNum, nT1PointNum);
	}
}

void  CLeftRenderArea::ShowFirstIndicatePointNumberSlot(int nPos)
{
	bool bChanage = false;
	if (nPos != m_nCurTimePos)
	{
		bChanage = true;
	}
	m_nCurTimePos = nPos;
	if (bChanage)
	{
		update();
	}
	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		int nPointNum = nPos + m_pChannelViewInfo->GetStartPointNum();
        // 发射信号
	}
}
