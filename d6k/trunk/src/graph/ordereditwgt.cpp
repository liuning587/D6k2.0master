#include "ordereditwgt.h"
#include "editcolorthreshold.h"
#include <QComboBox>
#include <QSpinBox>
#include "scadastudio/icore.h"
#include "variant.h"
#include "graph_module.h"
#include "browsescreenwgt.h"
#include "graph_module.h"
#include "dync_var_op_event.h"
#include "dync_file_op_event.h"



COrderEditWgt::COrderEditWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_pEventItem = nullptr;

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_7->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_8->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_9->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_10->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_11->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//初始化变量数据结构
	InitVarTabInfo();
	//初始化window
	InitWindowTabInfo();
	//
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_OrderConform()));
	//取消
	connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
}

COrderEditWgt::~COrderEditWgt()
{
	if (m_pEventItem != nullptr)
	{
		delete m_pEventItem;
	}
}

void COrderEditWgt::InitVarTabInfo()
{
	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		ui.tableWidget->item(i, 0)->setFlags(ui.tableWidget->item(i, 0)->flags() ^ Qt::ItemIsEditable);;
	}
	//变量
	m_pVarCellWgt = new EditCellWgt;
	m_pVarCellWgt->SetLableHidden();
	ui.tableWidget->setCellWidget(0, 1, m_pVarCellWgt);
	connect(m_pVarCellWgt,SIGNAL(Signals_Comform()),this,SLOT(Slot_VarBindData()));

	//action
	m_pVarActionCombox = new QComboBox;
	QStringList lstActions;
	lstActions << tr("set") << tr("reset") << tr("toggle") << tr("strobe") << tr("decrease") << tr("alphanumeric pad") << tr("numeric pad")
		<< tr("Append Value") << tr("Remove Value") << tr("Swap Plus-Minus") << tr("Append Decimal Mode ON-OFF") << tr("Move Value") 
		<< tr("Reset Statistic") << tr("Move Min.Value") << tr("Move Max. Value") << tr("Move Average value") << tr("Set da ID di Stringa");
	m_pVarActionCombox->addItems(lstActions);
	ui.tableWidget->setCellWidget(1, 1, m_pVarActionCombox);


	//传递到变量
	m_pVarTransforVarCellWgt = new EditCellWgt;
	m_pVarTransforVarCellWgt->SetLableHidden();
	ui.tableWidget->setCellWidget(2, 1, m_pVarTransforVarCellWgt);
	connect(m_pVarTransforVarCellWgt, SIGNAL(Signals_Comform()), this, SLOT(Slot_VarBindData()));

	//选通时间
	m_pVarxtTime = new QSpinBox;
	ui.tableWidget->setCellWidget(4, 1, m_pVarxtTime);
	m_pVarxtTime->setMinimum(0);

	//最大字符数
	m_pVarMaxStringLegth = new QSpinBox;
	ui.tableWidget->setCellWidget(7, 1, m_pVarMaxStringLegth);
	m_pVarMaxStringLegth->setMinimum(0);

}

void COrderEditWgt::InitWindowTabInfo()
{
	for (int i = 0; i < ui.tableWidget_2->rowCount(); i++)
	{
		ui.tableWidget_2->item(i, 0)->setFlags(ui.tableWidget_2->item(i, 0)->flags() ^ Qt::ItemIsEditable);;
	}
	//变量
	m_pWindowCellWgt = new EditCellWgt;
	m_pWindowCellWgt->SetLableHidden();
	ui.tableWidget_2->setCellWidget(0, 1, m_pWindowCellWgt);
	connect(m_pWindowCellWgt, SIGNAL(Signals_Comform()), this, SLOT(Slot_ChooseWindow()));

	//action
	m_pWindowActionCombobox = new QComboBox;
	QStringList lstActions;
	lstActions << tr("Open Normal (scree change)") << tr("Open Modal (pop-up screen)") << tr("Open Frame (multi-monitor)") 
		<< tr("Open in other process (Safe Mode)") << tr("Print") << tr("Close and Return Back") << tr("Execute Synapses")
		<< tr("Open Next (open as normal next ID screen)") << tr("Open Prev. (open as normal prev. ID screen)") 
		<< tr("Capture and Print") << tr("Capture and Save");

	m_pWindowActionCombobox->addItems(lstActions);
	ui.tableWidget_2->setCellWidget(1, 1, m_pWindowActionCombobox);
}

