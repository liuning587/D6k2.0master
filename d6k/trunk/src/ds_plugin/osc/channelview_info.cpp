

#if _MSC_VER >= 1600
#define _WIN32_WINNT  0x0500
#endif

#if 0
#if _MSC_VER >= 1600
#include <vld.h>
#endif
#endif

#include "channelview_info.h"
#include "wave_info.h"
#include "right_view.h"
#include "left_view.h"
#include "childfrm.h"

#include <QPointF>
#include <QPainter>

CAnaChannelViewInfo::CAnaChannelViewInfo(CAnaChannel & channel, int& nWaveRectHeight, int nRectTop, int nRectHeight, int nRectWidth):m_channel(channel), m_nWaveRectHeight(nWaveRectHeight)
{
	m_bHighLight = false;

	m_rcRightPos.setLeft(0);
	m_rcRightPos.setTop(nRectTop);
	m_rcRightPos.setHeight(nRectHeight);
	m_rcRightPos.setWidth(nRectWidth);

	m_rcLeftPos = m_rcRightPos;

	m_nWaveXAxisYCoord = nRectTop + nRectHeight / 2;

	std::vector<float> & arrData = m_channel.GetSampData();
	float fMaxValue = m_channel.GetMaxSampValue();
	float fMinValue = m_channel.GetMinSampValue();

// 	float fMaxValue = m_channel.GetMaxValue();
// 	float fMinValue = m_channel.GetMinValue();

	int i = 0;

	m_arrRawPoints.reserve(arrData.size());

	double y = 0.0f;

	for each (float fValue in arrData)
	{
		if (fMaxValue == fMinValue)
		{
			y = m_nWaveXAxisYCoord;
		}
		else
		{
			Q_ASSERT(fValue >= fMinValue && fValue <= fMaxValue);
			y = m_nWaveXAxisYCoord - m_nWaveRectHeight / 2 + (m_nWaveRectHeight-(fValue - fMinValue)*m_nWaveRectHeight / (fMaxValue - fMinValue));
		}

		QPointF ptVal(2 * i, y);
		m_arrRawPoints.emplace_back(ptVal);

		i++;
	}
	m_arrDrawPoints = m_arrRawPoints;
}

CAnaChannelViewInfo::CAnaChannelViewInfo(CAnaChannel & channel, int& nWaveRectHeight, int nWaveXAxisYCoord) :m_channel(channel), m_nWaveRectHeight(nWaveRectHeight), m_nWaveXAxisYCoord(nWaveXAxisYCoord)
{
	std::vector<float> & arrData = m_channel.GetSampData();
	float fMaxValue = m_channel.GetMaxSampValue();
	float fMinValue = m_channel.GetMinSampValue();

	int i = 0;

	m_arrRawPoints.reserve(arrData.size());

	double y = 0.0f;

	for each (float fValue in arrData)
	{
		if (fMaxValue == fMinValue)
		{
			y = m_nWaveXAxisYCoord ;
		}
		else
		{
			Q_ASSERT(fValue >= fMinValue && fValue <= fMaxValue);
			y = m_nWaveXAxisYCoord - m_nWaveRectHeight / 2 + (fValue - fMinValue)*m_nWaveRectHeight / (fMaxValue - fMinValue);
		}

		QPointF ptVal(2 * i, y);
		m_arrRawPoints.emplace_back(ptVal);

		i++;
	}
	m_arrDrawPoints = m_arrRawPoints;
}

