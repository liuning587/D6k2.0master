#pragma once

#include <QDialog>
#include "ui_orderlistwgt.h"

// class COrderEditWgt;
class CDyncEventData;
class COrderListWgt : public QDialog
{
	Q_OBJECT

public:
	COrderListWgt(QWidget *parent = Q_NULLPTR);
	~COrderListWgt();
	//获取数据
	CDyncEventData* GetEventData()
	{
		return m_pEventIntent;
	}

	//加载数据
	void LoadData();
	
public slots:
    void Slot_AddNewOrder();
	//编辑
	void Slot_EditOrder();
	//删除order
	void Slot_DeleteOrder();
	//增加时间
	void Slot_AddOrderTime();
	//确认
	void Slot_Conform();
	//双机编辑
	void Slot_DoubleClickListItem(QListWidgetItem *item);

private:
	Ui::COrderListWgt ui;
	//命令操作
// 	COrderEditWgt *m_pOrderEditWgt;
	//命令
	CDyncEventData* m_pEventIntent;

};
