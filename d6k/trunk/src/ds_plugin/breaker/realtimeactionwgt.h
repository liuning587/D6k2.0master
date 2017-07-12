#pragma once

#include <QWidget>
#include "ui_realtimeactionwgt.h"

class CRealtimeActionWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeActionWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeActionWgt();
	//��ʼ������
	void InitData();
	//����ֵ
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeActionWgt ui;
};
