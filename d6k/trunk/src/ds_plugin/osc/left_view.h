#ifndef LEFT_VIEW_H
#define LEFT_VIEW_H
 
#include <QWidget>

#include <memory>


class QLineEdit;
class CLeftRenderArea;
class CBorderLayout;
class CComtrade;
class CChannelViewInfo;

class CLeftView : public QWidget
{
	Q_OBJECT
public:
	CLeftView(std::shared_ptr<CComtrade> pWave,QWidget *parent = nullptr);
	~CLeftView();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

public:
	CLeftRenderArea * GetRenderArea()
	{
		return m_pLeftRender.get();
	}

	std::shared_ptr<CLeftRenderArea> GetRenderAreaPtr() 
	{
		return m_pLeftRender;
	}
	void Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo);

//public:


//signals:

//public slots :
//	void OnCoilDoubleClicked(const QModelIndex & index);
protected:
	void Destroy();
 

private:
	//QLineEdit * m_pPointNumEdit;
	std::shared_ptr<QLineEdit>  m_pPointNumEdit;

	//CLeftRenderArea * m_pLeftRender;
	std::shared_ptr<CLeftRenderArea> m_pLeftRender;

	CBorderLayout *m_pLayout;

	std::shared_ptr<CComtrade>  m_pWave;
};


class CLeftRenderArea : public QWidget
{
	Q_OBJECT
public:
	CLeftRenderArea(std::shared_ptr<CComtrade> pWave, QWidget *parent = 0);

	void Initial(std::shared_ptr<CChannelViewInfo> pChannelViewInfo);
public slots:
    void ShowFirstIndicatePointNumberSlot(int nPos);
protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
	void resizeEvent(QResizeEvent *event);
protected:
	void DrawBackground(QPainter &painter);
	void DrawDivisionLine(QPainter & painter);
	void DrawTimeRect(QPainter & painter);
	void DrawTriangle(QPainter & painter);

	void DrawChannelName(QPainter &painter);

	void DrawIndicateValue(QPainter &painter);
private:
	int m_nCurTimePos;//当前时间线位置(点号位置)
	int m_nStartChannelNum;//起始通道号
	int m_nStartPointNum;//起始点号
	int m_nDivision;//分割线之间的距离



					//设置曲线颜色
	QList<QColor> m_lstColor;

	std::shared_ptr<CChannelViewInfo> m_pChannelViewInfo;
	std::shared_ptr<CComtrade>  m_pWave;
};


#endif // LEFT_VIEW_H