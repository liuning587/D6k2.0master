#include "realtimedowgt.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>


CRealtimeDoWgt::CRealtimeDoWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	InitData();

}

CRealtimeDoWgt::~CRealtimeDoWgt()
{
}

void CRealtimeDoWgt::InitData()
{
	const std::map<int, std::shared_ptr<CDIDOInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetDOInfo();

	for (auto item : pPointTable)
	{
		int nRow = ui.tableWidget->rowCount();

		QTableWidgetItem *pItem0 = new QTableWidgetItem;
		pItem0->setText(QString::number(nRow + 1));

		QTableWidgetItem *pItem1 = new QTableWidgetItem;
		pItem1->setText(QString::number(item.second->m_nPointNum));

		QTableWidgetItem *pItem2 = new QTableWidgetItem;
		pItem2->setText(item.second->m_nName);

		QTableWidgetItem *pItem3 = new QTableWidgetItem;
		pItem3->setText(QString::number(item.second->m_nValue));

		QTableWidgetItem *pItem4 = new QTableWidgetItem;


		ui.tableWidget->insertRow(nRow);
		ui.tableWidget->setItem(nRow, 0, pItem0);
		ui.tableWidget->setItem(nRow, 1, pItem1);
		ui.tableWidget->setItem(nRow, 2, pItem2);
		ui.tableWidget->setItem(nRow, 3, pItem3);
		ui.tableWidget->setItem(nRow, 4, pItem4);


		//m_IdItem.insert(std::make_pair(item.second->m_nPointNum, pItem3));
	}
}

void CRealtimeDoWgt::UpdateItem(int nValue)
{
	//转换为二进制字符串
	QString strValue = QString::number(nValue, 2);
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		if (strValue.length() > i)
		{
			ui.tableWidget->item(i, 3)->setText(strValue.at(strValue.length() - 1 - i));
			ui.tableWidget->item(i, 4)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
		}
		else
		{
			ui.tableWidget->item(i, 3)->setText("0");
			ui.tableWidget->item(i, 4)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

		}
	}
}
