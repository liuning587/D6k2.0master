#pragma once

#include <QWidget>
#include <memory>

#include "ui_analogwgt.h"
#include "breaker_data.h"

class CAnalogWgt : public QWidget
{
	Q_OBJECT

public:
	CAnalogWgt(QWidget *parent = Q_NULLPTR);
	//初始化数据
	void InitData();
	~CAnalogWgt();
public slots:
	void Slot_RecvNewRealTimeData(DBG_GET_MEAS &tMeas);
signals:
	void Signal_SeoInfo(int nType);

private:
	Ui::CAnalogWgt ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
};
