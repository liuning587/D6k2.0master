#pragma once

#include <QWidget>
#include <memory>
#include "ui_protectdevwgt.h"
#include "breaker_data.h"

class CNetManager;
class CProtectDevWgt : public QWidget
{
	Q_OBJECT

public:
	CProtectDevWgt(CNetManager *pNetManager,QWidget *parent = Q_NULLPTR);
	~CProtectDevWgt();
	//��ʼ������
	void InitData();
	QList<int> GetSelectRows();

public slots:
	void Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas);
	void Slot_ContextMenuRequest(const QPoint &);
	//����
	void Slot_UpdateData();
	//���ö�ֵ
	void Slot_SetDevData();
private:
	Ui::CProtectDevWgt ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//����
	CNetManager *m_pNetManager;
};
