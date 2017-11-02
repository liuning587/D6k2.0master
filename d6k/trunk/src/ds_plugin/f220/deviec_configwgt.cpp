#include "deviec_configwgt.h"
#include "commthread.h"
#include "devicestudio/main_module.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QMenu>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>


CDevIecConfigWgt::CDevIecConfigWgt(CCommThread *pCommunicate, IMainModule *pCore, const QString &deviceName, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pCommunicate = pCommunicate;
	m_pCore = pCore;
	m_strDeviceName = deviceName;
	m_nBeginPointNum = IEC_START_POINT_NUM;

	m_pFixReadTimeOut = new QTimer;

	ui.tableWidget->setColumnWidth(0, 300);
	ui.tableWidget->setColumnWidth(4, 500);

	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);

	ui.tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui.tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//右击菜单
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
	//隐藏起始地址那一列
	//pCurrentTable->setColumnHidden(5, true);

	connect(m_pFixReadTimeOut, SIGNAL(timeout()), this, SLOT(Slot_FixReadTimeOut()));

	connect(ui.tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_DoubleClickTableItem(QTableWidgetItem *)));

	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	connect(ui.tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	connect(ui.tableWidget_3, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	connect(m_pCommunicate, SIGNAL(Signal_DevReadBack(QMap<int, float>)), this, SLOT(Slot_updateFixData(QMap<int, float>)));


	m_mapTypeCode.insert(1, "Boolean");
	m_mapTypeCode.insert(43, "Tiny");
	m_mapTypeCode.insert(32, "UTiny");
	m_mapTypeCode.insert(33, "Short");
	m_mapTypeCode.insert(45, "UShort");
	m_mapTypeCode.insert(2, "Int");
	m_mapTypeCode.insert(35, "Uint");
	m_mapTypeCode.insert(36, "Long");
	m_mapTypeCode.insert(37, "Ulong");
	m_mapTypeCode.insert(38, "Float");
	m_mapTypeCode.insert(39, "Double");
	m_mapTypeCode.insert(4, "String");
	m_strDeviceName = "FTU220";
}

CDevIecConfigWgt::~CDevIecConfigWgt()
{
	m_pFixReadTimeOut->deleteLater();
}

bool CDevIecConfigWgt::InitWgt(const QString & strFileName)
{
	QFile fFixConfig(strFileName);
	if (fFixConfig.open(QFile::ReadOnly | QFile::Text))
	{
		AnalyseXmlData(&fFixConfig);
		fFixConfig.close();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Open file %1 error!").arg(strFileName));
		return false;
	}

	//初始化iec信息
	InitIecInfo();

	return true;
}

void CDevIecConfigWgt::AnalyseXmlData(QIODevice * pDevice)
{
	QXmlStreamReader xmlReader(pDevice);


	while (!xmlReader.atEnd() && (!xmlReader.hasError()))
	{
		//遍历所有配置
		xmlReader.readNext();
		if (xmlReader.isStartElement())
		{
			qDebug() << xmlReader.name().toString();
			if (xmlReader.attributes().value("name") == QString("通讯参数"))
			{
				while (!xmlReader.atEnd())
				{
					//遍历所有配置
					xmlReader.readNext();

					if (xmlReader.isStartElement())
					{
						if (xmlReader.name().toString() == "Item")
						{
							//添加数据

							int iInsertRow = ui.tableWidget->rowCount();

							ui.tableWidget->insertRow(iInsertRow);
							QTableWidgetItem *item0 = new QTableWidgetItem;
							item0->setText(xmlReader.attributes().value("name").toString());
							item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 0, item0);

							QTableWidgetItem *item1 = new QTableWidgetItem;
							item1->setText(xmlReader.attributes().value("defValue").toString());
							item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 1, item1);

							QTableWidgetItem *item2 = new QTableWidgetItem;
							item2->setText(xmlReader.attributes().value("defValue").toString());
							item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 2, item2);

							QTableWidgetItem *item3 = new QTableWidgetItem;
							item3->setText(xmlReader.attributes().value("range").toString());
							item3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 3, item3);

							QTableWidgetItem *item4 = new QTableWidgetItem;
							item4->setText(xmlReader.attributes().value("desc").toString());
							item4->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 4, item4);

							QTableWidgetItem *item5 = new QTableWidgetItem;
							item5->setText(QString::number(m_nBeginPointNum++));
							item5->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 5, item5);

							QTableWidgetItem *item6 = new QTableWidgetItem;
							int nType = xmlReader.attributes().value("tag").toString().toInt();
							item6->setText(m_mapTypeCode[nType]);
							item6->setData(Qt::UserRole + 1, nType);
							item6->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 6, item6);

							QTableWidgetItem *item7 = new QTableWidgetItem;
							item7->setText(xmlReader.attributes().value("length").toString());
							item7->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 7, item7);

							m_pIdToItem.insert(item5->text().toInt(),item1);

						}
					}

					if (xmlReader.isEndElement())
					{
						//
						if (xmlReader.name().toString() == "DevParam")
						{
							break;
						}
					}
				}

				break;
			}

		}
	}
}

