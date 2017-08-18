#include "chartwgt.h"

#include <math.h>
#include <QAreaSeries>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QValueAxis>

CChartWgt::CChartWgt(QWidget *parent)
	: QWidget(parent)
{
	m_nXMin = QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch();
	m_nXMax = QDateTime::currentDateTime().addDays(1).toMSecsSinceEpoch();
	m_nYMax = 1;

	m_pChart = new QChart;
	m_chartView = new QChartView(m_pChart, this);
	m_mainLayout = new QGridLayout();
	m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);
	setLayout(m_mainLayout);

	InitCurve();

	m_pChart->setTitle("");
	m_pChart->legend()->setVisible(true);
	m_pChart->legend()->setAlignment(Qt::AlignBottom);
	m_chartView->setRenderHint(QPainter::Antialiasing);
	//默认情况下显示曲线  不显示区域
	SetCureShow();
}

CChartWgt::~CChartWgt()
{
	m_pChart->deleteLater();
	m_chartView->deleteLater();
	m_mainLayout->deleteLater();

	//日前预测
	m_pLineDayPre->deleteLater();
	m_pAreaDayPre->deleteLater();
	//超短期预测
	m_pLineShortPre->deleteLater();
	m_pAreaLineShortPre->deleteLater();
	//今日实测
	m_pLineCurrent->deleteLater();
	m_pAreaLineCurrent->deleteLater();
}


void CChartWgt::SetChartTitle(const QString &strName)
{
	m_pChart->setTitle(strName);
}

void CChartWgt::SetTitleFont(const QFont &Font)
{
	m_pChart->setTitleFont(Font);
}

void CChartWgt::SetTitleBrush(const QBrush &brush)
{
	m_pChart->setTitleBrush(brush);
}

//设置坐标轴范围
void CChartWgt::SetAxisRange(double dx, double dy)
{
	//判断  值大小
	if (m_nXMax < dx)
	{
		m_nXMax = dx; 
	}
	if (m_nXMin > dx)
	{
		m_nXMin = dx;
	}

	if (m_nYMax < dy)
	{
		m_nYMax = ((int)dy/10+1)*10;
	}

	m_pChart->axisX()->setRange(QDateTime::fromMSecsSinceEpoch(m_nXMin), QDateTime::fromMSecsSinceEpoch(m_nXMax));
	m_pChart->axisY()->setRange(0, m_nYMax);

}


void CChartWgt::SetAxisRange(const QList<QPointF>& lstPoint)
{
	for (auto item : lstPoint)
	{
		if (m_nXMax < item.x())
		{
			m_nXMax = item.x();
		}
		if (m_nXMin > item.x())
		{
			m_nXMin = item.x();
		}

		if (m_nYMax < item.y())
		{
			m_nYMax = ((int)item.y() / 10 + 1) * 10;
		}
	}

	m_pChart->axisX()->setRange(QDateTime::fromMSecsSinceEpoch(m_nXMin), QDateTime::fromMSecsSinceEpoch(m_nXMax));
	m_pChart->axisY()->setRange(0, m_nYMax);

}

void CChartWgt::InsertPointToDayPre(double dx, double dy)
{
	SetAxisRange(dx,dy);
	m_pLineDayPre->append(dx,dy);
}

void CChartWgt::InsertPointLstToDayPre(const QList<QPointF>& lstPoint)
{
	SetAxisRange(lstPoint);
	m_pLineDayPre->replace(lstPoint);
}

void CChartWgt::InsertPointToShortPre(double dx, double dy)
{
	SetAxisRange(dx, dy);
	m_pLineShortPre->append(dx, dy);
}

void CChartWgt::InsertPointLstToShortPre(const QList<QPointF>& lstPoint)
{
	SetAxisRange(lstPoint);
	m_pLineShortPre->replace(lstPoint);
}

void CChartWgt::InsertPointToCurrent(double dx, double dy)
{
	SetAxisRange(dx, dy);
	m_pLineCurrent->append(dx, dy);
}

void CChartWgt::InsertPointLstToCurrent(const QList<QPointF>& lstPoint)
{
	SetAxisRange(lstPoint);
	m_pLineCurrent->replace(lstPoint);
}

void CChartWgt::ClearDayPre()
{
	m_pLineDayPre->clear();
}



void CChartWgt::ClearShortPre()
{
	m_pLineShortPre->clear();
}



void CChartWgt::ClearCurrent()
{
	m_pLineCurrent->clear();
}

