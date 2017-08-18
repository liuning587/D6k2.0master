#include "predict_weather_table_widget.h"
#include "predict_module.h"

#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

CWeatherTableWidget::CWeatherTableWidget(CWeatherData* weatherItem)
	:m_pWeatherItem(weatherItem)
{
	QStringList headStringList;

	headStringList << "ID" << "NAME" << QObject::tr("Description") <<"Type" << "SelectPoint";

	this->setRowCount(9);
	this->setColumnCount(5);
	this->setHorizontalHeaderLabels(headStringList);

	QTableWidgetItem *newItem_0 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_TotalRadiation.m_nID));
	newItem_0->setTextAlignment(Qt::AlignCenter);
	newItem_0->setFlags(newItem_0->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 0, newItem_0);

	QTableWidgetItem *newItem_1 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_DirectRadiation.m_nID));
	newItem_1->setTextAlignment(Qt::AlignCenter);
	newItem_1->setFlags(newItem_1->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 0, newItem_1);

	QTableWidgetItem *newItem_2 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ScattRadiation.m_nID));
	newItem_2->setTextAlignment(Qt::AlignCenter);
	newItem_2->setFlags(newItem_2->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 0, newItem_2);

	QTableWidgetItem *newItem_3 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindDirect.m_nID));
	newItem_3->setTextAlignment(Qt::AlignCenter);
	newItem_3->setFlags(newItem_3->flags() & (~Qt::ItemIsEditable));
	this->setItem(3, 0, newItem_3);

	QTableWidgetItem *newItem_4 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindSpeed.m_nID));
	newItem_4->setTextAlignment(Qt::AlignCenter);
	newItem_4->setFlags(newItem_4->flags() & (~Qt::ItemIsEditable));
	this->setItem(4, 0, newItem_4);

	QTableWidgetItem *newItem_5 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirTemperature.m_nID));
	newItem_5->setTextAlignment(Qt::AlignCenter);
	newItem_5->setFlags(newItem_5->flags() & (~Qt::ItemIsEditable));
	this->setItem(5, 0, newItem_5);

	QTableWidgetItem *newItem_6 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_RelativeHumdty.m_nID));
	newItem_6->setTextAlignment(Qt::AlignCenter);
	newItem_6->setFlags(newItem_6->flags() & (~Qt::ItemIsEditable));
	this->setItem(6, 0, newItem_6);

	QTableWidgetItem *newItem_7 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirPressure.m_nID));
	newItem_7->setTextAlignment(Qt::AlignCenter);
	newItem_7->setFlags(newItem_7->flags() & (~Qt::ItemIsEditable));
	this->setItem(7, 0, newItem_7);

	QTableWidgetItem *pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ComponentTemperature.m_nID));
	pTmp->setTextAlignment(Qt::AlignCenter);
	pTmp->setFlags(pTmp->flags() & (~Qt::ItemIsEditable));
	this->setItem(8, 0, pTmp);



	QTableWidgetItem *newItem_10 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_TotalRadiation.m_szName));
	newItem_10->setTextAlignment(Qt::LeftToRight);
	newItem_10->setFlags(newItem_10->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 1, newItem_10);

	QTableWidgetItem *newItem_11 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_DirectRadiation.m_szName));
	newItem_11->setTextAlignment(Qt::LeftToRight);
	newItem_11->setFlags(newItem_11->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 1, newItem_11);

	QTableWidgetItem *newItem_12 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ScattRadiation.m_szName));
	newItem_12->setTextAlignment(Qt::LeftToRight);
	newItem_12->setFlags(newItem_12->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 1, newItem_12);

	QTableWidgetItem *newItem_13 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindDirect.m_szName));
	newItem_13->setTextAlignment(Qt::LeftToRight);
	newItem_13->setFlags(newItem_13->flags() & (~Qt::ItemIsEditable));
	this->setItem(3, 1, newItem_13);

	QTableWidgetItem *newItem_14 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindSpeed.m_szName));
	newItem_14->setTextAlignment(Qt::LeftToRight);
	newItem_14->setFlags(newItem_14->flags() & (~Qt::ItemIsEditable));
	this->setItem(4, 1, newItem_14);

	QTableWidgetItem *newItem_15 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirTemperature.m_szName));
	newItem_15->setTextAlignment(Qt::LeftToRight);
	newItem_15->setFlags(newItem_15->flags() & (~Qt::ItemIsEditable));
	this->setItem(5, 1, newItem_15);

	QTableWidgetItem *newItem_16 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_RelativeHumdty.m_szName));
	newItem_16->setTextAlignment(Qt::LeftToRight);
	newItem_16->setFlags(newItem_16->flags() & (~Qt::ItemIsEditable));
	this->setItem(6, 1, newItem_16);

	QTableWidgetItem *newItem_17 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirPressure.m_szName));
	newItem_17->setTextAlignment(Qt::LeftToRight);
	newItem_17->setFlags(newItem_17->flags() & (~Qt::ItemIsEditable));
	this->setItem(7, 1, newItem_17);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ComponentTemperature.m_szName));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(newItem_17->flags() & (~Qt::ItemIsEditable));
	this->setItem(8, 1, pTmp);


	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_TotalRadiation.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(newItem_10->flags() | (Qt::ItemIsEditable));
	this->setItem(0, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_DirectRadiation.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(1, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ScattRadiation.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(newItem_12->flags() | (Qt::ItemIsEditable));
	this->setItem(2, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindDirect.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(3, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindSpeed.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(4, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirTemperature.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(5, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_RelativeHumdty.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(6, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirPressure.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(7, 2, pTmp);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ComponentTemperature.m_strDescription));
	pTmp->setTextAlignment(Qt::LeftToRight);
	pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
	this->setItem(8, 2, pTmp);




	QTableWidgetItem *newItem_20 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_TotalRadiation.m_nType));
	newItem_20->setTextAlignment(Qt::AlignCenter);
	newItem_20->setFlags(newItem_20->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 3, newItem_20);

	QTableWidgetItem *newItem_21 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_DirectRadiation.m_nType));
	newItem_21->setTextAlignment(Qt::AlignCenter);
	newItem_21->setFlags(newItem_21->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 3, newItem_21);

	QTableWidgetItem *newItem_22 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ScattRadiation.m_nType));
	newItem_22->setTextAlignment(Qt::AlignCenter);
	newItem_22->setFlags(newItem_22->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 3, newItem_22);

	QTableWidgetItem *newItem_23 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindDirect.m_nType));
	newItem_23->setTextAlignment(Qt::AlignCenter);
	newItem_23->setFlags(newItem_23->flags() & (~Qt::ItemIsEditable));
	this->setItem(3, 3, newItem_23);

	QTableWidgetItem *newItem_24 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindSpeed.m_nType));
	newItem_24->setTextAlignment(Qt::AlignCenter);
	newItem_24->setFlags(newItem_24->flags() & (~Qt::ItemIsEditable));
	this->setItem(4, 3, newItem_24);

	QTableWidgetItem *newItem_25 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirTemperature.m_nType));
	newItem_25->setTextAlignment(Qt::AlignCenter);
	newItem_25->setFlags(newItem_25->flags() & (~Qt::ItemIsEditable));
	this->setItem(5, 3, newItem_25);

	QTableWidgetItem *newItem_26 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_RelativeHumdty.m_nType));
	newItem_26->setTextAlignment(Qt::AlignCenter);
	newItem_26->setFlags(newItem_26->flags() & (~Qt::ItemIsEditable));
	this->setItem(6, 3, newItem_26);

	QTableWidgetItem *newItem_27 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirPressure.m_nType));
	newItem_27->setTextAlignment(Qt::AlignCenter);
	newItem_27->setFlags(newItem_27->flags() & (~Qt::ItemIsEditable));
	this->setItem(7, 3, newItem_27);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ComponentTemperature.m_nType));
	pTmp->setTextAlignment(Qt::AlignCenter);
	pTmp->setFlags(pTmp->flags() & (~Qt::ItemIsEditable));
	this->setItem(8, 3, pTmp);




	QTableWidgetItem *newItem_30 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_TotalRadiation.m_szLinkedTagName));
	newItem_30->setTextAlignment(Qt::AlignCenter);
	newItem_30->setFlags(newItem_30->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 4, newItem_30);

	QTableWidgetItem *newItem_31 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_DirectRadiation.m_szLinkedTagName));
	newItem_31->setTextAlignment(Qt::AlignCenter);
	newItem_31->setFlags(newItem_31->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 4, newItem_31);

	QTableWidgetItem *newItem_32 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ScattRadiation.m_szLinkedTagName));
	newItem_32->setTextAlignment(Qt::AlignCenter);
	newItem_32->setFlags(newItem_32->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 4, newItem_32);

	QTableWidgetItem *newItem_33 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindDirect.m_szLinkedTagName));
	newItem_33->setTextAlignment(Qt::AlignCenter);
	newItem_33->setFlags(newItem_33->flags() & (~Qt::ItemIsEditable));
	this->setItem(3, 4, newItem_33);

	QTableWidgetItem *newItem_34 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AvergWindSpeed.m_szLinkedTagName));
	newItem_34->setTextAlignment(Qt::AlignCenter);
	newItem_34->setFlags(newItem_34->flags() & (~Qt::ItemIsEditable));
	this->setItem(4, 4, newItem_34);

	QTableWidgetItem *newItem_35 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirTemperature.m_szLinkedTagName));
	newItem_35->setTextAlignment(Qt::AlignCenter);
	newItem_35->setFlags(newItem_35->flags() & (~Qt::ItemIsEditable));
	this->setItem(5, 4, newItem_35);

	QTableWidgetItem *newItem_36 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_RelativeHumdty.m_szLinkedTagName));
	newItem_36->setTextAlignment(Qt::AlignCenter);
	newItem_36->setFlags(newItem_36->flags() & (~Qt::ItemIsEditable));
	this->setItem(6, 4, newItem_36);

	QTableWidgetItem *newItem_37 = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_AirPressure.m_szLinkedTagName));
	newItem_37->setTextAlignment(Qt::AlignCenter);
	newItem_37->setFlags(newItem_37->flags() & (~Qt::ItemIsEditable));
	this->setItem(7, 4, newItem_37);

	pTmp = new QTableWidgetItem(QString("%1").arg(m_pWeatherItem->m_ComponentTemperature.m_szLinkedTagName));
	pTmp->setTextAlignment(Qt::AlignCenter);
	pTmp->setFlags(pTmp->flags() & (~Qt::ItemIsEditable));
	this->setItem(8, 4, pTmp);




	QPushButton *pushButton = new QPushButton("");
	pushButton->setFlat(true);
	this->setCellWidget(0, 4, pushButton);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_1 = new QPushButton("");
	pushButton_1->setFlat(true);
	this->setCellWidget(1, 4, pushButton_1);
	connect(pushButton_1, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_2 = new QPushButton("");
	pushButton_2->setFlat(true);
	this->setCellWidget(2, 4, pushButton_2);
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_3 = new QPushButton("");
	pushButton_3->setFlat(true);
	this->setCellWidget(3, 4, pushButton_3);
	connect(pushButton_3, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_4 = new QPushButton("");
	pushButton_4->setFlat(true);
	this->setCellWidget(4, 4, pushButton_4);
	connect(pushButton_4, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_5 = new QPushButton("");
	pushButton_5->setFlat(true);
	this->setCellWidget(5, 4, pushButton_5);
	connect(pushButton_5, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_6 = new QPushButton("");
	pushButton_6->setFlat(true);
	this->setCellWidget(6, 4, pushButton_6);
	connect(pushButton_6, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_7 = new QPushButton("");
	pushButton_7->setFlat(true);
	this->setCellWidget(7, 4, pushButton_7);
	connect(pushButton_7, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_8 = new QPushButton("");
	pushButton_8->setFlat(true);
	this->setCellWidget(8, 4, pushButton_8);
	connect(pushButton_8, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	setColumnWidth(0, 100);
	setColumnWidth(1, 250);
	setColumnWidth(2, 250);
	setColumnWidth(3, 100);
	setColumnWidth(4, 300);

	connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(itemChanged(QTableWidgetItem *)));
}

QString CWeatherTableWidget::GetBindValue()
{
	std::string strNodeTagname;
	std::string strTagname;
	std::string strFilename;
	//
	std::vector<int> m_arrDataType;
	m_arrDataType.push_back(DT_NULL);
	m_arrDataType.push_back(DT_BOOLEAN);
	m_arrDataType.push_back(DT_CHAR);
	m_arrDataType.push_back(DT_BYTE);
	m_arrDataType.push_back(DT_SHORT);
	m_arrDataType.push_back(DT_WORD);
	m_arrDataType.push_back(DT_INT);
	m_arrDataType.push_back(DT_DWORD);
	m_arrDataType.push_back(DT_LLONG);
	m_arrDataType.push_back(DT_LWORD);
	m_arrDataType.push_back(DT_FLOAT);
	m_arrDataType.push_back(DT_DOUBLE);
	m_arrDataType.push_back(DT_STRING);
	m_arrDataType.push_back(DT_BLOCK);
	m_arrDataType.push_back(DT_MAX);
	//
	bool bSelectFlag = GetPreIMainInterface()->SelectTag(m_arrDataType, true, strNodeTagname, strTagname, strFilename);

	if (bSelectFlag)
	{
		QString strResult = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());
		return strResult;
	}

	return "";
}

void CWeatherTableWidget::itemChanged(QTableWidgetItem * item)
{
	CPPPointInfo  m_TotalRadiation;  //! 总辐射
	CPPPointInfo  m_DirectRadiation; //! 垂直辐射
	CPPPointInfo  m_ScattRadiation;  //! 散射辐射
	CPPPointInfo  m_AvergWindDirect; //! 平均风向
	CPPPointInfo  m_AvergWindSpeed;  //! 平均风速
	CPPPointInfo  m_AirTemperature;  //! 空气温度
	CPPPointInfo  m_RelativeHumdty;  //! 相对湿度
	CPPPointInfo  m_AirPressure;     //! 气压
	//组件温度

	if (item->column() == 2 && item->row() == 0)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_TotalRadiation.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 1)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_DirectRadiation.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 2)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_ScattRadiation.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 3)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_AvergWindDirect.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 4)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_AvergWindSpeed.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 5)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_AirTemperature.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 6)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_RelativeHumdty.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 7)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_AirPressure.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 8)
	{
		m_pWeatherItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pWeatherItem->m_ComponentTemperature.m_strDescription = item->text();
	}
}

