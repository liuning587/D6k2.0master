#pragma once

#include <QWidget>
#include "ui_sysdevwgt.h"
#include <memory>
#include "breaker_data.h"


enum 
{
	IP_ROW_NUM = 7,
	MASK_ROW_NUM = 8,
	MASK_ROW_GATE = 9,
	IP_ROW_NUM2 = 10,
	MASK_ROW_NUM2 = 11,
	MASK_ROW_GATE2,
};
class CNetManager;
class QLineEdit;
class CSysDevWgt : public QWidget
{
	Q_OBJECT

public:
	CSysDevWgt(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CSysDevWgt();
	//初始化数据
	void InitData();
	QList<int> GetSelectRows();

	QString GetFormatData(int nData);

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
	//
	QLineEdit *m_pLineEdIp1;
	//
	QLineEdit *m_pLineEdMask1;
	//
	QLineEdit *m_pLineEdGate1;

	QLineEdit *m_pLineEdIp2;
	//
	QLineEdit *m_pLineEdMask2;
	//
	QLineEdit *m_pLineEdGate2;

};
