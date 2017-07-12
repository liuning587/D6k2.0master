#pragma once

#include <QWidget>
#include "ui_realtimeactionwgt.h"

class CRealtimeActionWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeActionWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeActionWgt();
	//初始化数据
	void InitData();
	//更新值
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeActionWgt ui;
};