void COrderEditWgt::CreateScreenData()
{
	CDyncFileOpEventItem *pFileItem = dynamic_cast<CDyncFileOpEventItem*>(m_pEventItem);

	if (pFileItem != nullptr)
	{
		//组装变量
		pFileItem->SetEventType(CBaseDyncEventItem::DYNC_FILE_OP);
		QComboBox *pActionBox = qobject_cast<QComboBox*>(ui.tableWidget_2->cellWidget(1, 1));
		
		if (pActionBox != nullptr)
		{
			//action
			pFileItem->m_FileOpType = static_cast<CDyncFileOpEventItem::FILE_OP_TYPE>(pActionBox->currentIndex());
		}

		//! 文件名，格式： 目录\文件 不包含后缀
		pFileItem->m_szGraphFileName = m_pWindowCellWgt->GetVarData();

		//监视器
		pFileItem->m_nMontor = ui.tableWidget_2->item(2,1)->text().toInt();

		//参数文件
		pFileItem->m_szParaFile = ui.tableWidget_2->item(3,1)->text();

		pFileItem->m_nXPos = ui.tableWidget_2->item(4, 1)->text().toInt();;
		pFileItem->m_nYPos = ui.tableWidget_2->item(5, 1)->text().toInt();;

		//宽度
		pFileItem->m_nWidth = ui.tableWidget_2->item(6, 1)->text().toInt();;
		//高度
		pFileItem->m_nHeight = ui.tableWidget_2->item(7, 1)->text().toInt();;;;
		//标题
		pFileItem->m_bTitle = ui.tableWidget_2->item(8, 0)->checkState() == Qt::Checked ? true : false;
		//边框
		pFileItem->m_bFrame = ui.tableWidget_2->item(9, 0)->checkState() == Qt::Checked ? true : false;
		//可变边框
		pFileItem->m_bChangeableFrame = ui.tableWidget_2->item(10, 0)->checkState() == Qt::Checked ? true : false;
		//系统菜单
		pFileItem->m_bSystemMenu = ui.tableWidget_2->item(11, 0)->checkState() == Qt::Checked ? true : false;
		//最大化图标
		pFileItem->m_bMaxIcon = ui.tableWidget_2->item(12, 0)->checkState() == Qt::Checked ? true : false;
		//最小化图标
		pFileItem->m_bMinIcon = ui.tableWidget_2->item(13, 0)->checkState() == Qt::Checked ? true : false;
		//保持比例打印
		pFileItem->m_bScalePrint = ui.tableWidget_2->item(14, 0)->checkState() == Qt::Checked ? true : false;
		//打印页宽
		pFileItem->m_nPrintWidth = ui.tableWidget_2->item(15, 1)->text().toInt();;
		//打印页高
		pFileItem->m_nPrintHeight = ui.tableWidget_2->item(16, 1)->text().toInt();;
		//顶边界
		pFileItem->m_nTopMargin = ui.tableWidget_2->item(17, 1)->text().toInt();;
		//底边界
		pFileItem->m_nBottomMargin = ui.tableWidget_2->item(18, 1)->text().toInt();;
		//左边界
		pFileItem->m_nLeftMargin = ui.tableWidget_2->item(19, 1)->text().toInt();;
		//右边界
		pFileItem->m_nRightMaring = ui.tableWidget_2->item(20, 1)->text().toInt();

	}
}

