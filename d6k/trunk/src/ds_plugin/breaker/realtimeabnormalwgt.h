#pragma once

#include <QWidget>
#include "ui_realtimeabnormalwgt.h"

class CRealtimeAbnormalWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeAbnormalWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeAbnormalWgt();
	//��ʼ������
	void InitData();
	//����ֵ
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeAbnormalWgt ui;
};
