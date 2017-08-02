#include "sysdevwgt.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>
#include <QMenu>
#include <QLineEdit>
#include <QHostAddress>


CSysDevWgt::CSysDevWgt(CNetManager *pNetManager, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Q_ASSERT(pNetManager);
	if (pNetManager == nullptr)
	{
		return;
	}
	m_pNetManager = pNetManager;

	m_pLineEdIp1 = new QLineEdit;

	QRegExp ipRegExp = QRegExp("^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$");

	QRegExpValidator *ipRegExpValidator = new QRegExpValidator(ipRegExp, this);
	m_pLineEdIp1->setValidator(ipRegExpValidator);
	m_pLineEdIp1->setInputMask("000.000.000.000");

	m_pLineEdMask1 = new QLineEdit;
	m_pLineEdMask1->setValidator(ipRegExpValidator);
	m_pLineEdMask1->setInputMask("000.000.000.000");

	//
	m_pLineEdGate1 = new QLineEdit;
	m_pLineEdGate1->setValidator(ipRegExpValidator);
	m_pLineEdGate1->setInputMask("000.000.000.000");


	m_pLineEdIp2 = new QLineEdit;
	m_pLineEdIp2->setValidator(ipRegExpValidator);
	m_pLineEdIp2->setInputMask("000.000.000.000");

	//
	m_pLineEdMask2 = new QLineEdit;
	//
	m_pLineEdMask2->setValidator(ipRegExpValidator);
	m_pLineEdMask2->setInputMask("000.000.000.000");

	m_pLineEdGate2 = new QLineEdit;
	m_pLineEdGate2->setValidator(ipRegExpValidator);
	m_pLineEdGate2->setInputMask("000.000.000.000");



	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	InitData();
	//右击菜单
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));


}

CSysDevWgt::~CSysDevWgt()
{
}


//初始化数据
void CSysDevWgt::InitData()
{
	const std::map<int, std::shared_ptr<CDevInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetSystemInfoDevInfo();

	for (auto item : pPointTable)
	{
		int nRow = ui.tableWidget->rowCount();

		ui.tableWidget->insertRow(nRow);

		QTableWidgetItem *pItem0 = new QTableWidgetItem;
		pItem0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem0->setText(QString::number(nRow + 1));

		QTableWidgetItem *pItem1 = new QTableWidgetItem;
		pItem1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem1->setText(QString::number(item.second->m_nPointNum));

		QTableWidgetItem *pItem2 = new QTableWidgetItem;
		pItem2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem2->setText(item.second->m_strDevType);

		QTableWidgetItem *pItem3 = new QTableWidgetItem;
		pItem3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem3->setText(item.second->m_strDevName);

		QTableWidgetItem *pItem4 = new QTableWidgetItem;
		pItem4->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem4->setText(item.second->m_strDevUnit);

		QTableWidgetItem *pItem5 = new QTableWidgetItem;
		pItem5->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pItem5->setText(item.second->m_strDevRange);


// 		QTableWidgetItem *pItem6 = new QTableWidgetItem;
// 		pItem6->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
// 
// 		pItem6->setText(QString::number(item.second->m_strValue));
		if (nRow == IP_ROW_NUM)
		{
			m_pLineEdIp1->setText(GetFormatData(item.second->m_strValue));
			ui.tableWidget->setCellWidget(nRow,6,m_pLineEdIp1);
		}
		else if (nRow == MASK_ROW_NUM)
		{
			m_pLineEdMask1->setText(GetFormatData(item.second->m_strValue));
			ui.tableWidget->setCellWidget(nRow, 6, m_pLineEdMask1);
		}
		else if (nRow == MASK_ROW_GATE)
		{
			m_pLineEdGate1->setText(GetFormatData(item.second->m_strValue));
			ui.tableWidget->setCellWidget(nRow, 6, m_pLineEdGate1);
		}
		else if (nRow == IP_ROW_NUM2)
		{
			m_pLineEdIp2->setText(GetFormatData(item.second->m_strValue));
			ui.tableWidget->setCellWidget(nRow, 6, m_pLineEdIp2);
		}
		else if (nRow == MASK_ROW_NUM2)
		{
			m_pLineEdMask2->setText(GetFormatData(item.second->m_strValue));
			ui.tableWidget->setCellWidget(nRow, 6, m_pLineEdMask2);
		}
		else if (nRow == MASK_ROW_GATE2)
		{
			m_pLineEdGate2->setText(GetFormatData(item.second->m_strValue));
			ui.tableWidget->setCellWidget(nRow, 6, m_pLineEdGate2);
		}
		else
		{
			QTableWidgetItem *pItem6 = new QTableWidgetItem;
			pItem6->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
			
			pItem6->setText(QString::number(item.second->m_strValue));

			ui.tableWidget->setItem(nRow, 6, pItem6);
			m_IdItem.insert(std::make_pair(item.second->m_nPointNum, pItem6));
		}

		QTableWidgetItem *pItem7 = new QTableWidgetItem;
		pItem7->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		
		ui.tableWidget->setItem(nRow, 0, pItem0);
		ui.tableWidget->setItem(nRow, 1, pItem1);
		ui.tableWidget->setItem(nRow, 2, pItem2);
		ui.tableWidget->setItem(nRow, 3, pItem3);
		ui.tableWidget->setItem(nRow, 4, pItem4);
		ui.tableWidget->setItem(nRow, 5, pItem5);

		//ui.tableWidget->setItem(nRow, 6, pItem6);
		ui.tableWidget->setItem(nRow, 7, pItem7);

		
	}
}

