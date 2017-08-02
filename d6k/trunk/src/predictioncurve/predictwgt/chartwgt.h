#pragma once

#include <QWidget>
#include <QSplineSeries>
#include <QLegendMarker>
#include <QDebug>
#include <QChartView>
#include <QChart>
#include <QGridLayout>

using namespace QtCharts;

class CChartWgt : public QWidget
{
	Q_OBJECT

public:
	CChartWgt(QWidget *parent);
	~CChartWgt();
	//��������ͼ����
	void SetChartTitle(const QString &strName);
	//���ñ��������ʽ
	void SetTitleFont(const QFont &Font);
	//���ñ�����ɫ
	void SetTitleBrush(const QBrush &brush);
	//��ǰԤ����ӵ�  x����Ϊ��1970-01-01T00:00:00.000��ʼ����ǰʱ�̵ĺ�����
	void InsertPointToDayPre(double dx, double dy);
	void InsertPointLstToDayPre(const QList<QPointF>& lstPoint);
	//�������������
	void InsertPointToShortPre(double dx, double dy);
	void InsertPointLstToShortPre(const QList<QPointF>& lstPoint);
	//����ʵ��
	void InsertPointToCurrent(double dx, double dy);
	void InsertPointLstToCurrent(const QList<QPointF>& lstPoint);
	//���ĿǰԤ��
	void ClearDayPre();
	//��ճ�����Ԥ��
	void ClearShortPre();
	//��ս���ʵ��
	void ClearCurrent();
	//��������  ��ǰ����
	void SetDayPreVisable(bool bFlag);
	//����������
	void SetShortPreVisable(bool bFlag);
	//����ʵ��
	void SetCurrentVisable(bool bFlag);
	//��ʾ����
	void SetCureShow();
	//��ʾ��״ͼ
	void SetAreaCureShow();

public:
	//��ʼ������
	void InitCurve();
	//���������᷶Χ
	void SetAxisRange(double dx, double dy);
	//
	void SetAxisRange(const QList<QPointF>& lstPoint);
private:

	QChart *m_pChart;
	QChartView *m_chartView;
	QGridLayout *m_mainLayout;

	//��ǰԤ��
	QLineSeries *m_pLineDayPre;
	QAreaSeries * m_pAreaDayPre;
	//������Ԥ��
	QLineSeries *m_pLineShortPre;
	QAreaSeries * m_pAreaLineShortPre;
	//����ʵ��
	QLineSeries *m_pLineCurrent;
	QAreaSeries * m_pAreaLineCurrent;
	//Ԥ��ֵ  xmax
	double m_nXMax;
	//Ԥ��ֵ  xmin
	double m_nXMin;
	//y max    yminΪ�̶�0
	double m_nYMax;
};


