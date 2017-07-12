#pragma once

#include <QWidget>
#include <memory>

#include "ui_debugwidget.h"
#include "breaker_data.h"
class CNetManager;


class CDebugWidget : public QWidget
{
	Q_OBJECT

public:
	CDebugWidget(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CDebugWidget();
	//初始化数据
	void InitData();
public slots:
	void Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas);
	void Slot_ContextMenuRequest(const QPoint &);
	//更新
	void Slot_UpdateData();


private:
	Ui::CDebugWidget ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//网络
	CNetManager *m_pNetManager;

};
