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
	//��ȡ����
	CDyncEventData* GetEventData()
	{
		return m_pEventIntent;
	}

	//��������
	void LoadData();
	
public slots:
    void Slot_AddNewOrder();
	//�༭
	void Slot_EditOrder();
	//ɾ��order
	void Slot_DeleteOrder();
	//����ʱ��
	void Slot_AddOrderTime();
	//ȷ��
	void Slot_Conform();
	//˫���༭
	void Slot_DoubleClickListItem(QListWidgetItem *item);

private:
	Ui::COrderListWgt ui;
	//�������
// 	COrderEditWgt *m_pOrderEditWgt;
	//����
	CDyncEventData* m_pEventIntent;

};
