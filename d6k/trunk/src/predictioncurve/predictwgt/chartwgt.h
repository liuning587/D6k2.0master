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
	//设置曲线图标题
	void SetChartTitle(const QString &strName);
	//设置标题字体格式
	void SetTitleFont(const QFont &Font);
	//设置标题颜色
	void SetTitleBrush(const QBrush &brush);
	//日前预测添加点  x坐标为从1970-01-01T00:00:00.000开始到当前时刻的毫秒数
	void InsertPointToDayPre(double dx, double dy);
	void InsertPointLstToDayPre(const QList<QPointF>& lstPoint);
	//超短期添加数据
	void InsertPointToShortPre(double dx, double dy);
	void InsertPointLstToShortPre(const QList<QPointF>& lstPoint);
	//今日实测
	void InsertPointToCurrent(double dx, double dy);
	void InsertPointLstToCurrent(const QList<QPointF>& lstPoint);
	//清空目前预测
	void ClearDayPre();
	//清空超短期预测
	void ClearShortPre();
	//清空今日实测
	void ClearCurrent();
	//隐藏曲线  日前曲线
	void SetDayPreVisable(bool bFlag);
	//超短期曲线
	void SetShortPreVisable(bool bFlag);
	//今日实测
	void SetCurrentVisable(bool bFlag);
	//显示折线
	void SetCureShow();
	//显示柱状图
	void SetAreaCureShow();

public:
	//初始化曲线
	void InitCurve();
	//设置坐标轴范围
	void SetAxisRange(double dx, double dy);
	//
	void SetAxisRange(const QList<QPointF>& lstPoint);
private:

	QChart *m_pChart;
	QChartView *m_chartView;
	QGridLayout *m_mainLayout;

	//日前预测
	QLineSeries *m_pLineDayPre;
	QAreaSeries * m_pAreaDayPre;
	//超短期预测
	QLineSeries *m_pLineShortPre;
	QAreaSeries * m_pAreaLineShortPre;
	//今日实测
	QLineSeries *m_pLineCurrent;
	QAreaSeries * m_pAreaLineCurrent;
	//预测值  xmax
	double m_nXMax;
	//预测值  xmin
	double m_nXMin;
	//y max    ymin为固定0
	double m_nYMax;
};