void CChartWgt::SetDayPreVisable(bool bFlag)
{
	m_pLineDayPre->setVisible(bFlag);
}

void CChartWgt::SetShortPreVisable(bool bFlag)
{
	m_pLineShortPre->setVisible(bFlag);
}

void CChartWgt::SetCurrentVisable(bool bFlag)
{
	m_pLineCurrent->setVisible(bFlag);
}

void CChartWgt::SetCureShow()
{
	m_pLineDayPre->setVisible(true);
	 m_pAreaDayPre->setVisible(false);
	//超短期预测
	m_pLineShortPre->setVisible(true);
	m_pAreaLineShortPre->setVisible(false);
	//今日实测
	m_pLineCurrent->setVisible(true);
	m_pAreaLineCurrent->setVisible(false);

}

void CChartWgt::SetAreaCureShow()
{
	m_pLineDayPre->setVisible(false);
	m_pAreaDayPre->setVisible(true);
	//超短期预测
	m_pLineShortPre->setVisible(false);
	m_pAreaLineShortPre->setVisible(true);
	//今日实测
	m_pLineCurrent->setVisible(false);
	m_pAreaLineCurrent->setVisible(true);

}

void CChartWgt::InitCurve()
{
	m_pLineDayPre = new QLineSeries;
	m_pLineDayPre->setName(QStringLiteral("日前预测"));
	//m_pLineDayPre->setBrush(QColor(255,0,0));
	
 	m_pAreaDayPre = new QAreaSeries(m_pLineDayPre);
	
 	m_pAreaDayPre->setName(QStringLiteral("日前预测"));

	m_pLineShortPre = new QLineSeries;
	m_pLineShortPre->setName(QStringLiteral("超短期预测"));
	m_pAreaLineShortPre = new QAreaSeries(m_pLineShortPre);
	m_pAreaLineShortPre->setName(QStringLiteral("超短期预测"));

	m_pLineCurrent = new QLineSeries;
	m_pLineCurrent->setName(QStringLiteral("今日实测"));
	m_pAreaLineCurrent = new QAreaSeries(m_pLineCurrent);
	m_pAreaLineCurrent->setName(QStringLiteral("今日实测"));

	m_pChart->addSeries(m_pLineDayPre);
	m_pChart->addSeries(m_pLineShortPre);
	m_pChart->addSeries(m_pLineCurrent);

	m_pChart->addSeries(m_pAreaDayPre);
	m_pChart->addSeries(m_pAreaLineShortPre);
	m_pChart->addSeries(m_pAreaLineCurrent);

	m_pChart->createDefaultAxes();

	QDateTimeAxis *axis = new QDateTimeAxis;
	axis->setFormat(QStringLiteral("yyyy-MM-dd hh:mm:ss"));
	//axis->setRange(QDateTime::fromMSecsSinceEpoch(m_nXMin), QDateTime::fromMSecsSinceEpoch(m_nXMax));

	//m_pChart->axisY()->setRange(0, m_nYMax);

	//axis->setLabelsAngle(30);
	m_pChart->setAxisX(axis, m_pLineDayPre);
	m_pChart->setAxisX(axis, m_pLineShortPre);
	m_pChart->setAxisX(axis, m_pLineCurrent);

	m_pChart->setAxisX(axis, m_pAreaDayPre);
	m_pChart->setAxisX(axis, m_pAreaLineShortPre);
	m_pChart->setAxisX(axis, m_pAreaLineCurrent);

	axis->setTickCount(10);

	QValueAxis *axisy = new QValueAxis;
	//axisy->setRange(0, m_nYMax);
	axisy->setTickCount(11);
	m_pChart->setAxisY(axisy, m_pLineDayPre);
	m_pChart->setAxisY(axisy, m_pLineShortPre);
	m_pChart->setAxisY(axisy, m_pLineCurrent);


	m_pChart->setAxisY(axisy, m_pAreaDayPre);
	m_pChart->setAxisY(axisy, m_pAreaLineShortPre);
	m_pChart->setAxisY(axisy, m_pAreaLineCurrent);


	m_pChart->axisY()->setTitleText(QStringLiteral("功率(MW)"));

	m_pAreaDayPre->setBrush(m_pLineDayPre->color());
	m_pAreaLineCurrent->setBrush(m_pLineCurrent->color());
	m_pAreaLineShortPre->setBrush(m_pLineShortPre->color());
}
