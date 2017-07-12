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
	//��ʼ������
	void InitData();
public slots:
	void Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas);
	void Slot_ContextMenuRequest(const QPoint &);
	//����
	void Slot_UpdateData();


private:
	Ui::CDebugWidget ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//����
	CNetManager *m_pNetManager;

};