void CDevIecConfigWgt::InitIecInfo()
{
	if (!Init101IecInfo())
	{
		QMessageBox::warning(this,tr("告警"),tr("初始化101规约配置失败"));
		return;
	}

	if (!Init104IecInfo())
	{
		QMessageBox::warning(this, tr("告警"), tr("初始化104规约配置失败"));
		return;

	}
}

bool CDevIecConfigWgt::Init101IecInfo()
{
	QFile tFile(qApp->applicationDirPath() + IEC_101_FILE);

	if (tFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!tFile.atEnd())
		{
			QByteArray tbyData = tFile.readLine();
			QString strLineData = QString::fromLocal8Bit(tbyData);

			if (!strLineData.contains("="))
			{
				continue;
			}

			QTableWidgetItem *pItem0 = new QTableWidgetItem;
			pItem0->setText(strLineData.split("=").first());
			pItem0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			QTableWidgetItem *pItem1 = new QTableWidgetItem;
			pItem1->setText(strLineData.split("=").last().trimmed());
			pItem1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);

			QTableWidgetItem *pItem2 = new QTableWidgetItem;
			pItem2->setText(strLineData.split("=").last().trimmed());
			pItem2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			QTableWidgetItem *pItem3 = new QTableWidgetItem;
			pItem3->setText(QString::number(m_nBeginPointNum++));
			pItem3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			int nRow = ui.tableWidget_3->rowCount();
			ui.tableWidget_3->insertRow(nRow);

			ui.tableWidget_3->setItem(nRow, 0, pItem0);
			ui.tableWidget_3->setItem(nRow, 1, pItem1);
			ui.tableWidget_3->setItem(nRow, 2, pItem2);
			ui.tableWidget_3->setItem(nRow, 3, pItem3);

			m_pIdToItem.insert(pItem3->text().toInt(), pItem1);

		}
	}
	else
	{
		return false;
	}
	return true;
}

bool CDevIecConfigWgt::Init104IecInfo()
{
	QFile tFile(qApp->applicationDirPath() + IEC_104_FILE);

	if (tFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!tFile.atEnd())
		{
			QByteArray tbyData = tFile.readLine();
			QString strLineData = QString::fromLocal8Bit(tbyData);

			if (!strLineData.contains("="))
			{
				continue;
			}
			

			QTableWidgetItem *pItem0 = new QTableWidgetItem;
			pItem0->setText(strLineData.split("=").first());
			pItem0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			QTableWidgetItem *pItem1 = new QTableWidgetItem;
			pItem1->setText(strLineData.split("=").last().trimmed());
			pItem1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);

			QTableWidgetItem *pItem2 = new QTableWidgetItem;
			pItem2->setText(strLineData.split("=").last().trimmed());
			pItem2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			QTableWidgetItem *pItem3 = new QTableWidgetItem;
			pItem3->setText(QString::number(m_nBeginPointNum++));
			pItem3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			int nRow = ui.tableWidget_2->rowCount();
			ui.tableWidget_2->insertRow(nRow);

			ui.tableWidget_2->setItem(nRow, 0, pItem0);
			ui.tableWidget_2->setItem(nRow, 1, pItem1);
			ui.tableWidget_2->setItem(nRow, 2, pItem2);
			ui.tableWidget_2->setItem(nRow, 3, pItem3);

			m_pIdToItem.insert(pItem3->text().toInt(), pItem1);

		}
	}
	else
	{
		return false;
	}
	return true;
}

void CDevIecConfigWgt::Slot_ContextMenuRequest(const QPoint & pointCurrent)
{
	QTableWidget *pCurrentTable = qobject_cast <QTableWidget*>(sender());


	QMenu *pLeftMenu = new QMenu(pCurrentTable);

	QAction *pUpdateAllAct = new QAction(tr("Get All"), pCurrentTable);
	QAction *pSendAllAct = new QAction(tr("Send All"), pCurrentTable);
	//保存

	connect(pUpdateAllAct, SIGNAL(triggered()), this, SLOT(Slot_GetAllPoints()));
	connect(pSendAllAct, SIGNAL(triggered()), this, SLOT(Slot_SendCurrentAllPoints()));

	pLeftMenu->addAction(pUpdateAllAct);
	pLeftMenu->addAction(pSendAllAct);

	pLeftMenu->exec(QCursor::pos());
	pLeftMenu->deleteLater();
}