void CAnaChannelViewInfo::DrawLeftIndicateValue(CLeftRenderArea *pRender, QPainter & painter, int nT1PointNum, int nT2PointNum)
{
	Q_ASSERT(pRender);
	if (pRender == nullptr)
		return;

	QRect viewPort = painter.viewport();
	int nWidth = viewPort.width();
	
	// 画通道名称
	int nPad = m_rcLeftPos.height() / 10;

	QString szName = m_channel.GetName();

	float fValue = 0.0f;
	bool bRet = m_channel.GetValue(nT1PointNum, fValue);

	painter.setPen(Qt::black);

	if (bRet)
	{
		QString szUnit = m_channel.GetUnit();
		if (szUnit.isEmpty() == false)
		{
			szName += QString(" [%1] ").arg(szUnit);
		}
		szName += QString(" [T1 = %1]").arg(fValue);
		painter.drawText(QRect(nPad, nPad + m_rcLeftPos.top(), nWidth * 2 / 3, m_rcLeftPos.height() - 2 * nPad), Qt::AlignVCenter | Qt::AlignLeft, szName);
	}
	else
	{
		if (szName.isEmpty() == false)
		{
			painter.drawText(QRect(nPad, nPad + m_rcLeftPos.top(), nWidth * 2 / 3, m_rcLeftPos.height() - 2 * nPad), Qt::AlignVCenter | Qt::AlignLeft, szName);
		}
	}

// 	if (szName.isEmpty() == false)
// 	{
// 		painter.drawText(QRect(nPad, nPad + m_rcLeftPos.top(), nWidth * 2 / 3, m_rcLeftPos.height() - 2 * nPad), Qt::AlignVCenter | Qt::AlignLeft, szName);
// 	}
}

void CAnaChannelViewInfo::DrawLeftBackground(CLeftRenderArea *pRender, QPainter & painter)
{
	Q_ASSERT(pRender);
	if (pRender == nullptr)
		return;

	QRect viewPort = painter.viewport();
	int nWidth = viewPort.width();

	m_rcLeftPos.setWidth(nWidth);

	if (m_bHighLight)
	{
		QColor crHighLight = Qt::lightGray;
		crHighLight = crHighLight.lighter(10);
	}
	else
	{
		painter.fillRect(this->m_rcLeftPos, QBrush(Qt::lightGray));
	}

	QPen pen = painter.pen();
	pen.setColor(Qt::black);
	pen.setStyle(Qt::DashLine);
	painter.setRenderHint(QPainter::Antialiasing);

	// 画矩形框
	pen.setColor(Qt::darkGray);
	painter.drawLine(0, m_rcLeftPos.bottom(), m_rcLeftPos.right(), m_rcLeftPos.bottom());
	// 画三角形指示
	//for (int i = 0; i < 7; i++)
	{
	//	painter.setBrush(QBrush(m_lstColor.at(i % 7)));
// 		QBrush br = painter.brush();
// 		br.setColor(Qt::blue);
		painter.setBrush(QBrush(Qt::blue));
		QPointF points[3] =
		{
			QPointF(nWidth, m_rcLeftPos.top()+m_rcLeftPos.height()/2),
			QPointF((nWidth - 10),m_rcLeftPos.top() + m_rcLeftPos.height() / 2 - 6),
			QPointF((nWidth - 10),m_rcLeftPos.top() + m_rcLeftPos.height() / 2 + 6)
		};
		painter.drawPolygon(points, 3);		
	}
}
/*! \fn void CAnaChannelViewInfo::DrawBackground(CRightRenderArea *pRender, QPainter & painter)
********************************************************************************************************* 
** \brief CAnaChannelViewInfo::DrawBackground 
** \details 画曲线背景
** \param pRender 
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月16日 
** \note 
********************************************************************************************************/
void CAnaChannelViewInfo::DrawRightBackground(CRightRenderArea *pRender, QPainter & painter)
{
	Q_ASSERT(pRender);
	if (pRender == nullptr)
		return;

	QRect viewPort = painter.viewport();
	int nWidth = viewPort.width();

	m_rcRightPos.setWidth(nWidth);

	painter.save();
	//painter.setRenderHint(QPainter::Antialiasing);
//	painter.fillRect(this->m_rcPos, QBrush(Qt::white));
	if (m_bHighLight)
	{
		QColor crHighLight = Qt::lightGray;
		crHighLight = crHighLight.lighter(10);
	}
	else
	{
		painter.fillRect(this->m_rcRightPos, QBrush(Qt::lightGray));
	}

	QPen pen = painter.pen();
	pen.setColor(Qt::black);
	pen.setStyle(Qt::DashLine);
	painter.setRenderHint(QPainter::Antialiasing);
	// 中间的坐标轴 
	painter.drawLine(0, m_nWaveXAxisYCoord, nWidth, m_nWaveXAxisYCoord);
	// 画矩形框
	pen.setColor(Qt::darkGray);
	painter.drawLine(0, m_rcRightPos.bottom(), m_rcRightPos.right(), m_rcRightPos.bottom());

	painter.restore();
}
/*! \fn void CAnaChannelViewInfo::DrawWave(CRightRenderArea *pRender, CChannelViewInfo *pViewInfo, QPainter & painter)
********************************************************************************************************* 
** \brief CAnaChannelViewInfo::DrawWave 
** \details 绘制波形曲线
** \param pRender 
** \param pViewInfo 
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月19日 
** \note 
********************************************************************************************************/
void CAnaChannelViewInfo::DrawWave(CRightRenderArea *pRender, CChannelViewInfo *pViewInfo, QPainter & painter)
{
	Q_ASSERT(pRender);
	if (pRender == nullptr)
		return;

	Q_ASSERT(pViewInfo);
	if (pViewInfo == nullptr)
		return;

	int nPointNum = pViewInfo->GetViewPortPointNum();

	if (nPointNum != m_arrDrawPoints.size())
	{// 发生变化了
		OnUpdate(pRender, pViewInfo, pViewInfo->GetStartPointNum());
	}

	painter.save();

	if (m_arrDrawPoints.empty() == false)
	{
		painter.drawPolyline(m_arrDrawPoints.data(), static_cast<int>(m_arrDrawPoints.size()));
	}
	// 如果画测值点
	if (m_arrDrawPoints.empty() == false)
	{
		QRectF rcRound;	
		int nIdx = 0;

		for each (auto pt in m_arrDrawPoints)
		{
			if (nIdx % 10 == 0)
			{
				rcRound.setX(pt.x() - 1);
				rcRound.setY(pt.y() - 1);
				rcRound.setHeight(3);
				rcRound.setWidth(3);

				QPainterPath path;

				path.addEllipse(rcRound);
				painter.drawRect(rcRound);

// 				QBrush br = painter.brush();
// 				br.setColor(Qt::green);
// 				painter.fillPath(path, br);
			}
			nIdx++;	
		}
		//	rcRound.setX()
	}
	painter.restore();
}


