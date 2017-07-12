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
	//初始化数据
	void InitData();
	QList<int> GetSelectRows();

public slots:
	void Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas);
	void Slot_ContextMenuRequest(const QPoint &);
	//更新
	void Slot_UpdateData();
	//设置定值
	void Slot_SetDevData();

private:
	Ui::CSysDevWgt ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//网络
	CNetManager *m_pNetManager;
};
