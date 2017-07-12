#pragma once

#include <QWidget>
#include "ui_realtimedowgt.h"

class CRealtimeDoWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeDoWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeDoWgt();
	//初始化数据
	void InitData();
	//更新值
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeDoWgt ui;
};
