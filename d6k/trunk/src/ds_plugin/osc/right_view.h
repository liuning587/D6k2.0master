#ifndef RIGHT_VIEW_H
#define RIGHT_VIEW_H

#include <QWidget> 
#include <QScrollBar>

#include <memory>

class CBorderLayout; 
class CRightRenderArea;
class CComtrade;
class CAnaChannelViewInfo;
class CChannelViewInfo;

class CRightView : public QWidget
{
	Q_OBJECT
public:
	CRightView(std::shared_ptr<CComtrade> pWave, QWidget *parent = nullptr);
	~CRightView();

public:
	void Initial(std::shared_ptr<CChannelViewInfo> m_pChannelViewInfo);

	CRightRenderArea * GetRenderArea()
	{
		return m_pRightRender.get();
	}

	std::shared_ptr<CRightRenderArea> GetRenderAreaPtr()
	{
		return m_pRightRender;
	}

	//signals:

	//public slots :
	//	void OnCoilDoubleClicked(const QModelIndex & index);
protected:
	void Destroy();
private:
	std::shared_ptr<CRightRenderArea> m_pRightRender;

	std::shared_ptr<QScrollBar> m_pHorzScrollBar;
	std::shared_ptr<QScrollBar> m_pVertScrollBar;

	CBorderLayout *m_pLayout;

	std::shared_ptr<CComtrade>  m_pWave;
};



class CRightRenderArea : public QWidget
{
	Q_OBJECT
public:
	CRightRenderArea(std::shared_ptr<CComtrade> pWave, QWidget *parent = nullptr);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	enum MoveIndicateLine
	{
		IndicateLineNone = 0,
		IndicateLineFirst = 1,
		IndicateLineSecond = 2,
	};

public slots:
	void SetStartChannelNum(int nChannelNum);
	void SetStartPointNum(int nStartPointNum);
signals:
	void ShowFirstIndicatePointNumberSignal(int nPos);
public:
	void Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo);


protected:
	void paintEvent(QPaintEvent *event);
  	void mousePressEvent(QMouseEvent * event);
 	void keyPressEvent(QKeyEvent *event);
// 	void enterEvent(QEvent * event);
  	void mouseMoveEvent(QMouseEvent * event);
// 	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

protected:
	void DrawIndicateLine(QPainter & painter, int x);
	void DrawIndicateLine2(QPainter & painter, int x);
	void DrawChannelRect(QPainter & painter);
	void DrawTimeLine(QPainter & painter);
    // 画时间轴上的间隔线  
	void DrawTimeInterval(QPainter & painter);

	void DrawWave(QPainter & painter);
	//画对称轴
	void DrawSymmetryLine(QPainter & painter);

	void DrawBackground(QPainter &painter);

	void HighLightChannel(int nYValue);

	void SetFirstIndicatePos(int nPos);
	void SetSecondIndicatePos(int nPos);

private:
	int m_nFirstIndicatePos;     //! 指示线当前x坐标
	int m_nSecondIndicatePos;    //! 另一指示线
					   //int formerPos;
					   //bool indicatePosFlag;

//	int m_nMoveIndicateLine; //! 移动 指示线  0 没有 
	MoveIndicateLine  m_nMoveIndicateLine;

	int startChannelNum;
//	int m_nStartPointNum;

// 	int m_nChannelRectHeight;   //! 每个通道的所占的矩形的高度
// 	int m_nChannelMargin;       //! 通道框架与内容之间的边距
// 	int m_nWaveRectHeight;      //! 波形曲线的高度  = m_nChannelRectHeight - 2*m_nChannelMargin 
// 	int m_nPixBetweenPoint;     //! 相邻点之间的象素间隔
// 
// 	int m_nPixBetweenMaxMid;//画波形时最大值与中间值差就用pixBetweenMaxMid个象素表示
// 
// 	int m_nPointsPerScreen;
	int len_25;  // //水平滚动条点一次移动25个采样点  

	bool exchangeCurveFlag;//交换曲线标识
	int exchangeChannelNum1;//被交换曲线通道标识1
	int exchangeChannelNum2;//被交换曲线通道标识1

	std::shared_ptr<CComtrade>  m_pWave;

	std::shared_ptr<CChannelViewInfo> m_pChannelViewInfo;

//	std::vector<std::shared_ptr<CAnaChannelViewInfo>> m_arrAnaChannelInfos;
};


#endif // RIGHT_VIEW_H