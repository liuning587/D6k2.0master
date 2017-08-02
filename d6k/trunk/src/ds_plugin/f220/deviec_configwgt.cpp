#include "deviec_configwgt.h"
#include "commthread.h"
#include "devicestudio/main_module.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QMenu>
#include <QAbstractItemView>
#include <QTableWidgetItem>

CDevIecConfigWgt::CDevIecConfigWgt(CCommThread *pCommunicate, IMainModule *pCore, const QString &deviceName, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pCommunicate = pCommunicate;
	m_pCore = pCore;
	m_strDeviceName = deviceName;

	ui.tableWidget->setColumnWidth(0, 300);
	ui.tableWidget->setColumnWidth(4, 500);

	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	//右击菜单
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	//隐藏起始地址那一列
	//pCurrentTable->setColumnHidden(5, true);

	connect(ui.tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_DoubleClickTableItem(QTableWidgetItem *)));

	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

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

}

CDevIecConfigWgt::~CDevIecConfigWgt()
{
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
			if (xmlReader.name().toString() == QString("通讯参数"))
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
							item5->setText(QString::number(111));
							item5->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 5, item5);

							QTableWidgetItem *item6 = new QTableWidgetItem;
							//int nType = devParamDatas.lstItems.at(i).TypeCode;
							//item6->setText(m_mapTypeCode[nType]);
						//	item6->setData(Qt::UserRole + 1, nType);
							item6->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 6, item6);

							QTableWidgetItem *item7 = new QTableWidgetItem;
						//	int nDataLeg = devParamDatas.lstItems.at(i).DataLength;
						//	item7->setText(QString::number(nDataLeg));
							item7->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
							ui.tableWidget->setItem(iInsertRow, 7, item7);





							/*
							s_DevItems devItemData;

							devItemData.valueType = xmlReader.attributes().value("valueType").toString();
							devItemData.defalutVlaue = xmlReader.attributes().value("defValue").toString().toInt();
							devItemData.ItemName = xmlReader.attributes().value("name").toString();
							devItemData.ItemDesc = xmlReader.attributes().value("desc").toString();
							devItemData.rangeFt = xmlReader.attributes().value("range").toString();
							devItemData.TypeCode = xmlReader.attributes().value("tag").toString().toInt();
							devItemData.DataLength = xmlReader.attributes().value("length").toString().toInt();

							m_strConfigValueType = xmlReader.attributes().value("valueType").toString();

							currentData.lstItems.append(devItemData);
							*/
						}
					}

					if (xmlReader.isEndElement())
					{
						//
						if (xmlReader.name().toString() == QString("通讯参数"))
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
}

void CDevIecConfigWgt::Slot_ContextMenuRequest(const QPoint & point)
{
}


void CDevIecConfigWgt::Slot_DoubleClickTableItem(QTableWidgetItem *item)
{
}
