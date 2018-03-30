

#include "right_view.h"
#include "borderlayout.h"

#include "wave_info.h"
#include "channelview_info.h"

CRightView::CRightView(std::shared_ptr<CComtrade> pWave, QWidget *parent) : m_pWave(pWave),QWidget(parent)
{
	Q_ASSERT(pWave);
	m_pRightRender = std::make_shared<CRightRenderArea>(pWave,this);

	m_pHorzScrollBar = std::make_shared<QScrollBar>(Qt::Horizontal);
	m_pHorzScrollBar->setMinimum(0);
	m_pHorzScrollBar->setSingleStep(25);
	m_pHorzScrollBar->setPageStep(100);

	connect(m_pHorzScrollBar.get(), SIGNAL(valueChanged(int)), m_pRightRender.get(), SLOT(SetStartPointNum(int)));

	m_pVertScrollBar = std::make_shared<QScrollBar>(Qt::Vertical);
	m_pVertScrollBar->setMinimum(0);
	// verScrollBar->setMaximum(7);//?????????????? to  be changed!???????????
	m_pVertScrollBar->setSingleStep(1);
	m_pVertScrollBar->setPageStep(4);

	connect(m_pVertScrollBar.get(), SIGNAL(valueChanged(int)), m_pRightRender.get(), SLOT(SetStartChannelNum(int)));


	m_pLayout = new CBorderLayout;
	m_pLayout->setSizeConstraint(QLayout::SetMinimumSize);
	m_pLayout->addWidget(m_pRightRender.get(), CBorderLayout::Center); 
	m_pLayout->addWidget(m_pHorzScrollBar.get(), CBorderLayout::South);
	m_pLayout->addWidget(m_pVertScrollBar.get(), CBorderLayout::East);

	setLayout(m_pLayout);
}

CRightView::~CRightView()
{
	Destroy();
}

void CRightView::Destroy()
{
	if (m_pLayout)
	{
		delete m_pLayout;
	}
}

void  CRightView::Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo)
{
	Q_ASSERT(m_pRightRender);
	if (m_pRightRender)
	{
		m_pRightRender->Initial(pChannelViewInfo);
	}
}

CRightRenderArea::CRightRenderArea(std::shared_ptr<CComtrade> pWave, QWidget *parent) : m_pWave(pWave), QWidget(parent)
{
	Q_ASSERT(pWave);
	setMouseTracking(true);
	m_nFirstIndicatePos = 4;
	m_nSecondIndicatePos = 2;

	startChannelNum = 0;
//	m_nStartPointNum = 0;

//	m_nChannelRectHeight = 100;
//	m_nChannelMargin = 5;

//	m_nWaveRectHeight = m_nChannelRectHeight - 2 * m_nChannelMargin;

//	m_nPixBetweenPoint = 2;
//	m_nPixBetweenMaxMid = 20;

	//每屏点数
//	m_nPointsPerScreen = (width() - 2) / m_nPixBetweenPoint;

	Q_ASSERT(pWave);
#if 0
	if (pWave)
	{
		const std::shared_ptr<CWaveCfg> & pCfg = pWave->GetConfig();
		Q_ASSERT(pCfg);
		if (pCfg)
		{
			int nIndex = 0;
			std::vector<std::shared_ptr<CAnaChannel>> & arrAna = pCfg->GetAnaChannels();

			std::shared_ptr<CAnaChannelViewInfo> pAnaViewInfo;

			for (auto it = arrAna.cbegin(); it != arrAna.cend(); ++it)
			{
			//	pAnaViewInfo = std::make_shared<CAnaChannelViewInfo>(**it, m_nWaveRectHeight, nIndex*m_nChannelRectHeight + m_nChannelRectHeight / 2);
				pAnaViewInfo = std::make_shared<CAnaChannelViewInfo>(**it, m_nWaveRectHeight, nIndex*m_nChannelRectHeight, m_nChannelRectHeight,width());

				m_arrAnaChannelInfos.emplace_back(pAnaViewInfo);
				nIndex++;
			}
		}
	}
#endif

	m_nMoveIndicateLine = MoveIndicateLine::IndicateLineNone;
}

QSize CRightRenderArea::minimumSizeHint() const
{
	return QSize(100, 400);
}

QSize CRightRenderArea::sizeHint() const
{
	return QSize(100, 400);
}

void CRightRenderArea::Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo)
{
	m_pChannelViewInfo = pChannelViewInfo;
	Q_ASSERT(pChannelViewInfo);
	if (pChannelViewInfo)
	{
		pChannelViewInfo->SetRightRender(this);
	}
}

//paintEvent function
void CRightRenderArea::paintEvent(QPaintEvent *pEvent)
{
	int x = (pEvent->rect()).width();
	int y = (pEvent->rect()).height();

	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->SetPointNumPerScreen(x);
	}
