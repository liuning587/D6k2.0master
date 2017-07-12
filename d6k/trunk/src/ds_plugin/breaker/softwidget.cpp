#include "softwidget.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>
#include <QMenu>



CSoftWidget::CSoftWidget(CNetManager *pNetManager, QWidget *parent)
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

CSoftWidget::~CSoftWidget()
{
}


//初始化数据
void CSoftWidget::InitData()
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


		m_IdItem.insert(std::make_pair(item.second->m_nPointNum, pItem4));
	}
}
void CSoftWidget::Slot_RecvNewRealTimeData(DEG_SOFT_INFO &tMeas)
{
	//更新实时数据

	int nRecvData = *(int *)tMeas.measData;
	QString strValue = QString::number(nRecvData, 2);

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

void CSoftWidget::Slot_ContextMenuRequest(const QPoint &cPoint)
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

//定值获取
void CSoftWidget::Slot_UpdateData()
{
	DBG_HEADER dbgHeader;
	dbgHeader.header0 = 0xAA;
	dbgHeader.header1 = 0x55;

	dbgHeader.msgLeg.SetAddr(1);
	//获取保护定值信息
	dbgHeader.type = DBG_CODE_GET_SOFTSTRAP;

	m_pNetManager->GetSender()->OnSendRequest(&dbgHeader);
}



//设置定值
void CSoftWidget::Slot_SetDevData()
{
	QList<int> lstRows = GetSelectRows();
	if (lstRows.count() == 0)
	{
		return;
	}

	DEG_SOFT_INFO devData;
	devData.header0 = 0xAA;
	devData.header1 = 0x55;
	devData.msgLeg.SetAddr(1 + 4);   //长度包括功能码
	devData.type = DBG_CODE_SET_SOFTSTRAP;

	QString strValue;
	for (int i = 0; i < lstRows.count(); i++)
	{
		strValue.append(ui.tableWidget->item(i, 4)->text());
	}

	int *nRecvData = (int *)devData.measData;
	*nRecvData = ParseBinary(strValue.toStdString().c_str(),strValue.length());

	int nlengtx = *nRecvData;

	m_pNetManager->GetSender()->OnSendSoftData(devData);
	GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("发送定值设置包").toLocal8Bit().data(), 1);
}


//获取选中的行号
QList<int> CSoftWidget::GetSelectRows()
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


int CSoftWidget::ParseBinary(char const * const binaryString, int size)
{
	//将字符串的二进制数字取出，减去48，转换为int型，并保存在新的数组binary内。
	int * binary = new int[size];

	for (int i = 0; i < size; i++)
	{
		int temp = *(binaryString + i) - 48;
		*(binary + i) = temp;
	}

	//将二进制数字转换为十进制
	int parseBinary = 0;

	for (int i = 0; i < size; i++)
	{
		parseBinary += pow((*(binary + i) * 2.0), i);
	}

	return parseBinary;
}