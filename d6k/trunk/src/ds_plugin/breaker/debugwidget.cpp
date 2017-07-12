#include "debugwidget.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>
#include <QMenu>
#include <QDebug>

CDebugWidget::CDebugWidget(CNetManager *pNetManager, QWidget *parent)
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
	InitData();
	//右击菜单
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

}

CDebugWidget::~CDebugWidget()
{
}

void CDebugWidget::InitData()
{
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetDebugInfo();

	for (auto item : pPointTable)
	{
		int nRow = ui.tableWidget->rowCount();

		QTableWidgetItem *pItem0 = new QTableWidgetItem;
		pItem0->setText(QString::number(nRow + 1));

		QTableWidgetItem *pItem1 = new QTableWidgetItem;
		pItem1->setText(QString::number(item.second->m_nPointNum));

		QTableWidgetItem *pItem2 = new QTableWidgetItem;
		pItem2->setText(item.second->m_strType);

		QTableWidgetItem *pItem3 = new QTableWidgetItem;
		pItem3->setText(item.second->m_strName);

		QTableWidgetItem *pItem4 = new QTableWidgetItem;
		pItem4->setText(item.second->m_strUnit);

		QTableWidgetItem *pItem5 = new QTableWidgetItem;
		pItem5->setText(item.second->m_strValue);

		QTableWidgetItem *pItem6 = new QTableWidgetItem;

		ui.tableWidget->insertRow(nRow);
		ui.tableWidget->setItem(nRow, 0, pItem0);
		ui.tableWidget->setItem(nRow, 1, pItem1);
		ui.tableWidget->setItem(nRow, 2, pItem2);
		ui.tableWidget->setItem(nRow, 3, pItem3);
		ui.tableWidget->setItem(nRow, 4, pItem4);
		ui.tableWidget->setItem(nRow, 5, pItem5);
		ui.tableWidget->setItem(nRow, 6, pItem6);

		m_IdItem.insert(std::make_pair(item.second->m_nPointNum, pItem5));
	}
}

void CDebugWidget::Slot_RecvNewRealTimeData(DBG_GET_MEAS & tMeas)
{
	//更新实时数据

	int nDataNum = tMeas.msgLeg.GetAddr() / sizeof(DBG_GET_MEAS::MEAS_DATA);

	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetDebugInfo();


	for (int i = 0; i < nDataNum; i++)
	{
		if (pPointTable.find(tMeas.m_data[i].infoaddr) != pPointTable.end())
		{
			if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_INT)
			{
				//int 类型数据
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					QString strValue = QString::number(*(int *)tMeas.m_data[i].measData);
					//更新数据
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;

					if (nSmallNum != 0)
					{
						strValue = QString::number(strValue.toInt()*1.0 / (nSmallNum * 10.0), 'g', strValue.length());
					}

					m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
				}
			}
			else if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_UINT)
			{
				//无符号整形
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					QString strValue = QString::number(*(unsigned int *)tMeas.m_data[i].measData);
					//更新数据
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;

					for (int i = 0; i < nSmallNum; i++)
					{
						if (i == 0)
						{
							strValue += ".";
						}
						strValue += "0";
					}

					m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
				}
			}
			else if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_HEX)
			{
				//hex类型数据
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					QString strValue = QString::number(*(int *)tMeas.m_data[i].measData, 16);
					strValue = "0x" + strValue;
					//更新数据
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;

					for (int i = 0; i < nSmallNum; i++)
					{
						if (i == 0)
						{
							strValue += ".";
						}
						strValue += "0";
					}

					m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
				}
			}
				else if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_FLOAT)
				{
					//浮点型
					if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
					{
						float fValue = *(float *)tMeas.m_data[i].measData;
						//更新数据
						int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;


						m_IdItem[tMeas.m_data[i].infoaddr]->setText(QString::number(fValue,'g',nSmallNum));


						ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
					}
				}
			
		}

	}
}

void CDebugWidget::Slot_ContextMenuRequest(const QPoint &cPoint)
{
	Q_UNUSED(cPoint);
	QMenu *pMenu = new QMenu(ui.tableWidget);

	QAction *pUpdateAct = new QAction(tr("Update"), ui.tableWidget);
	pMenu->addAction(pUpdateAct);

	connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateData()));

	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}

void CDebugWidget::Slot_UpdateData()
{
	DBG_HEADER dbgHeader;
	dbgHeader.header0 = 0xAA;
	dbgHeader.header1 = 0x55;

	dbgHeader.msgLeg.SetAddr(1);
	//获取开入开出
	dbgHeader.type = DBG_CODE_GET_DEBUG_DATA;

	m_pNetManager->GetSender()->OnSendRequest(&dbgHeader);
}