#if 0
	if ((pEvent->rect()).width() > 2)
	{
		m_nPointsPerScreen = ((pEvent->rect()).width() - 2) / m_nPixBetweenPoint;
	}
#endif

	QPainter painter(this);
	painter.setViewport(QRect(0, 0, x, y));

	DrawBackground(painter);
	//画指示线
	DrawIndicateLine(painter, m_nFirstIndicatePos);
	DrawIndicateLine2(painter, m_nSecondIndicatePos);
 //	DrawChannelRect(painter);
  
  	DrawTimeLine(painter);
//  	DrawSymmetryLine(painter);
// 	if (!datFileList.empty() && datFileList.size() != 0)
// 	{
 		DrawTimeInterval(painter);
 		DrawWave(painter);
// 	}
}
/*! \fn void CRightRenderArea::DrawBackground(QPainter &painter)
********************************************************************************************************* 
** \brief CRightRenderArea::DrawBackground 
** \details 画背景
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月14日 
** \note 
********************************************************************************************************/
void CRightRenderArea::DrawBackground(QPainter &painter)
{
	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->DrawRightBackground(this,painter);
	}
#if 0
	painter.save();

//	painter.setRenderHint(QPainter::Antialiasing);
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->DrawRightBackground(this,painter);
		}
	}
	painter.restore();

	int x = width();
	int y = height();

	painter.save();

	//painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(this->rect(), QBrush(Qt::white));

	painter.restore();
#endif
}

//画指示线
void  CRightRenderArea::DrawIndicateLine(QPainter & painter, int x)
{
	painter.save();

	QPen pen = painter.pen();
	pen.setCapStyle(Qt::SquareCap);
	pen.setStyle(Qt::DotLine);
	pen.setColor(Qt::red);
	pen.setWidthF(0);
	painter.setPen(pen);
	//painter.save();
	QRect viewPort = painter.viewport();
	int nHeight = viewPort.height();
	painter.drawLine(x, 0, x, nHeight);

	painter.restore();
}