void CWeatherTableWidget::Slot_SetBindValue()
{
	QString strBindValue = GetBindValue();
	if (strBindValue.isEmpty())
	{
		return;
	}

	QPushButton *pushBtn = qobject_cast<QPushButton*>(sender());

	Q_ASSERT(pushBtn);
	if (pushBtn == nullptr)
	{
		return;
	}

	pushBtn->setText(strBindValue);

	for (int i = 0; i < rowCount(); i++)
	{
		if (this->cellWidget(i, 4) == pushBtn)
		{
			m_pWeatherItem->m_vecTableInfo[i].m_szLinkedTagName = strBindValue;
		}
	}
}



CWeatherTableWidget::~CWeatherTableWidget()
{


}


CWeatherData* CWeatherTableWidget::GetWeatherItem() const
{
	Q_ASSERT(m_pWeatherItem);
	if (m_pWeatherItem)
	{
		return m_pWeatherItem;
	}
	else
	{
		return Q_NULLPTR;
	}

}

void CWeatherTableWidget::Save()
{


}


void CWeatherTableWidget::Refresh()
{


}


void CWeatherTableWidget::SetModifyFlag(bool bFlag)
{
	Q_UNUSED(bFlag);

}

bool CWeatherTableWidget::GetModifyFlag()
{
	return true;
}


CPredictWeatherWidget::CPredictWeatherWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CPredictWeatherWidget::~CPredictWeatherWidget()
{


}



void CPredictWeatherWidget::AddToTableCtrl(CWeatherData* pWeatherItem, QString strName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CWeatherTableWidget* pTableWdgt = dynamic_cast<CWeatherTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CWeatherData* pItem = pTableWdgt->GetWeatherItem();
			if (pItem && (pItem == pWeatherItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CWeatherTableWidget* pTableWdgt = new CWeatherTableWidget(pWeatherItem);

	QString szName = strName + "_" + "Weather Data";

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CPredictWeatherWidget::ReleaseData()
{



}


void CPredictWeatherWidget::SetTabViewHead()
{




}

