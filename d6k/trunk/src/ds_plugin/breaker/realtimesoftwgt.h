#pragma once

#include <QWidget>
#include "ui_realtimesoftwgt.h"

class CRealtimeSoftWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeSoftWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeSoftWgt();
	//��ʼ������
	void InitData();
	//����ֵ
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeSoftWgt ui;
};
