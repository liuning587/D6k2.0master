#include "predict_station_table_widget.h"
#include "predict_module.h"

#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <qaction.h>
#include <qmenu.h>
#include <qset.h>

CStationTableWidget::CStationTableWidget(CPlantInfo* plantItem)
	:m_pPlantItem(plantItem)
{
	QStringList headStringList;

	headStringList << "ID" << "NAME" << "Type" <<"SelectPoint";

	this->setRowCount(2);
	this->setColumnCount(4);
	this->setHorizontalHeaderLabels(headStringList);

	QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ActPower.m_nID));
	newItem->setTextAlignment(Qt::AlignCenter);
	newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 0, newItem);

	QTableWidgetItem *newItem_1 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ReactPower.m_nID));
	newItem_1->setTextAlignment(Qt::AlignCenter);
	newItem_1->setFlags(newItem_1->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 0, newItem_1);

	QTableWidgetItem *newItem_2 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ActPower.m_szName));
	newItem_2->setTextAlignment(Qt::LeftToRight);
	newItem_2->setFlags(newItem_2->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 1, newItem_2);

	QTableWidgetItem *newItem_3 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ReactPower.m_szName));
	newItem_3->setTextAlignment(Qt::LeftToRight);
	newItem_3->setFlags(newItem_3->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 1, newItem_3);

	QTableWidgetItem *newItem_4 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ActPower.m_nType));
	newItem_4->setTextAlignment(Qt::AlignCenter);
	newItem_4->setFlags(newItem_4->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 2, newItem_4);

	QTableWidgetItem *newItem_5 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ReactPower.m_nType));
	newItem_5->setTextAlignment(Qt::AlignCenter);
	newItem_5->setFlags(newItem_5->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 2, newItem_5);

	QTableWidgetItem *newItem_6 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ActPower.m_szLinkedTagName));
	newItem_6->setTextAlignment(Qt::AlignCenter);
	newItem_6->setFlags(newItem_6->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 3, newItem_6);

	QTableWidgetItem *newItem_7 = new QTableWidgetItem(QString("%1").arg(m_pPlantItem->m_ReactPower.m_szLinkedTagName));
	newItem_7->setTextAlignment(Qt::AlignCenter);
	newItem_7->setFlags(newItem_7->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 3, newItem_7);

	QPushButton *pushButton = new QPushButton("");
	pushButton->setFlat(true);
	this->setCellWidget(0, 3, pushButton);
	connect(pushButton,SIGNAL(clicked()),this,SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_1 = new QPushButton("");
	pushButton_1->setFlat(true);
	this->setCellWidget(1, 3, pushButton_1);
	connect(pushButton_1, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	setColumnWidth(0, 100);
	setColumnWidth(1, 200);
	setColumnWidth(2, 100);
	setColumnWidth(3, 300);

}



CStationTableWidget::~CStationTableWidget()
{


}


CPlantInfo* CStationTableWidget::GetPlantItem() const 
{
	Q_ASSERT(m_pPlantItem);
	if (m_pPlantItem)
	{
		return m_pPlantItem;
	}
	else
	{
		return Q_NULLPTR;
	}

}


QString CStationTableWidget::GetBindValue()
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

void CStationTableWidget::Slot_SetBindValue()
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

	for (int i = 0; i<rowCount(); i++)
	{
		if (this->cellWidget(i,3) == pushBtn)
		{
			m_pPlantItem->m_vecTableInfo[i].m_szLinkedTagName = strBindValue;
		}
	}
}


void CStationTableWidget::Save()
{


}


void CStationTableWidget::Refresh()
{


}


void CStationTableWidget::SetModifyFlag(bool bFlag)
{
	Q_UNUSED(bFlag);

}

bool CStationTableWidget::GetModifyFlag()
{
	return true;
}


CPredictStationWidget::CPredictStationWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CPredictStationWidget::~CPredictStationWidget()
{


}



