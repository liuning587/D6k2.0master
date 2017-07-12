#pragma once

#include <QWidget>
#include "ui_realtimesoftwgt.h"

class CRealtimeSoftWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeSoftWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeSoftWgt();
	//初始化数据
	void InitData();
	//更新值
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeSoftWgt ui;
};