void  CAnaChannelViewInfo::OnUpdate(CRightRenderArea *pRender, CChannelViewInfo *pViewInfo, int nStartNum)
{
	Q_ASSERT(pRender);
	if (pRender == nullptr)
		return;

	Q_ASSERT(nStartNum >= 0);
	if (nStartNum < 0)
		return;

	// 溢出了
	if (nStartNum >= m_arrRawPoints.size())
		return;

	m_nStartNum = nStartNum;

	int nPointNum = pViewInfo->GetViewPortPointNum();
	m_arrDrawPoints.clear();
	m_arrDrawPoints.reserve(nPointNum);	

	int i = 0;
	int nXCoord = 0;
	for (i = m_nStartNum; i < m_arrRawPoints.size() && i <= nPointNum + m_nStartNum; i++)
	{
		QPointF ptVal = m_arrRawPoints.at(i);

		ptVal.setX(nXCoord * 2);
		m_arrDrawPoints.emplace_back(ptVal);
		nXCoord++;
	}	 
}
/*! \fn void  CAnaChannelViewInfo::SetHighLight(bool bHighLight, CRightRenderArea *pRender)
********************************************************************************************************* 
** \brief CAnaChannelViewInfo::SetHighLight 
** \details 高亮曲线矩形
** \param bHighLight 
** \param pRender 
** \return bool  true 说明发生了改变
** \author LiJin 
** \date 2018年2月16日 
** \note 
********************************************************************************************************/
bool  CAnaChannelViewInfo::SetHighLight(bool bHighLight, CRightRenderArea *pRender)
{
	bool bRet = false;
	if (bHighLight != m_bHighLight)
	{
		if (pRender)
		{
			pRender->update();
		}
		bRet = true;
	}
	m_bHighLight = bHighLight;
	return bRet;
}

bool  CAnaChannelViewInfo::SetHighLight(bool bHighLight, CLeftRenderArea *pRender)
{
	bool bRet = false;
	if (bHighLight != m_bHighLight)
	{
		if (pRender)
		{
			pRender->update();
		}
		bRet = true;
	}
	m_bHighLight = bHighLight;
	return bRet;
}