void CPredictStationWidget::AddToTableCtrl(CPlantInfo* pPlantItem, QString strName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CStationTableWidget* pTableWdgt = dynamic_cast<CStationTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CPlantInfo* pItem = pTableWdgt->GetPlantItem();
			if (pItem && (pItem == pPlantItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CStationTableWidget* pTableWdgt = new CStationTableWidget(pPlantItem);

	QString szName = strName + "_" + "Plant Data";

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CPredictStationWidget::ReleaseData()
{



}


void CPredictStationWidget::SetTabViewHead()
{




}

CStationStaticTableWidget::CStationStaticTableWidget(CPlantInfo* plantItem)
	:m_pPlantItem(plantItem)
{
	QStringList headStringList;

	headStringList << QObject::tr("ID") << QObject::tr("Key") << QObject::tr("Name") << QObject::tr("Description") << QObject::tr("Type");

	this->setRowCount(m_pPlantItem->m_vecTableStaticInfo.count());
	this->setColumnCount(5);
	this->setHorizontalHeaderLabels(headStringList);
	int i = 0;
	for each (auto var in m_pPlantItem->m_vecTableStaticInfo)
	{
		QTableWidgetItem *pTmp = new QTableWidgetItem(QString("%1").arg(var.m_nID));
		pTmp->setTextAlignment(Qt::AlignCenter);
		pTmp->setFlags(pTmp->flags() & (Qt::ItemIsEditable));
		this->setItem(i, 0, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(var.m_szName));
		pTmp->setTextAlignment(Qt::LeftToRight);
		pTmp->setFlags(pTmp->flags() & (Qt::ItemIsEditable));
		this->setItem(i, 1, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(var.m_strDescription));
		pTmp->setTextAlignment(Qt::LeftToRight);
		pTmp->setFlags(pTmp->flags() & (Qt::ItemIsEditable));
		this->setItem(i, 2, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(var.m_nType));
		pTmp->setTextAlignment(Qt::AlignCenter);
		pTmp->setFlags(pTmp->flags() & (Qt::ItemIsEditable));
		this->setItem(i, 3, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(var.m_szLinkedTagName));
		pTmp->setTextAlignment(Qt::AlignCenter);
		pTmp->setFlags(pTmp->flags() & (Qt::ItemIsEditable));
		this->setItem(i, 4, pTmp);

		//QPushButton *pushButton = new QPushButton("");
		//pushButton->setFlat(true);
		//this->setCellWidget(i, 3, pushButton);
		//connect(pushButton, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

		i++;
	}



	setColumnWidth(0, 100);
	setColumnWidth(1, 200);
	setColumnWidth(2, 100);
	setColumnWidth(3, 100);
	setColumnWidth(4, 300);

	this->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));
}

CStationStaticTableWidget::~CStationStaticTableWidget()
{

}

void CStationStaticTableWidget::Save()
{
}

void CStationStaticTableWidget::Refresh()
{

}

void CStationStaticTableWidget::SetModifyFlag(bool bFlag)
{

}

bool CStationStaticTableWidget::GetModifyFlag()
{
	return false;
}

CPlantInfo * CStationStaticTableWidget::GetPlantItem() const
{
	Q_ASSERT(m_pPlantItem);
	if (m_pPlantItem)
	{
		return m_pPlantItem;
	}
	else
	{
		return Q_NULLPTR;
	}
}

QString CStationStaticTableWidget::GetBindValue()
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

#include <qdebug.h>

int cmp(int a, int b)
{
	if (a > b)
		return 1; //升序排列，如果改为 a >b，则为降序,要注意sort()中cmp()的返值只有1和0，不像qsort中存在－1！！！！
	else
		return 0;
}

void CStationStaticTableWidget::on_customContextMenuRequested(const QPoint &pos)
{	
	QMenu menu;
	
	auto actionAddRow = new QAction(this);
	auto actionRemoveRow = new QAction(this);

	menu.addAction(actionAddRow);
	menu.addAction(actionRemoveRow);
	QAction *selectedItem = menu.exec(QCursor::pos());

	if (selectedItem == actionAddRow)
	{
		this->insertRow(this->rowCount());

		QString strTmp = QString("%1").arg(this->rowCount() + 1);

		QTableWidgetItem *pTmp = new QTableWidgetItem(QString("%1").arg(this->rowCount()));
		pTmp->setTextAlignment(Qt::AlignCenter);
		pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
		this->setItem(this->rowCount() - 1, 0, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(""));
		pTmp->setTextAlignment(Qt::LeftToRight);
		pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
		this->setItem(this->rowCount() - 1, 1, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(""));
		pTmp->setTextAlignment(Qt::LeftToRight);
		pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
		this->setItem(this->rowCount() - 1, 2, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(""));
		pTmp->setTextAlignment(Qt::AlignCenter);
		pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
		this->setItem(this->rowCount() - 1, 3, pTmp);

		pTmp = new QTableWidgetItem(QString("%1").arg(""));
		pTmp->setTextAlignment(Qt::AlignCenter);
		pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
		this->setItem(this->rowCount() - 1, 4, pTmp);
	}
	else if (selectedItem == actionRemoveRow)
	{
		QModelIndexList list = this->selectedIndexes();
		std::vector<int> vector;
		for each (auto var in list)
		{
			vector.push_back(var.row());
		}
		
		std::sort(vector.begin(), vector.end(), cmp);

		std::vector<int>::iterator new_end;
		new_end = unique(vector.begin(), vector.end());    //"删除"相邻的重复元素

		vector.erase(new_end, vector.end());  //删除（真正的删除）重复的元素

		for each (auto var in vector)
		{
			qDebug() << var;

			this->removeRow(var);

			delete this->takeItem(var, 0);
			delete this->takeItem(var, 1);
			delete this->takeItem(var, 2);
			delete this->takeItem(var, 3);
			delete this->takeItem(var, 4);
		}
	}
}

void CStationStaticTableWidget::Slot_SetBindValue()
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
		if (this->cellWidget(i, 3) == pushBtn)
		{
			m_pPlantItem->m_vecTableInfo[i].m_szLinkedTagName = strBindValue;
		}
	}
}