void CRightRenderArea::DrawIndicateLine2(QPainter & painter, int x)
{
	painter.save();

	QPen pen = painter.pen();
	pen.setCapStyle(Qt::SquareCap);
	pen.setStyle(Qt::DotLine);
	pen.setColor(Qt::darkRed);
	pen.setWidthF(0);
	painter.setPen(pen);
	//painter.save();
	QRect viewPort = painter.viewport();
	int nHeight = viewPort.height();
	painter.drawLine(x, 0, x, nHeight);

	painter.restore();
}
/*! \fn void CRightRenderArea::DrawDivisionLine(QPainter & painter)
********************************************************************************************************* 
** \brief CRightRenderArea::DrawDivisionLine 
** \details 画分隔线
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月2日 
** \note 
********************************************************************************************************/
void CRightRenderArea::DrawChannelRect(QPainter & painter)
{
#if 0
	painter.save();

	QPen pen = painter.pen();
	pen.setColor(Qt::darkGray);
//	pen.setStyle(Qt::DashLine);
	//pen.setWidthF(0.5);
	pen.setCapStyle(Qt::SquareCap);
	painter.setPen(pen);
	QRect viewPort = painter.viewport();
	int x = viewPort.width();
	for (int i = 1; i <= 7; i++)
		painter.drawLine(0, i*m_nChannelRectHeight, x, i*m_nChannelRectHeight);

	painter.restore();
#endif
}
/*! \fn void CRightRenderArea::DrawTimeLine(QPainter & painter)
********************************************************************************************************* 
** \brief CRightRenderArea::DrawTimeLine 
** \details 画时间线(下方的时间轴)
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月2日 
** \note 
********************************************************************************************************/
void CRightRenderArea::DrawTimeLine(QPainter & painter)
{
	painter.save();

	QRect viewPort = painter.viewport();
	int x = viewPort.width();
	int y = viewPort.height();
	painter.setBrush(Qt::blue);
	painter.drawRect(0, y - 20, x, 20);
	painter.setPen(Qt::lightGray);
	painter.drawLine(0, y - 20, x, y - 20);
	
	painter.restore();
}
/*! \fn void CRightRenderArea::DrawSymmetryLine(QPainter & painter)
********************************************************************************************************* 
** \brief CRightRenderArea::DrawSymmetryLine 
** \details 中间的坐标轴
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月2日 
** \note 
********************************************************************************************************/
void CRightRenderArea::DrawSymmetryLine(QPainter & painter)
{
#if 0
	QPen pen = painter.pen();
	pen.setColor(Qt::black);
	pen.setStyle(Qt::DashLine);
	QRect viewPort = painter.viewport();
	int nWidth = viewPort.width();
	for (int i = 0; i < 7; i++)
	{
		painter.drawLine(0, (m_nChannelRectHeight / 2 + m_nChannelRectHeight * i), nWidth, (m_nChannelRectHeight / 2 + m_nChannelRectHeight * i));
	}
#endif
}
/**
画时间轴上的间隔线
25个点用50个象素表示
*/
void CRightRenderArea::DrawTimeInterval(QPainter & painter)
{
	Q_ASSERT(m_pWave);
	if (m_pWave == nullptr)
		return;

	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo == nullptr)
		return;

	QRect viewPort = painter.viewport();
	int y = viewPort.height();
	painter.setPen(Qt::magenta);
	int i = 0;

	int nLineCount = m_pWave->GetData()->GetDataLineCount();

	int nStartPointNum = m_pChannelViewInfo->GetStartPointNum();
	int nPixBetweenMaxMid = m_pChannelViewInfo->GetPixBetweenMaxMid();
	
	while ((nStartPointNum + i * 25) <= nLineCount)
	{
		//用2个象素的长度来标识相邻采样点的间隔(不同采样频率下，所表示的时间间隔不一样)	
		painter.drawLine(2 + 25 * nPixBetweenMaxMid*i, y - 20, 2 + 25 * nPixBetweenMaxMid*i, y - 25);
		if (i == 0)//第一个点的位置要偏右才能显示完整的数据
		{
			painter.drawText(2 + 25 * nPixBetweenMaxMid*i, y - 8, QString::number(i * 25 + nStartPointNum, 10));
		}
		else
		{
			if ((nStartPointNum + i * 25) < 10)  painter.drawText(2 + 25 * nPixBetweenMaxMid*i, y - 10, QString::number(i * 25 + nStartPointNum, 10));
			else if ((nStartPointNum + i * 25) < 100)
				painter.drawText(2 + 25 * nPixBetweenMaxMid*i - 5, y - 8, QString::number(i * 25 + nStartPointNum, 10));
			else
				painter.drawText(2 + 25 * nPixBetweenMaxMid*i - 10, y - 8, QString::number(i * 25 + nStartPointNum, 10));
		}
		i++;
	}

	if ((nLineCount % 25) != 0)
	{
		painter.drawLine(2 + 25 * nPixBetweenMaxMid*i, y - 20, 2 + 25 * nPixBetweenMaxMid*i, y - 25);//用2个象素的长度来标识相邻采样点的间隔(不同采样频率下，所表示的时间间隔不一样)
		painter.drawText(2 + 25 * nPixBetweenMaxMid*i - 10, y - 8, QString::number(i * 25 + nStartPointNum, 10));
	}
	// 画T1 T2
	painter.drawText(m_nFirstIndicatePos - 10, y - 20, "T1");
	painter.drawText(m_nSecondIndicatePos, y - 20, "T2");
}

void CRightRenderArea::DrawWave(QPainter & painter)
{
	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->DrawWave(this, painter);
	}
#if 0
	painter.save();

	painter.setRenderHint(QPainter::Antialiasing);
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->DrawWave(this, painter);
		}
	}
	painter.restore();
#endif
}

void CRightRenderArea::SetStartChannelNum(int nChannelNum)
{

}

void CRightRenderArea::SetStartPointNum(int nStartPointNum)
{
	Q_ASSERT(nStartPointNum >= 0);
	if (nStartPointNum < 0)
		return;

	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo == nullptr)
		return;

	// 没有发生改变
	if (nStartPointNum == m_pChannelViewInfo->GetStartPointNum())
		return;

	//每屏点数
	//m_nPointsPerScreen = (width() - 2) / m_nPixBetweenPoint;
	m_pChannelViewInfo->SetPointNumPerScreen(width());

	m_pChannelViewInfo->SetStartPointNum(nStartPointNum);

	m_pChannelViewInfo->OnUpdate(this, nStartPointNum);

	update();
}


void CRightRenderArea::resizeEvent(QResizeEvent *event)
{
	//每屏点数
	//m_nPointsPerScreen = (width() - 2) / m_nPixBetweenPoint;
	int nWidth = width();

	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->SetRightRectSize(nWidth);
		bool bRet = m_pChannelViewInfo->SetPointNumPerScreen(nWidth);
		if (bRet)
		{
			update();
		}
	}
#if 0
	int nTest = event->size().width();
	if (nWidth >= 2)
	{
		int nPointNum = (nWidth - 2) / m_nPixBetweenPoint;
		if (nPointNum != m_nPointsPerScreen)
		{
			m_nPointsPerScreen = nPointNum;
			update();
		}
	}
#endif
}

void CRightRenderArea::HighLightChannel(int nYValue)
{
	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		m_pChannelViewInfo->RightHighLightChannel(nYValue, this);
	}
