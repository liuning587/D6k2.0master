/*! @file graphview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graphview.h
�ļ�ʵ�ֹ��� :  ͼ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ��
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
	//���س���
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