CPredictStationStaticWidget::CPredictStationStaticWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CPredictStationStaticWidget::~CPredictStationStaticWidget()
{


}



void CPredictStationStaticWidget::AddToTableCtrl(CPlantInfo* pPlantItem, QString strName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CStationStaticTableWidget* pTableWdgt = dynamic_cast<CStationStaticTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CPlantInfo* pItem = pTableWdgt->GetPlantItem();
			if (pItem && (pItem == pPlantItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CStationStaticTableWidget* pTableWdgt = new CStationStaticTableWidget(pPlantItem);

	QString szName = strName + "_" + "Plant Data" + "_Static";

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CPredictStationStaticWidget::ReleaseData()
{



}


void CPredictStationStaticWidget::SetTabViewHead()
{




}


CStationDynaticTableWidget::CStationDynaticTableWidget(CPlantInfo* plantItem)
	:m_pPlantItem(plantItem)
{
	QStringList headStringList;

	headStringList << "ID" << "NAME" << QObject::tr("Description") << "Type" << "SelectPoint";

	this->setRowCount(2);
	this->setColumnCount(5);
	this->setHorizontalHeaderLabels(headStringList);
	int i = 0;
	for each (auto var in m_pPlantItem->m_vecTableInfo)
	{
		QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(var.m_nID));
		newItem->setTextAlignment(Qt::AlignCenter);
		newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
		this->setItem(i, 0, newItem);

		QTableWidgetItem *newItem_2 = new QTableWidgetItem(QString("%1").arg(var.m_szName));
		newItem_2->setTextAlignment(Qt::LeftToRight);
		newItem_2->setFlags(newItem_2->flags() & (~Qt::ItemIsEditable));
		this->setItem(i, 1, newItem_2);

		QTableWidgetItem *pTmp = new QTableWidgetItem(QString("%1").arg(var.m_strDescription));
		pTmp->setTextAlignment(Qt::LeftToRight);
		pTmp->setFlags(pTmp->flags() | (Qt::ItemIsEditable));
		this->setItem(i, 2, pTmp);

		QTableWidgetItem *newItem_4 = new QTableWidgetItem(QString("%1").arg(var.m_nType));
		newItem_4->setTextAlignment(Qt::AlignCenter);
		newItem_4->setFlags(newItem_4->flags() & (~Qt::ItemIsEditable));
		this->setItem(i, 3, newItem_4);

		QTableWidgetItem *newItem_6 = new QTableWidgetItem(QString("%1").arg(var.m_szLinkedTagName));
		newItem_6->setTextAlignment(Qt::AlignCenter);
		newItem_6->setFlags(newItem_6->flags() & (~Qt::ItemIsEditable));
		this->setItem(i, 4, newItem_6);

		QPushButton *pushButton = new QPushButton("");
		pushButton->setFlat(true);
		this->setCellWidget(i, 4, pushButton);
		connect(pushButton, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

		i++;
	}



	setColumnWidth(0, 100);
	setColumnWidth(1, 200);
	setColumnWidth(2, 200);
	setColumnWidth(3, 100);
	setColumnWidth(4, 300);

	connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(itemChanged(QTableWidgetItem *)));
}

CStationDynaticTableWidget::~CStationDynaticTableWidget()
{

}

void CStationDynaticTableWidget::itemChanged(QTableWidgetItem *item)
{
	if (item->column() == 2 && item->row() == 0)
	{
		m_pPlantItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pPlantItem->m_ActPower.m_strDescription = item->text();
	}
	else if (item->column() == 2 && item->row() == 1)
	{
		m_pPlantItem->m_vecTableInfo[item->row()].m_strDescription = item->text();
		m_pPlantItem->m_ReactPower.m_strDescription = item->text();
	}
}

void CStationDynaticTableWidget::Save()
{
}

void CStationDynaticTableWidget::Refresh()
{

}

void CStationDynaticTableWidget::SetModifyFlag(bool bFlag)
{

}

bool CStationDynaticTableWidget::GetModifyFlag()
{
	return false;
}

CPlantInfo * CStationDynaticTableWidget::GetPlantItem() const
{
	Q_ASSERT(m_pPlantItem);
	if (m_pPlantItem)
	{
		return m_pPlantItem;
	}
	else
	{
		return Q_NULLPTR;
	}
}

QString CStationDynaticTableWidget::GetBindValue()
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

void CStationDynaticTableWidget::Slot_SetBindValue()
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
		if (this->cellWidget(i, 3) == pushBtn)
		{
			m_pPlantItem->m_vecTableInfo[i].m_szLinkedTagName = strBindValue;
		}
	}
}