#if 0
	bool bRet = false;
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			//	pInfo->OnUpdate(this, nStartPointNum);
			bRet = pInfo->IsInRightRect(nYValue);
			pInfo->SetHighLight(bRet, this);
		}
	}
#endif
}
void CRightRenderArea::mousePressEvent(QMouseEvent * pEvent)
{
	if (pEvent == nullptr)
		return;
	
	int nButtonX = pEvent->x();
	int nButtonY = pEvent->y();
	int nType = pEvent->type();
	Qt::MouseButton rtBtn = pEvent->button();

	if (nType == QEvent::MouseButtonPress && rtBtn == Qt::LeftButton)
	{
		SetFirstIndicatePos(nButtonX);
	}
	if (nType == QEvent::MouseButtonPress && rtBtn == Qt::RightButton)
	{
		SetSecondIndicatePos(nButtonX);
	}
}

void CRightRenderArea::mouseMoveEvent(QMouseEvent * pEvent)
{
	int nButtonX = pEvent->x();
	int nButtonY = pEvent->y();

	int nType = pEvent->type();

	HighLightChannel(nButtonY);

#if 0
	Qt::MouseButton rtBtn = pEvent->button();

	if (rtBtn == Qt::LeftButton)
	{
		SetFirstIndicatePos(nButtonX);
	}
	else if (rtBtn == Qt::RightButton)
	{
		SetSecondIndicatePos(nButtonX);
	}

	if (m_doubleIndicateLineMode)
	{
		if (m_pressIndicateLineType == 0)
		{
			setCursor(Qt::UpArrowCursor);
			setCurIndicatePos(buttonX);

			//emit drawPointValueSignal((buttonX-2)/pixBetweenPoint);//向leftrender窗口中发画起始点值的信号			
		}
		if (m_pressIndicateLineType == 99)
		{
			setCursor(Qt::UpArrowCursor);
			setAnoIndicatePos(buttonX);
		}
	}
#endif
}

void CRightRenderArea::SetFirstIndicatePos(int nPos)
{
	bool bUpdate = false;
	if (nPos != m_nFirstIndicatePos)
	{
		bUpdate = true;
	}
	m_nFirstIndicatePos = nPos;
	if (m_nFirstIndicatePos <= 2)
		m_nFirstIndicatePos = 2;

	if (bUpdate)
	{
		update();
	}

	Q_ASSERT(m_pChannelViewInfo);
	if (m_pChannelViewInfo)
	{
		int nPointNum = static_cast<int>((m_nFirstIndicatePos - 2) / m_pChannelViewInfo->GetPixBetweenPoint());
	//	nPointNum += m_pChannelViewInfo->GetStartPointNum();

		emit ShowFirstIndicatePointNumberSignal(nPointNum);
	}
	int timeVal = 0;
#if 0
	try
	{
		int pointOrd = (curIndicatePos - 2) / pixBetweenPoint;

		if ((pointOrd + startPointNum + 1) >= exchangeDatFileList.size()) 
			timeVal = 0;
		else 
		{
			timeVal = exchangeDatFileList.at(pointOrd + startPointNum)->at(1);
		}
	}
	catch (...)
	{
		QMessageBox::information(this, "message", "error");
	}
	emit changeRightValueSignal(timeVal);
#endif
}

void  CRightRenderArea::SetSecondIndicatePos(int nPos)
{
	if (nPos != m_nSecondIndicatePos)
	{
		update();
	}
	m_nSecondIndicatePos = nPos;
	if (m_nSecondIndicatePos <= 3)
		m_nSecondIndicatePos = 3;
}

void CRightRenderArea::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Left:
	{
		if ((this->m_nFirstIndicatePos) <= 2)
		{
			//emit drawPointValueSignal(0);
			SetFirstIndicatePos(2);
		}
		else
		{
			//emit drawPointValueSignal((curIndicatePos - pixBetweenPoint - 2) / pixBetweenPoint);
			Q_ASSERT(m_pChannelViewInfo);
			if (m_pChannelViewInfo)
			{
				SetFirstIndicatePos(m_nFirstIndicatePos - m_pChannelViewInfo->GetPixBetweenPoint());
			}
		}
		break;
	}
	case Qt::Key_Right:
		//emit drawPointValueSignal((curIndicatePos + pixBetweenPoint - 2) / pixBetweenPoint);
		Q_ASSERT(m_pChannelViewInfo);
		if (m_pChannelViewInfo)
		{
			if (m_nFirstIndicatePos + m_pChannelViewInfo->GetPixBetweenPoint() < width() - 2)
			{
				SetFirstIndicatePos((this->m_nFirstIndicatePos) + m_pChannelViewInfo->GetPixBetweenPoint());
			}
		}
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}