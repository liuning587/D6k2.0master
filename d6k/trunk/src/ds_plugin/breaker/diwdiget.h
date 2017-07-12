#pragma once

#include <QWidget>
#include "ui_diwdiget.h"
#include "breaker_data.h"
#include <memory>

class CNetManager;
class CDiWdiget : public QWidget
{
	Q_OBJECT

public:
	CDiWdiget(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CDiWdiget();
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
	Ui::CDiWdiget ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//����
	CNetManager *m_pNetManager;
};