void CSysDevWgt::Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas)
{
	//更新实时数据

	int nDataNum = tMeas.msgLeg.GetAddr() / sizeof(DBG_GET_MEAS::MEAS_DATA);

	const std::map<int, std::shared_ptr<CDevInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetSystemInfoDevInfo();


	for (int i = 0; i < nDataNum; i++)
	{
		if (pPointTable.find(tMeas.m_data[i].infoaddr) != pPointTable.end())
		{

			//
			if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
			{
				QString strValue = QString::number(*(int *)tMeas.m_data[i].measData);

				//更新数据
				m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);

				ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 7)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
			}
			else
			{
				int nData = *(int *)tMeas.m_data[i].measData;
				if (tMeas.m_data[i].infoaddr == IP_ROW_NUM)
				{
					//7
					m_pLineEdIp1->setText(QHostAddress(nData).toString());
				}
				else if (tMeas.m_data[i].infoaddr == MASK_ROW_NUM)
				{
					m_pLineEdMask1->setText(QHostAddress(nData).toString());
				}
				else if (tMeas.m_data[i].infoaddr == MASK_ROW_GATE)
				{
					m_pLineEdGate1->setText(QHostAddress(nData).toString());
				}
				else if (tMeas.m_data[i].infoaddr == IP_ROW_NUM2)
				{
					m_pLineEdIp2->setText(QHostAddress(nData).toString());
				}
				else if (tMeas.m_data[i].infoaddr == MASK_ROW_NUM2)
				{
					m_pLineEdMask2->setText(QHostAddress(nData).toString());
				}
				else if (tMeas.m_data[i].infoaddr == MASK_ROW_GATE2)
				{
					m_pLineEdGate2->setText(QHostAddress(nData).toString());
				}

				ui.tableWidget->item(tMeas.m_data[i].infoaddr, 7)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

			}

		}


	}
}

void CSysDevWgt::Slot_ContextMenuRequest(const QPoint &cPoint)
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
void CSysDevWgt::Slot_UpdateData()
{
	DBG_HEADER dbgHeader;
	dbgHeader.header0 = 0xAA;
	dbgHeader.header1 = 0x55;

	dbgHeader.msgLeg.SetAddr(1);
	//获取系统参数定值
	dbgHeader.type = DBG_CODE_GET_SYSCFG;

	m_pNetManager->GetSender()->OnSendRequest(&dbgHeader);
}


//设置定值
void CSysDevWgt::Slot_SetDevData()
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
	devData.type = DBG_CODE_SET_SYSCFG;

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		if (i == IP_ROW_NUM)
		{
			//7
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = QHostAddress(m_pLineEdIp1->text()).toIPv4Address();
		}
		else if (i == MASK_ROW_NUM)
		{
			//8
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = QHostAddress(m_pLineEdMask1->text()).toIPv4Address();

		}
		else if (i == MASK_ROW_GATE)
		{
			//9
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = QHostAddress(m_pLineEdGate1->text()).toIPv4Address();

		}
		else if (i == IP_ROW_NUM2)
		{
			//10
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = QHostAddress(m_pLineEdIp2->text()).toIPv4Address();

		}
		else if (i == MASK_ROW_NUM2)
		{
			//11
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = QHostAddress(m_pLineEdMask2->text()).toIPv4Address();

		}
		else if (i == MASK_ROW_GATE2)
		{
			//12
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = QHostAddress(m_pLineEdGate2->text()).toIPv4Address();

		}
		else
		{
			devData.m_data[i].infoaddr = ui.tableWidget->item(i, 1)->text().toInt();
			int *pnData = (int *)devData.m_data[i].measData;
			*pnData = ui.tableWidget->item(i, 6)->text().toInt();
		}

	}

	m_pNetManager->GetSender()->OnSend4Data(devData);
	GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("发送定值设置包").toLocal8Bit().data(), 1);
}


//获取选中的行号
QList<int> CSysDevWgt::GetSelectRows()
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

QString CSysDevWgt::GetFormatData(int nData)
{
	QHostAddress taddr(nData);
	QString strValue = taddr.toString();
	return strValue;
}
