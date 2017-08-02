#include "diwdiget.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>
#include <QMenu>



CDiWdiget::CDiWdiget(CNetManager *pNetManager, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Q_ASSERT(pNetManager);
	if (pNetManager == nullptr)
	{
		return;
	}
	m_pNetManager = pNetManager;

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	InitData();
	//右击菜单
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

}

CDiWdiget::~CDiWdiget()
{
}


//初始化数据
void CDiWdiget::InitData()
{
	const std::map<int, std::shared_ptr<CDIDOInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetDIInfo();

	for (auto item : pPointTable)
	{
		int nRow = ui.tableWidget->rowCount();

		QTableWidgetItem *pItem0 = new QTableWidgetItem;
		pItem0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem0->setText(QString::number(nRow + 1));

		QTableWidgetItem *pItem1 = new QTableWidgetItem;
		pItem1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem1->setText(QString::number(item.second->m_nPointNum));

		QTableWidgetItem *pItem2 = new QTableWidgetItem;
		pItem2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem2->setText(item.second->m_nName);

		QTableWidgetItem *pItem3 = new QTableWidgetItem;
		pItem3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);

		QTableWidgetItem *pItem4 = new QTableWidgetItem;
		pItem4->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		pItem4->setText(QString::number(item.second->m_nValue));

		QTableWidgetItem *pItem5 = new QTableWidgetItem;
		pItem5->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		ui.tableWidget->insertRow(nRow);
		ui.tableWidget->setItem(nRow, 0, pItem0);
		ui.tableWidget->setItem(nRow, 1, pItem1);
		ui.tableWidget->setItem(nRow, 2, pItem2);
		ui.tableWidget->setItem(nRow, 3, pItem3);
		ui.tableWidget->setItem(nRow, 4, pItem4);
		ui.tableWidget->setItem(nRow, 5, pItem5);


		m_IdItem.insert(std::make_pair(item.second->m_nPointNum, pItem3));
	}
}

void CDiWdiget::Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas)
{
	//更新实时数据

	int nDataNum = tMeas.msgLeg.GetAddr() / sizeof(DBG_GET_MEAS::MEAS_DATA);

	const std::map<int, std::shared_ptr<CDIDOInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetDIInfo();


	for (int i = 0; i < nDataNum; i++)
	{
		if (pPointTable.find(tMeas.m_data[i].infoaddr) != pPointTable.end())
		{

			//int 类型数据
			if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
			{
				DBG_MSG2 *strValue = (DBG_MSG2*)tMeas.m_data[i].measData;
				DBG_MSG2 *strValueType = (DBG_MSG2*)(tMeas.m_data[i].measData+2);

				//更新数据
				m_IdItem[tMeas.m_data[i].infoaddr]->setText(QString::number(strValue->GetAddr()));
				//更新数据类型
				ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 4)->setText(QString::number(strValueType->GetAddr()));

				ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 5)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
			}

		}

	}
}

void CDiWdiget::Slot_ContextMenuRequest(const QPoint &cPoint)
{
	Q_UNUSED(cPoint);
	QMenu *pMenu = new QMenu(ui.tableWidget);

	QAction *pUpdateAct = new QAction(QStringLiteral("更新"), ui.tableWidget);
	pMenu->addAction(pUpdateAct);

	QAction *pSetAct = new QAction(QStringLiteral("设置"), ui.tableWidget);
	pMenu->addAction(pSetAct);

	connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateData()));
	connect(pSetAct, SIGNAL(triggered()), this, SLOT(Slot_SetDevData()));

	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}

void CDiWdiget::Slot_UpdateData()
{
	DBG_HEADER dbgHeader;
	dbgHeader.header0 = 0xAA;
	dbgHeader.header1 = 0x55;

	dbgHeader.msgLeg.SetAddr(1);
	//获取开入开出
	dbgHeader.type = DBG_CODE_GET_DICFG;

	m_pNetManager->GetSender()->OnSendRequest(&dbgHeader);
}

//设置定值
void CDiWdiget::Slot_SetDevData()
{
// 	QList<int> lstRows = GetSelectRows();
// 	if (lstRows.count() == 0)
// 	{
// 		return;
// 	}

	DBG_GET_MEAS devData;
	devData.header0 = 0xAA;
	devData.header1 = 0x55;
	devData.msgLeg.SetAddr(1 + sizeof(DBG_GET_MEAS::MEAS_DATA) *ui.tableWidget->rowCount());   //长度包括功能码
	devData.type = DBG_CODE_SET_DICFG;

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();

		DBG_MSG2 *strValue = (DBG_MSG2*)devData.m_data[i].measData;
		DBG_MSG2 *strValueType = (DBG_MSG2*)(devData.m_data[i].measData + 2);

		strValue->SetAddr(ui.tableWidget->item(i, 3)->text().toInt());
		strValueType->SetAddr(ui.tableWidget->item(i, 4)->text().toInt());
	}

	m_pNetManager->GetSender()->OnSend4Data(devData);
	GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("发送定值设置包").toLocal8Bit().data(), 1);
}


//获取选中的行号
QList<int> CDiWdiget::GetSelectRows()
{
	QList<int> lstSelectRows;


	QList<QTableWidgetSelectionRange>ranges = ui.tableWidget->selectedRanges();

	int count = ranges.count();

	for (int i = 0; i < count; i++)

	{

		int topRow = ranges.at(i).topRow();

		int bottomRow = ranges.at(i).bottomRow();

		for (int j = topRow; j <= bottomRow; j++)

		{
			lstSelectRows.append(j);
		}
	}

	return lstSelectRows;
}