void  CAnaChannelViewInfo::SetRightRectSize(int nWidth)
{
	Q_ASSERT(nWidth >= 0);
	if (nWidth >=0)
	{
		m_rcRightPos.setWidth(nWidth);
	}
}

void  CAnaChannelViewInfo::SetLeftRectSize(int nWidth)
{
	Q_ASSERT(nWidth >= 0);
	if (nWidth >= 0)
	{
		m_rcLeftPos.setWidth(nWidth);
	}
}

CChannelViewInfo::CChannelViewInfo()
{
	startChannelNum = 0;
	m_nStartPointNum = 0;

	m_nChannelRectHeight = 100;
	m_nChannelMargin = 5;

	m_nWaveRectHeight = m_nChannelRectHeight - 2 * m_nChannelMargin;

	m_nPixBetweenPoint = 2;
	m_nPixBetweenMaxMid = 20;

	m_pLeftRender = nullptr;
	m_pRightRender = nullptr;
}

CChannelViewInfo::CChannelViewInfo(std::shared_ptr<CComtrade> pWave):m_pWave(pWave)
{
	m_pLeftRender = nullptr;
	m_pRightRender = nullptr;
	startChannelNum = 0;
	m_nStartPointNum = 0;

	m_nChannelRectHeight = 100;
	m_nChannelMargin = 5;

	m_nWaveRectHeight = m_nChannelRectHeight - 2 * m_nChannelMargin;

	m_nPixBetweenPoint = 2;
	m_nPixBetweenMaxMid = 20;

	Q_ASSERT(pWave);
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
				pAnaViewInfo = std::make_shared<CAnaChannelViewInfo>(**it, m_nWaveRectHeight, nIndex*m_nChannelRectHeight, m_nChannelRectHeight, 0);

				m_arrAnaChannelInfos.emplace_back(pAnaViewInfo);
				nIndex++;
			}
		}
	}
}

CChannelViewInfo::~CChannelViewInfo()
{

}

bool CChannelViewInfo::Initial(int& nWaveRectHeight, int nRectHeight)
{
	Q_ASSERT(m_pWave);
	if (m_pWave)
	{
		const std::shared_ptr<CWaveCfg> & pCfg = m_pWave->GetConfig();
		Q_ASSERT(pCfg);
		if (pCfg)
		{
			int nIndex = 0;
			std::vector<std::shared_ptr<CAnaChannel>> & arrAna = pCfg->GetAnaChannels();

			std::shared_ptr<CAnaChannelViewInfo> pAnaViewInfo;

			for (auto it = arrAna.cbegin(); it != arrAna.cend(); ++it)
			{
				//	pAnaViewInfo = std::make_shared<CAnaChannelViewInfo>(**it, m_nWaveRectHeight, nIndex*m_nChannelRectHeight + m_nChannelRectHeight / 2);
				pAnaViewInfo = std::make_shared<CAnaChannelViewInfo>(**it, nWaveRectHeight, nIndex*nRectHeight, nRectHeight, 0);

				m_arrAnaChannelInfos.emplace_back(pAnaViewInfo);
				nIndex++;
			}
		}
	}
	return true;
}
/*! \fn bool CChannelViewInfo::SetPointNumPerScreen(int nViewportWidth)
********************************************************************************************************* 
** \brief CChannelViewInfo::SetPointNumPerScreen 
** \details 设置每一屏幕显示的点数
** \param nViewportWidth 
** \return void 
** \author LiJin 
** \date 2018年2月18日 
** \note 
********************************************************************************************************/
bool CChannelViewInfo::SetPointNumPerScreen(int nViewportWidth)
{
	Q_ASSERT(m_nPixBetweenPoint != 0);
	if (m_nPixBetweenPoint != 0)
	{
		if (nViewportWidth>0)
		{
			int nPointsPerScreen = static_cast<int>(nViewportWidth / m_nPixBetweenPoint) + 1;
			if (m_nPointsPerScreen != nPointsPerScreen)
			{
				m_nPointsPerScreen = nPointsPerScreen;
				return true;
			}
		}
	}
	return false;
}
/*! \fn void CChannelViewInfo::DrawWave(CRightRenderArea * pRender, QPainter & painter)
********************************************************************************************************* 
** \brief CChannelViewInfo::DrawWave 
** \details 画波形
** \param pRender 
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月18日 
** \note 
********************************************************************************************************/
void CChannelViewInfo::DrawWave(CRightRenderArea * pRender, QPainter & painter)
{
	painter.save();

	painter.setRenderHint(QPainter::Antialiasing);
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->DrawWave(pRender,this, painter);
		}
	}
	painter.restore();
}

