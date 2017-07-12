#include "realtimesoftwgt.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>


CRealtimeSoftWgt::CRealtimeSoftWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	InitData();

}

CRealtimeSoftWgt::~CRealtimeSoftWgt()
{
}


void CRealtimeSoftWgt::InitData()
{
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetSoftInfo();

	for (auto item : pPointTable)
	{
		int nRow = ui.tableWidget->rowCount();

		QTableWidgetItem *pItem0 = new QTableWidgetItem;
		pItem0->setText(QString::number(nRow + 1));

		QTableWidgetItem *pItem1 = new QTableWidgetItem;
		pItem1->setText(QString::number(item.second->m_nPointNum));

		QTableWidgetItem *pItem2 = new QTableWidgetItem;
		pItem2->setText(QString::number(item.second->m_nOwnId));

		QTableWidgetItem *pItem3 = new QTableWidgetItem;
		pItem3->setText(item.second->m_strName);

		QTableWidgetItem *pItem4 = new QTableWidgetItem;
		pItem4->setText(QString::number(item.second->m_nVlaue));

		QTableWidgetItem *pItem5 = new QTableWidgetItem;

		ui.tableWidget->insertRow(nRow);
		ui.tableWidget->setItem(nRow, 0, pItem0);
		ui.tableWidget->setItem(nRow, 1, pItem1);
		ui.tableWidget->setItem(nRow, 2, pItem2);
		ui.tableWidget->setItem(nRow, 3, pItem3);
		ui.tableWidget->setItem(nRow, 4, pItem4);
		ui.tableWidget->setItem(nRow, 5, pItem5);


	}
}

void CRealtimeSoftWgt::UpdateItem(int nValue)
{
	//转换为二进制字符串
	QString strValue = QString::number(nValue, 2);
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		if (strValue.length() > i)
		{
			ui.tableWidget->item(i, 4)->setText(strValue.at(strValue.length() - 1 - i));
			ui.tableWidget->item(i, 5)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
		}
		else
		{
			ui.tableWidget->item(i, 4)->setText("0");
			ui.tableWidget->item(i, 5)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

		}
	}
}
