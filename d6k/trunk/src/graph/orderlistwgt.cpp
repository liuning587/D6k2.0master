#include "orderlistwgt.h"
#include "ordereditwgt.h"
#include "dync_event.h"
#include "dynamic_factory.h"
#include "base_dync_event_item.h"
#include "dync_file_op_event.h"
#include "dync_var_op_event.h"

#include "graph_module.h"
#include <QMessageBox>

COrderListWgt::COrderListWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_AddNewOrder()));

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_EditOrder()));

	connect(ui.pushButton_3,SIGNAL(clicked()),this,SLOT(Slot_DeleteOrder()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(Slot_AddOrderTime()));

	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(Slot_Conform()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(close()));

	connect(ui.listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(Slot_DoubleClickListItem(QListWidgetItem *)));

	m_pEventIntent = new CDyncEventData;
}

COrderListWgt::~COrderListWgt()
{
	delete m_pEventIntent;
}

void COrderListWgt::LoadData()
{
	//加载数据

	for (auto eventItem : m_pEventIntent->getEvents())
	{
		//加载数据
		QListWidgetItem *pItem = new QListWidgetItem;
		ui.listWidget->addItem(pItem);

		if (eventItem->GetEventType() == CBaseDyncEventItem::DYNC_FILE_OP)
		{
			//文件操作
			CDyncFileOpEventItem *pEventItem = dynamic_cast<CDyncFileOpEventItem*>(eventItem);

			if (pEventItem != nullptr)
			{
				pItem->setText(tr("Screen - ") + (pEventItem->m_arrFileOpTypeTxt)[pEventItem->m_FileOpType] + "  -  " + pEventItem->m_szGraphFileName);

				pItem->setData(Qt::UserRole + 1, reinterpret_cast<long long>(pEventItem));
			}

		}
		else if (eventItem->GetEventType() == CBaseDyncEventItem::DYNC_VAR_OP)
		{
			//变量操作
			CDyncVarOpEventItem *pVarItem = dynamic_cast<CDyncVarOpEventItem*>(eventItem);

			if (pVarItem != nullptr)
			{
				pItem->setText(tr("Var - ") + (pVarItem->m_arrVarOpTypeTxt)[pVarItem->m_eAction] + "  -  " + pVarItem->m_strBindValue);

				pItem->setData(Qt::UserRole + 1, reinterpret_cast<long long>(pVarItem));
			}
		}
	}
}

void COrderListWgt::Slot_AddNewOrder()
{

	COrderEditWgt* pOrderEditWgt = new COrderEditWgt;

	if (pOrderEditWgt->exec())
	{
		//加载数据
		QListWidgetItem *pItem = new QListWidgetItem;
		ui.listWidget->addItem(pItem);

		if (pOrderEditWgt->GetEventItem()->GetEventType() == CBaseDyncEventItem::DYNC_FILE_OP)
		{
			//文件操作
			CDyncFileOpEventItem *pEventItem = dynamic_cast<CDyncFileOpEventItem*>(pOrderEditWgt->GetEventItem());
			if (pEventItem != nullptr)
			{
				pItem->setText(tr("Screen - ") + (pEventItem->m_arrFileOpTypeTxt)[pEventItem->m_FileOpType] + "  -  " + pEventItem->m_szGraphFileName);

				//创建数据
				CBaseDyncEventItem *pNewEventItem = m_pEventIntent->CreateEventItem(pEventItem);
				pItem->setData(Qt::UserRole+1,reinterpret_cast<long long>(pNewEventItem));
			}

			
		}
		else if (pOrderEditWgt->GetEventItem()->GetEventType() == CBaseDyncEventItem::DYNC_VAR_OP)
		{
			//变量操作
			CDyncVarOpEventItem *pVarItem = dynamic_cast<CDyncVarOpEventItem*>(pOrderEditWgt->GetEventItem());
			if (pVarItem != nullptr)
			{
				pItem->setText(tr("Var - ") + (pVarItem->m_arrVarOpTypeTxt)[pVarItem->m_eAction] + "  -  " + pVarItem->m_strBindValue);

				//创建数据
				CBaseDyncEventItem *pNewVarItem = m_pEventIntent->CreateEventItem(pVarItem);

				pItem->setData(Qt::UserRole + 1, reinterpret_cast<long long>(pNewVarItem));
			}
		}
	}

	pOrderEditWgt->deleteLater();
	
}

void COrderListWgt::Slot_EditOrder()
{
	//编辑
	QListWidgetItem *pItem = ui.listWidget->currentItem();
	if (pItem == nullptr)
	{
		return;
	}
	//当前处理的对象
	CBaseDyncEventItem* pCurrentEventItem = reinterpret_cast<CBaseDyncEventItem*>(pItem->data(Qt::UserRole + 1).toLongLong());

	Q_ASSERT(pCurrentEventItem);
	if (pCurrentEventItem == nullptr)
	{
		return;
	}

	COrderEditWgt* pOrderEditWgt = new COrderEditWgt;
	pOrderEditWgt->EditEditItem(pCurrentEventItem);

	if (pOrderEditWgt->exec())
	{
		//加载数据
		
		if (pItem != nullptr)
		{
			if (pOrderEditWgt->GetEventItem()->GetEventType() == CBaseDyncEventItem::DYNC_FILE_OP)
			{
				//文件操作
				CDyncFileOpEventItem *pEventItem = dynamic_cast<CDyncFileOpEventItem*>(pOrderEditWgt->GetEventItem());

				if (pEventItem != nullptr)
				{
					CDyncFileOpEventItem *pModifyEventItem = dynamic_cast<CDyncFileOpEventItem*>(pCurrentEventItem);

					Q_ASSERT(pModifyEventItem);
					if (pModifyEventItem != nullptr)
					{
						*pModifyEventItem = *pEventItem;
					}

				}

				
			}
			else if (pOrderEditWgt->GetEventItem()->GetEventType() == CBaseDyncEventItem::DYNC_VAR_OP)
			{
				// var操作
				CDyncVarOpEventItem *pVarItem = dynamic_cast<CDyncVarOpEventItem*>(pOrderEditWgt->GetEventItem());

				if (pVarItem != nullptr)
				{

					CDyncVarOpEventItem *pModifyEventItem = dynamic_cast<CDyncVarOpEventItem*>(pCurrentEventItem);

					Q_ASSERT(pModifyEventItem);
					if (pModifyEventItem != nullptr)
					{
						*pModifyEventItem = *pVarItem;
					}

				}
			}
		}

	}

	pOrderEditWgt->deleteLater();
}

void COrderListWgt::Slot_DeleteOrder()
{
	//删除命令
	QListWidgetItem *pItem = ui.listWidget->currentItem();

	CBaseDyncEventItem* pCurrentEventItem = reinterpret_cast<CBaseDyncEventItem*>(pItem->data(Qt::UserRole + 1).toLongLong());

	if (pCurrentEventItem != nullptr)
	{
		m_pEventIntent->DeleteOpEvent(pCurrentEventItem);
	}
	delete pItem;
	
}

void COrderListWgt::Slot_AddOrderTime()
{
	QMessageBox::information(this,"hello", "hello world!");
}

void COrderListWgt::Slot_Conform()
{
	//更新值
	if (ui.listWidget->count() != 0)
	{
		m_pEventIntent->m_bEnable = true;
	}
	else
	{
		m_pEventIntent->m_bEnable = false;
	}

	accept();
}

//双机显示
void COrderListWgt::Slot_DoubleClickListItem(QListWidgetItem *item)
{
	Slot_EditOrder();
}