void CChannelViewInfo::DrawLeftBackground(CLeftRenderArea * pRender, QPainter &painter)
{
	painter.save();
	// 画第一个线
	QPen pen = painter.pen();
	pen.setColor(Qt::darkGray);
	painter.drawLine(0, 0, pRender->width(), 0);

	//	painter.setRenderHint(QPainter::Antialiasing);
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->DrawLeftBackground(pRender, painter);
		}
	}
	painter.restore();
}
void  CChannelViewInfo::DrawLeftIndicateValue(CLeftRenderArea *pRender, QPainter & painter, int nT1PointNum, int nT2PointNum)
{
	painter.save();
	
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->DrawLeftIndicateValue(pRender, painter,nT1PointNum,nT2PointNum);
		}
	}
	painter.restore();
}

/*! \fn void  CChannelViewInfo::DrawRightBackground(CRightRenderArea * pRender, QPainter &painter)
********************************************************************************************************* 
** \brief CChannelViewInfo::DrawRightBackground 
** \details 绘制右边背景
** \param pRender 
** \param painter 
** \return void 
** \author LiJin 
** \date 2018年2月19日 
** \note 
********************************************************************************************************/
void  CChannelViewInfo::DrawRightBackground(CRightRenderArea * pRender, QPainter &painter)
{
	painter.save();
	// 画第一个线
	QPen pen = painter.pen();
	pen.setColor(Qt::darkGray);
	painter.drawLine(0, 0, pRender->width(),0);

//	painter.setRenderHint(QPainter::Antialiasing);
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->DrawRightBackground(pRender, painter);
		}
	}
	painter.restore();
}
/*! \fn void  CChannelViewInfo::OnUpdate(CRightRenderArea *pRender, int nStartPointNum)
********************************************************************************************************* 
** \brief CChannelViewInfo::OnUpdate 
** \details 更新显示的起始点，通常是移动滚动条时调用
** \param pRender 
** \param nStartPointNum 
** \return void 
** \author LiJin 
** \date 2018年2月18日 
** \note 
********************************************************************************************************/
void  CChannelViewInfo::OnUpdate(CRightRenderArea *pRender, int nStartPointNum)
{
	Q_ASSERT(pRender);
	if (pRender == nullptr)
		return;

	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->OnUpdate(pRender,this, nStartPointNum);
		}
	}
}

void CChannelViewInfo::RightHighLightChannel(int nYValue, CRightRenderArea * pRender)
{
	bool bRet = false;
	bool bChanage = false;
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			//	pInfo->OnUpdate(this, nStartPointNum);
			bRet = pInfo->IsInRightRect(nYValue);
			bChanage |= pInfo->SetHighLight(bRet, pRender);
		}
	}
	if (bChanage)
	{// 同步更新左边视图
		Q_ASSERT(m_pLeftRender);
		if (m_pLeftRender)
		{
			m_pLeftRender->update();
		}
	}
}

void CChannelViewInfo::LeftHighLightChannel(int nYValue, CLeftRenderArea * pRender)
{
	bool bRet = false;
	bool bChanage = false;

	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			bRet = pInfo->IsInLeftRect(nYValue);
			bChanage |= pInfo->SetHighLight(bRet, pRender);
		}
	}
	if (bChanage)
	{// 同步更新右边视图
		Q_ASSERT(m_pRightRender);
		if (m_pRightRender)
		{
			m_pRightRender->update();
		}
	}	
}

void  CChannelViewInfo::SetRightRectSize(int nWidth)
{
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->SetRightRectSize(nWidth);	
		}
	}
}

void  CChannelViewInfo::SetLeftRectSize(int nWidth)
{
	for each (auto pInfo in m_arrAnaChannelInfos)
	{
		Q_ASSERT(pInfo);
		if (pInfo)
		{
			pInfo->SetRightRectSize(nWidth);
		}
	}
}