#ifndef CHANNEL_VIEW_INFO_H
#define CHANNEL_VIEW_INFO_H 

#include <vector>
#include <QRectF>
#include <memory>


class QPointF;
class QPainter;
class CAnaChannel;
class CRightRenderArea;
class CLeftRenderArea;
class CChannelViewInfo;

class CAnaChannelViewInfo
{
public:
	CAnaChannelViewInfo(CAnaChannel & channel, int& nWaveRectHeight, int nWaveXAxisYCoord);
	// 波形所占高度，通道上方位置，通道高度
	CAnaChannelViewInfo(CAnaChannel & channel, int& nWaveRectHeight, int nRectTop,int nRectHeight,int nRectWidth);

	~CAnaChannelViewInfo()
	{
		m_arrRawPoints.clear();
	}
public:
	// 画背景及中间坐标轴
	void DrawLeftBackground(CLeftRenderArea *pRender, QPainter & painter);
	void DrawLeftIndicateValue(CLeftRenderArea *pRender, QPainter & painter, int nT1PointNum, int nT2PointNum);

	void DrawRightBackground(CRightRenderArea *pRender, QPainter & painter);
	// 画波形
	void DrawWave(CRightRenderArea *pRender, CChannelViewInfo *pViewInfo, QPainter & painter);

	void OnUpdate(CRightRenderArea *pRender, CChannelViewInfo *pViewInfo, int nStartNum);

	bool IsInRightRect(int nYValue) const
	{
		if (nYValue>= m_rcRightPos.top() && nYValue < m_rcRightPos.bottom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsInLeftRect(int nYValue) const
	{
		if (nYValue >= m_rcLeftPos.top() && nYValue < m_rcLeftPos.bottom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool SetHighLight(bool bHighLight, CLeftRenderArea *pRender);
	bool SetHighLight(bool bHighLight, CRightRenderArea *pRender);

	void SetRightRectSize(int nWidth);
	void SetLeftRectSize(int nWidth);
protected:
	CAnaChannelViewInfo() = delete;

private:
	//! 原始的位置
	std::vector<QPointF> m_arrRawPoints;
	//! 变化后的曲线
	std::vector<QPointF> m_arrDrawPoints;

	CAnaChannel & m_channel;

	QRectF  m_rcRightPos; //! 位置坐标
	QRectF  m_rcLeftPos;  //! 左边坐标

	int &m_nWaveRectHeight;

	int m_nWaveXAxisYCoord; //! 波形的X轴在视口的Y坐标

	int m_nStartNum;

	bool m_bHighLight;   //! 高亮
};

class CComtrade;
class CChannelViewInfo
{
public:
	CChannelViewInfo(std::shared_ptr<CComtrade> pWave);
	~CChannelViewInfo();
protected:
	CChannelViewInfo();

public:
	bool Initial(int& nWaveRectHeight, int nRectHeight);

	const std::vector<std::shared_ptr<CAnaChannelViewInfo>> & GetAnaChannelViewInfo()
	{
		return m_arrAnaChannelInfos;
	}

	int GetViewPortPointNum() const
	{
		return m_nPointsPerScreen;
	}

	int GetStartPointNum()const
	{
		return m_nStartPointNum;
	}

	void SetLeftRender(CLeftRenderArea *pLeft)
	{
		m_pLeftRender = pLeft;
	}

	void SetRightRender(CRightRenderArea *pRight)
	{
		m_pRightRender = pRight;
	}

	void SetStartPointNum(int nStartPointNum)
	{
		m_nStartPointNum = nStartPointNum;
	}

	int GetPixBetweenMaxMid() const
	{
		return m_nPixBetweenMaxMid;
	}

	int GetPixBetweenPoint()const
	{
		return m_nPixBetweenPoint;
	}
	
	bool SetPointNumPerScreen(int nViewportWidth);

	void SetRightRectSize(int nWidth);
	void SetLeftRectSize(int nWidth);
	
	void OnUpdate(CRightRenderArea *pRender, int nStartNum);

	void RightHighLightChannel(int nYValue, CRightRenderArea * pRender);
	void LeftHighLightChannel(int nYValue, CLeftRenderArea * pRender);

	void DrawWave(CRightRenderArea * pRender, QPainter & painter);
	
	void DrawLeftBackground(CLeftRenderArea * pRender, QPainter &painter);
	void DrawLeftIndicateValue(CLeftRenderArea *pRender, QPainter & painter,int nT1PointNum,int nT2PointNum);

	void DrawRightBackground(CRightRenderArea * pRender, QPainter &painter);
private:

	std::vector<std::shared_ptr<CAnaChannelViewInfo>> m_arrAnaChannelInfos;

	std::shared_ptr<CComtrade>  m_pWave;

	int startChannelNum;
	int m_nStartPointNum;

	int m_nChannelRectHeight;   //! 每个通道的所占的矩形的高度
	int m_nChannelMargin;       //! 通道框架与内容之间的边距
	int m_nWaveRectHeight;      //! 波形曲线的高度  = m_nChannelRectHeight - 2*m_nChannelMargin 
	int m_nPixBetweenPoint;     //! 相邻点之间的象素间隔

	int m_nPixBetweenMaxMid;    //！ 画波形时最大值与中间值差就用pixBetweenMaxMid个象素表示

	int m_nPointsPerScreen;

	CLeftRenderArea *m_pLeftRender;
	CRightRenderArea * m_pRightRender;
};


#endif // CHANNEL_VIEW_INFO_H
