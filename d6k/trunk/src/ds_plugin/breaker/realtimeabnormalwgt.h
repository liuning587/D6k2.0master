#pragma once

#include <QWidget>
#include "ui_realtimeabnormalwgt.h"

class CRealtimeAbnormalWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeAbnormalWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeAbnormalWgt();
	//初始化数据
	void InitData();
	//更新值
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeAbnormalWgt ui;
};
