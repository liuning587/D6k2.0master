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
    // ��ʱ�����ϵļ����  
	void DrawTimeInterval(QPainter & painter);

	void DrawWave(QPainter & painter);
	//���Գ���
	void DrawSymmetryLine(QPainter & painter);

	void DrawBackground(QPainter &painter);

	void HighLightChannel(int nYValue);

	void SetFirstIndicatePos(int nPos);
	void SetSecondIndicatePos(int nPos);

private:
	int m_nFirstIndicatePos;     //! ָʾ�ߵ�ǰx����
	int m_nSecondIndicatePos;    //! ��һָʾ��
					   //int formerPos;
					   //bool indicatePosFlag;

//	int m_nMoveIndicateLine; //! �ƶ� ָʾ��  0 û�� 
	MoveIndicateLine  m_nMoveIndicateLine;

	int startChannelNum;
//	int m_nStartPointNum;

// 	int m_nChannelRectHeight;   //! ÿ��ͨ������ռ�ľ��εĸ߶�
// 	int m_nChannelMargin;       //! ͨ�����������֮��ı߾�
// 	int m_nWaveRectHeight;      //! �������ߵĸ߶�  = m_nChannelRectHeight - 2*m_nChannelMargin 
// 	int m_nPixBetweenPoint;     //! ���ڵ�֮������ؼ��
// 
// 	int m_nPixBetweenMaxMid;//������ʱ���ֵ���м�ֵ�����pixBetweenMaxMid�����ر�ʾ
// 
// 	int m_nPointsPerScreen;
	int len_25;  // //ˮƽ��������һ���ƶ�25��������  

	bool exchangeCurveFlag;//�������߱�ʶ
	int exchangeChannelNum1;//����������ͨ����ʶ1
	int exchangeChannelNum2;//����������ͨ����ʶ1

	std::shared_ptr<CComtrade>  m_pWave;

	std::shared_ptr<CChannelViewInfo> m_pChannelViewInfo;

//	std::vector<std::shared_ptr<CAnaChannelViewInfo>> m_arrAnaChannelInfos;
};


#endif // RIGHT_VIEW_H