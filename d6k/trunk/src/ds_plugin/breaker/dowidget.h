#pragma once

#include <QWidget>
#include "ui_dowidget.h"
#include "breaker_data.h"
#include <memory>


class CNetManager;
class CDoWidget : public QWidget
{
	Q_OBJECT

public:
	CDoWidget(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CDoWidget();
	//��ʼ������
	void InitData();
	QList<int> GetSelectRows();

public slots:
	void Slot_RecvNewRealTimeData(DEG_GET_MSG2 &tMeas);
	void Slot_ContextMenuRequest(const QPoint &);
	//����
	void Slot_UpdateData();
	//���ö�ֵ
	void Slot_SetDevData();

private:
	Ui::CDoWidget ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//����
	CNetManager *m_pNetManager;
};