void COrderEditWgt::CreateVarData()
{
	CDyncVarOpEventItem *pVarItem = dynamic_cast<CDyncVarOpEventItem*>(m_pEventItem);

	if (pVarItem != nullptr)
	{
		//组装变量
		pVarItem->SetEventType(CBaseDyncEventItem::DYNC_VAR_OP);

		pVarItem->m_strBindValue = m_pVarCellWgt->GetVarData();

		QComboBox *pActionBox = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(1, 1));

		if (pActionBox != nullptr)
		{
			//action
			pVarItem->m_eAction = static_cast<CDyncVarOpEventItem::VAR_ACTION>(pActionBox->currentIndex());
		}

		//传送变量
		pVarItem->m_strTransformPara = m_pVarTransforVarCellWgt->GetVarData();

		//value
		pVarItem->m_strValue = ui.tableWidget->item(3, 1)->text();
		//strobe Time
		pVarItem->m_nStrobeTime = m_pVarxtTime->text().toInt();
		//max value
		pVarItem->m_fMaxValue = ui.tableWidget->item(5, 1)->text().toFloat();
		//min value
		pVarItem->m_fMinValue = ui.tableWidget->item(6, 1)->text().toFloat();
		//max length
		pVarItem->m_nMaxCharNums = m_pVarMaxStringLegth->text().toInt();



	}
}

void COrderEditWgt::EditEditItem(CBaseDyncEventItem* pEventItem)
{
	Q_ASSERT(pEventItem);

	if (pEventItem == nullptr)
	{
		return;
	}

	if (pEventItem->GetEventType() == CBaseDyncEventItem::DYNC_FILE_OP)
	{
		ui.tabWidget->setCurrentIndex(0);
		ui.tabWidget_2->setCurrentIndex(1);
		//文件操作
		CDyncFileOpEventItem *pFileItem = dynamic_cast<CDyncFileOpEventItem*>(pEventItem);

		Q_ASSERT(pFileItem);
		if (pFileItem != nullptr)
		{
			m_pWindowCellWgt->SetVarData(pFileItem->m_szGraphFileName);

			m_pWindowActionCombobox->setCurrentIndex(pFileItem->m_FileOpType);

			ui.tableWidget_2->item(2, 1)->setText(QString::number(pFileItem->m_nMontor));
			ui.tableWidget_2->item(3, 1)->setText(pFileItem->m_szParaFile);

			ui.tableWidget_2->item(4, 1)->setText(QString::number(pFileItem->m_nXPos));
			ui.tableWidget_2->item(5, 1)->setText(QString::number(pFileItem->m_nYPos));

			ui.tableWidget_2->item(6, 1)->setText(QString::number(pFileItem->m_nWidth));
			ui.tableWidget_2->item(7, 1)->setText(QString::number(pFileItem->m_nHeight));

			ui.tableWidget_2->item(8, 0)->setCheckState(pFileItem->m_bTitle? Qt::Checked : Qt::Unchecked);
			ui.tableWidget_2->item(9, 0)->setCheckState(pFileItem->m_bFrame ? Qt::Checked : Qt::Unchecked);
			ui.tableWidget_2->item(10, 0)->setCheckState(pFileItem->m_bChangeableFrame ? Qt::Checked : Qt::Unchecked);
			ui.tableWidget_2->item(11, 0)->setCheckState(pFileItem->m_bSystemMenu ? Qt::Checked : Qt::Unchecked);
			ui.tableWidget_2->item(12, 0)->setCheckState(pFileItem->m_bMaxIcon ? Qt::Checked : Qt::Unchecked);
			ui.tableWidget_2->item(13, 0)->setCheckState(pFileItem->m_bMinIcon ? Qt::Checked : Qt::Unchecked);
			ui.tableWidget_2->item(14, 0)->setCheckState(pFileItem->m_bScalePrint ? Qt::Checked : Qt::Unchecked);

			ui.tableWidget_2->item(15, 1)->setText(QString::number(pFileItem->m_nPrintWidth));
			ui.tableWidget_2->item(16, 1)->setText(QString::number(pFileItem->m_nPrintHeight));

			ui.tableWidget_2->item(17, 1)->setText(QString::number(pFileItem->m_nTopMargin));
			ui.tableWidget_2->item(18, 1)->setText(QString::number(pFileItem->m_nBottomMargin));

			ui.tableWidget_2->item(19, 1)->setText(QString::number(pFileItem->m_nLeftMargin));
			ui.tableWidget_2->item(20, 1)->setText(QString::number(pFileItem->m_nRightMaring));

		}
	}
	else if (pEventItem->GetEventType() == CBaseDyncEventItem::DYNC_VAR_OP)
	{
		//变量
		ui.tabWidget->setCurrentIndex(0);
		ui.tabWidget_2->setCurrentIndex(0);
		//变量
		CDyncVarOpEventItem *pVarItem = dynamic_cast<CDyncVarOpEventItem*>(pEventItem);

		Q_ASSERT(pVarItem);
		if (pVarItem != nullptr)
		{
			m_pVarCellWgt->SetVarData(pVarItem->m_strBindValue);
			
			m_pVarActionCombox->setCurrentIndex(pVarItem->m_eAction);

			m_pVarTransforVarCellWgt->SetVarData(pVarItem->m_strTransformPara);

			ui.tableWidget->item(3, 1)->setText(pVarItem->m_strValue);

			m_pVarxtTime->setValue(pVarItem->m_nStrobeTime);

			ui.tableWidget->item(5, 1)->setText(QString::number(pVarItem->m_fMaxValue));

			ui.tableWidget->item(6, 1)->setText(QString::number(pVarItem->m_fMinValue));

			m_pVarMaxStringLegth->setValue(pVarItem->m_nMaxCharNums);

		}
	}

}