CPredictStationDynaticWidget::CPredictStationDynaticWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CPredictStationDynaticWidget::~CPredictStationDynaticWidget()
{


}



void CPredictStationDynaticWidget::itemChanged(QTableWidgetItem *item)
{

}

void CPredictStationDynaticWidget::AddToTableCtrl(CPlantInfo* pPlantItem, QString strName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CStationDynaticTableWidget* pTableWdgt = dynamic_cast<CStationDynaticTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CPlantInfo* pItem = pTableWdgt->GetPlantItem();
			if (pItem && (pItem == pPlantItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CStationDynaticTableWidget* pTableWdgt = new CStationDynaticTableWidget(pPlantItem);

	QString szName = strName + "_" + "Plant Data" + "_Dynatic";

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CPredictStationDynaticWidget::ReleaseData()
{



}


void CPredictStationDynaticWidget::SetTabViewHead()
{




}




CBasicStationTableWidget::CBasicStationTableWidget(CStationData* pItem)
	:m_pStationItem(pItem)
{
	QStringList headStringList;

	headStringList << "Station_ID" << "Descrpition" << "Algorithm" << "ReverseIsolationPath";

	this->setRowCount(1);
	this->setColumnCount(4);
	this->setHorizontalHeaderLabels(headStringList);

	QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(m_pStationItem->m_nStationID));
	newItem->setTextAlignment(Qt::AlignCenter);
	newItem->setFlags(newItem->flags() | (Qt::ItemIsEditable));
	this->setItem(0, 0, newItem);

	QTableWidgetItem *newItem_2 = new QTableWidgetItem(QString("%1").arg(m_pStationItem->m_strDescrition));
	newItem_2->setTextAlignment(Qt::AlignCenter);
	newItem_2->setFlags(newItem_2->flags() | (Qt::ItemIsEditable));
	this->setItem(0, 1, newItem_2);

	QTableWidgetItem *newItem_3 = new QTableWidgetItem(QString("%1").arg(m_pStationItem->m_strAlgorithm));
	newItem_3->setTextAlignment(Qt::AlignCenter);
	newItem_3->setFlags(newItem_3->flags() | (Qt::ItemIsEditable));
	this->setItem(0, 2, newItem_3);

	QTableWidgetItem *newItem_4 = new QTableWidgetItem(QString("%1").arg(m_pStationItem->m_strReverseIsolationPath));
	newItem_4->setTextAlignment(Qt::AlignCenter);
	newItem_4->setFlags(newItem_4->flags() | (Qt::ItemIsEditable));
	this->setItem(0, 3, newItem_4);

	connect(this, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(itemChanged(QTableWidgetItem *)));
}



CBasicStationTableWidget::~CBasicStationTableWidget()
{


}

void CBasicStationTableWidget::setData(int role, const QVariant & value)
{





}


void CBasicStationTableWidget::itemChanged(QTableWidgetItem *item)
{
	if (item->column() == 1)
	{
		m_pStationItem->m_strDescrition = item->text();
	}
	else if (item->column() == 2)
	{
		m_pStationItem->m_strAlgorithm = item->text();
	}
	else if (item->column() == 3)
	{
		m_pStationItem->m_strReverseIsolationPath = item->text();
	}
	else if (item->column() == 0)
	{
		m_pStationItem->m_nStationID = item->text().toInt();
	}
}

CStationData* CBasicStationTableWidget::GetBasicStationItem() const
{
	Q_ASSERT(m_pStationItem);
	if (m_pStationItem)
	{
		return m_pStationItem;
	}
	else
	{
		return Q_NULLPTR;
	}
}


void CBasicStationTableWidget::Save()
{


}


void CBasicStationTableWidget::Refresh()
{


}


void CBasicStationTableWidget::SetModifyFlag(bool bFlag)
{
	Q_UNUSED(bFlag);

}

bool CBasicStationTableWidget::GetModifyFlag()
{
	return true;
}




CBasicStationWidget::CBasicStationWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CBasicStationWidget::~CBasicStationWidget()
{


}



void CBasicStationWidget::AddToTableCtrl(CStationData* pStationItem, QString strName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CBasicStationTableWidget* pTableWdgt = dynamic_cast<CBasicStationTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			//场站数据
			CStationData* pItem = pTableWdgt->GetBasicStationItem();
			if (pItem && (pItem == pStationItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CBasicStationTableWidget* pTableWdgt = new CBasicStationTableWidget(pStationItem);

	QString szName = strName + "_" + "Station";

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CBasicStationWidget::ReleaseData()
{



}


void CBasicStationWidget::SetTabViewHead()
{




}





