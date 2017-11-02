#pragma once

#include "base_widget.h"
#include <QQuickWidget>


class QMouseEvent;
class CSurface3dModel;

class CSurface3DWidget : public CBaseWidget
{
	Q_OBJECT

public:
	CSurface3DWidget();
	virtual ~CSurface3DWidget();
public:
	enum THREE_D_TYPE
	{
		//����ͼ
		SURFACE_3D = 0,
	};

public:

	CSurface3DWidget(QRectF rcPos, CSurface3DWidget::THREE_D_TYPE shape);
public:
// 	void AddWidget(QWidget *pWgt);
	//������ 
	void InitModelData(CSurface3dModel *tModel);

public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	// ��ת�����ĵ� 
	virtual QPointF GetRotationCenterPoint()const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	// 	virtual void DrawSingleSelectedTracker(QPainter *pPainter);
	// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);

public:
	//��ȡ����ģ��
	CSurface3dModel *GetSurfaceModel()
	{
		return m_p3dModel;
	}
	//��ȡ�߽���
	int GetInterval()
	{
		return m_nInterval;
	}

	//���ñ߽���
	void SetInterval(int nVal)
	{
		m_nInterval = nVal;
	}

private:
	THREE_D_TYPE m_Shape;

	QGraphicsProxyWidget *m_pProxyWgt;
	//surface����ģ��
	CSurface3dModel *m_p3dModel;
	//quick view
	QQuickWidget *m_pQuickView;
	//�߽���  ���
	int m_nInterval;

};
