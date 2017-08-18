/*! @file graphview.h
<PRE>
********************************************************************************
模块名       :
文件名       :  graphview.h
文件实现功能 :  图层
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图层
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#ifndef REALTIME_GRAPH_VIEW_H
#define REALTIME_GRAPH_VIEW_H
 
#include <QGraphicsView>
#include <vector> 
#include <QMap>
enum
{
	OPER_TYPE_SELECT = 1,
	OPER_TYPE_PRESET = 2,
	OPER_TYPE_EXEC = 3,
	OPER_TYPE_DEXEC = 4,
};

class CGraphFile;
class CBaseWidget;
class CPenInfo;
class CBrushInfo;
class CFontInfo;
class CGraphScene;

class CRealTimeView : public QGraphicsView
{
    Q_OBJECT
public:
	CRealTimeView(QWidget *parent = 0);
	CRealTimeView(QGraphicsScene *scene, QWidget * parent = 0);

	~CRealTimeView();

protected:
	void OnLButtonDown(QMouseEvent * e);
	CBaseWidget* HitTest(QPoint & pos, unsigned int nLayerIdx);
protected:
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	//返回场景
	CGraphScene *GetGraphicsScene() const; 

	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);
	virtual void mouseDoubleClickEvent(QMouseEvent * e);
Q_SIGNALS:
	void sig_BtnOrderType(const QString& tagName,int nType);

public:
	void SetLayerIndex(int nIndex)
	{
		Q_ASSERT(nIndex >= 0);
		m_nLayerIndex = nIndex;
	}

	int GetLayerIndex() const
	{
		return m_nLayerIndex;
	}

private:
	int m_nLayerIndex;
};

#endif // REALTIME_GRAPH_VIEW_H

/** @}*/