void COrderEditWgt::Slot_VarBindData()
{

	EditCellWgt *pCellwgt = qobject_cast<EditCellWgt*>(sender());
	Q_ASSERT(pCellwgt);
	if (pCellwgt == nullptr)
	{
		return;
	}

	std::string strNodeTagname;
	std::string strTagname;
	std::string strFilename;
	//
	std::vector<int> arrDataType;
	arrDataType.push_back(DT_NULL);
	arrDataType.push_back(DT_BOOLEAN);
	arrDataType.push_back(DT_CHAR);
	arrDataType.push_back(DT_BYTE);
	arrDataType.push_back(DT_SHORT);
	arrDataType.push_back(DT_WORD);
	arrDataType.push_back(DT_INT);
	arrDataType.push_back(DT_DWORD);
	arrDataType.push_back(DT_LLONG);
	arrDataType.push_back(DT_LWORD);
	arrDataType.push_back(DT_FLOAT);
	arrDataType.push_back(DT_DOUBLE);
	arrDataType.push_back(DT_STRING);
	arrDataType.push_back(DT_BLOCK);
	arrDataType.push_back(DT_MAX);
	//
	bool bSelectFlag = GetIMainInterface()->SelectTag(arrDataType, true, strNodeTagname, strTagname, strFilename);

	if (bSelectFlag)
	{
		QString strBindData = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());
		pCellwgt->SetVarData(strBindData);
	}
	else
	{
		pCellwgt->SetVarData("");
	}
}

void COrderEditWgt::Slot_ChooseWindow()
{
	CBrowseScreenWgt *pBrowseWgt = new CBrowseScreenWgt;
	QString strFilename = GetModuleApi()->GetGraphicPath();

	strFilename.remove(strFilename.length() - 1, 1);
	strFilename.remove( "/" + strFilename.split("/").last());

	strFilename = strFilename + "/" + strFilename.split("/").last() + ".proj";


	pBrowseWgt->LoadWindowData(strFilename);
	if (pBrowseWgt->exec())
	{
		//
		m_pWindowCellWgt->SetVarData(pBrowseWgt->GetFilename());
	}
	pBrowseWgt->deleteLater();
}

void COrderEditWgt::Slot_OrderConform()
{
	//确认

	//判断当前页面
	if (ui.tabWidget->currentIndex() == 0)
	{
		//基础
		if (ui.tabWidget_2->currentIndex() == 0)
		{
			//变量
			m_pEventItem = new CDyncVarOpEventItem;
			CreateVarData();
		}
		else if (ui.tabWidget_2->currentIndex() == 1)
		{
			// 窗口
			m_pEventItem = new CDyncFileOpEventItem;
			CreateScreenData();
		}
		else if (ui.tabWidget_2->currentIndex() == 2)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 3)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 4)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 5)
		{
		}
	}
	else if (ui.tabWidget->currentIndex() == 1)
	{
		//操作
		if (ui.tabWidget_3->currentIndex() == 0)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
	}
	accept();
}
