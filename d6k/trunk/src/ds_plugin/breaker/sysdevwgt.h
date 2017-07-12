#pragma once

#include <QWidget>
#include "ui_sysdevwgt.h"
#include <memory>
#include "breaker_data.h"

class CNetManager;
class CSysDevWgt : public QWidget
{
	Q_OBJECT

public:
	CSysDevWgt(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CSysDevWgt();
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
	Ui::CSysDevWgt ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//����
	CNetManager *m_pNetManager;
};