void CDevIecConfigWgt::Slot_GetAllPoints()
{
	//
	//
	QAction *pClickAct = qobject_cast<QAction*>(sender());

	if (pClickAct == nullptr)
	{
		return;
	}

	if (pClickAct->parent() != nullptr)
	{
		QTableWidget *pCurrentTableWgt = qobject_cast<QTableWidget*>(pClickAct->parent());

		if (pCurrentTableWgt != NULL)
		{
			//处理当前选中的行
			IEC_BASE devBaseData;
			devBaseData.m_nAsduID = 1;

			devBaseData.m_nCommandType = D_DV_RD_1;
			
			devBaseData.m_nCto = 5;

			devBaseData.m_nQos = 0;

			for (int i = 0; i < pCurrentTableWgt->rowCount(); i++)
			{
				IEC_DATA singleData;
				if (ui.tableWidget == pCurrentTableWgt)
				{
					singleData.nAddress = pCurrentTableWgt->item(i, 5)->text().toInt();

				}
				else
				{
					singleData.nAddress = pCurrentTableWgt->item(i, 3)->text().toInt();
				}

				singleData.nValue = pCurrentTableWgt->item(i, 1)->text().toFloat();

				devBaseData.m_lstData.append(singleData);
			}
			//发送数据
			m_pCommunicate->SendIecOrder(devBaseData);
			//启动定时器
			//m_pFixReadTimeOut->start(5000);
			m_pCore->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Iec Read Request").toLocal8Bit().data(), 1);
		}
	}
}

void CDevIecConfigWgt::Slot_SendCurrentAllPoints()
{
	QAction *pClickAct = qobject_cast<QAction*>(sender());

	if (pClickAct == nullptr)
	{
		return;
	}

	if (pClickAct->parent() != nullptr)
	{
		QTableWidget *pCurrentTableWgt = qobject_cast<QTableWidget*>(pClickAct->parent());

		if (pCurrentTableWgt != NULL)
		{
			//处理当前选中的行
			IEC_BASE devBaseData;
			devBaseData.m_nAsduID = 1;

			devBaseData.m_nCommandType = D_DV_WR_3;

			devBaseData.m_nCto = COT_ACT;

			devBaseData.m_nQos = 1;

			for (int i = 0; i < pCurrentTableWgt->rowCount(); i++)
			{
				IEC_DATA singleData;
				if (ui.tableWidget == pCurrentTableWgt)
				{
					singleData.nAddress = pCurrentTableWgt->item(i, 5)->text().toInt();

				}
				else
				{
					singleData.nAddress = pCurrentTableWgt->item(i, 3)->text().toInt();
				}
				singleData.nValue = pCurrentTableWgt->item(i, 1)->text().toInt();
				devBaseData.m_lstData.append(singleData);
			}

			//发送数据
			m_pCommunicate->SendIecOrder(devBaseData);
			//m_lstBaseInfo.append(devBaseData);
			//启动定时器
			//m_pFixReadTimeOut->start(5000);
			m_pCore->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Iec Config Write Preset Request").toLocal8Bit().data(), 1);
		}
	}
}

void CDevIecConfigWgt::Slot_DoubleClickTableItem(QTableWidgetItem *item)
{
	QTableWidgetItem * pValueItem = item->tableWidget()->item(item->row(), 1);
	item->tableWidget()->editItem(pValueItem);

}

//读取超时
void CDevIecConfigWgt::Slot_FixReadTimeOut()
{
	m_pCore->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Waiting for Respond Timeout").toLocal8Bit().data(), 1);
	m_pFixReadTimeOut->stop();
	QMessageBox::warning(this, tr("Warning"), tr("Waiting for Respond Timeout,Update Error"));
}

void CDevIecConfigWgt::Slot_updateFixData(QMap<int, float> mapInfo)
{
	QMap<int, float>::const_iterator it = mapInfo.constBegin();

	while (it != mapInfo.constEnd())
	{
		if (m_pIdToItem.contains(it.key()))
		{
			m_pIdToItem[it.key()]->setText(QString::number((int)it.value()));
		}
		it++;
	}

}
