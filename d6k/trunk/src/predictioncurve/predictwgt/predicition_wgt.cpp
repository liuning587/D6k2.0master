#include "predicition_wgt.h"
#include "cellwidgts.h"
#include "chartwgt.h"

#include <QTimer>


CPredicitionWgt::CPredicitionWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_CurveWgt = new CChartWgt(this);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(m_CurveWgt);
	layout->setContentsMargins(0, 0, 0, 0);
	ui.tab_4->setLayout(layout);

	ui.splitter->setStretchFactor(0, 1);
	ui.splitter->setStretchFactor(1, 6);
	ui.treeWidget->setMinimumWidth(300);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	InitWidget();
	ui.treeWidget->expandAll();

	MakeData();

}

void CPredicitionWgt::InitWidget()
{
	if (ui.treeWidget->topLevelItemCount() != 4)
	{
		return;
	}

	m_pStartTime = new CCellTimeWgt;
	m_pStartTime->SetLabelText(QStringLiteral("开始时间:"));

	m_pEndTime = new CCellTimeWgt;
	m_pEndTime->SetLabelText(QStringLiteral("结束时间:"));


	QTreeWidgetItem *pBeginTime = new QTreeWidgetItem;	
	ui.treeWidget->topLevelItem(0)->addChild(pBeginTime);
	ui.treeWidget->setItemWidget(pBeginTime, 0, m_pStartTime);

	QTreeWidgetItem *pEndTime = new QTreeWidgetItem;
	ui.treeWidget->topLevelItem(0)->addChild(pEndTime);
	ui.treeWidget->setItemWidget(pEndTime, 0, m_pEndTime);


	//图形属性
	mj_pAttrWgt = new CCellRadioWgt;
	QTreeWidgetItem *pAttrItem = new QTreeWidgetItem;
	ui.treeWidget->topLevelItem(1)->addChild(pAttrItem);
	ui.treeWidget->setItemWidget(pAttrItem, 0, mj_pAttrWgt);
	connect(mj_pAttrWgt,SIGNAL(Signal_ClickItem(int)),this,SLOT(Slot_ChooseCureType(int)));
	//类型选择
	m_pTypeChooseWgt = new CCellCheckWgt;
	QTreeWidgetItem *pTypeItem = new QTreeWidgetItem;
	ui.treeWidget->topLevelItem(2)->addChild(pTypeItem);
	ui.treeWidget->setItemWidget(pTypeItem, 0, m_pTypeChooseWgt);

	//场站列表
	QTreeWidgetItem *m_pStationItem = new QTreeWidgetItem;
	m_pStationItem->setText(0,QStringLiteral("测试场站"));
	ui.treeWidget->topLevelItem(3)->addChild(m_pStationItem);
}

#include <QDateTime>
void CPredicitionWgt::MakeData()
{
	m_CurveWgt->SetChartTitle(QStringLiteral("测试场站"));

	//m_CurveWgt->SetAreaCureShow();
// 	QList<QPointF>  xxlst;
// 	xxlst.append(QPointF(QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch(), 3));
// 	xxlst.append(QPointF(QDateTime::currentDateTime().toMSecsSinceEpoch(), 5));
// 	xxlst.append(QPointF(QDateTime::currentDateTime().addDays(1).toMSecsSinceEpoch(), 6));
//  	m_CurveWgt->InsertPointLstToDayPre(xxlst);

	m_CurveWgt->InsertPointToDayPre(QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch(), 3);
	m_CurveWgt->InsertPointToDayPre(QDateTime::currentDateTime().toMSecsSinceEpoch(), 6);
	m_CurveWgt->InsertPointToDayPre(QDateTime::currentDateTime().addDays(1).toMSecsSinceEpoch(), 3);


	m_CurveWgt->InsertPointToCurrent(QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch(), 1);
	m_CurveWgt->InsertPointToCurrent(QDateTime::currentDateTime().toMSecsSinceEpoch(), 2);
	m_CurveWgt->InsertPointToCurrent(QDateTime::currentDateTime().addDays(1).toMSecsSinceEpoch(), 1);

	m_CurveWgt->InsertPointToShortPre(QDateTime::currentDateTime().addDays(-1).toMSecsSinceEpoch(), 1);
	m_CurveWgt->InsertPointToShortPre(QDateTime::currentDateTime().toMSecsSinceEpoch(), 3);
	m_CurveWgt->InsertPointToShortPre(QDateTime::currentDateTime().addDays(1).toMSecsSinceEpoch(), 1);


}

void CPredicitionWgt::Slot_ChooseCureType(int nFlag)
{
	if (nFlag == 1)
	{
		//折线图
		m_CurveWgt->SetCureShow();
	}
	else if (nFlag == 2)
	{
		//柱状图
		m_CurveWgt->SetAreaCureShow();
	}
}
