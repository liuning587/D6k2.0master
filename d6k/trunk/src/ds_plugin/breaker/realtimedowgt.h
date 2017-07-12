#pragma once

#include <QWidget>
#include "ui_realtimedowgt.h"

class CRealtimeDoWgt : public QWidget
{
	Q_OBJECT

public:
	CRealtimeDoWgt(QWidget *parent = Q_NULLPTR);
	~CRealtimeDoWgt();
	//��ʼ������
	void InitData();
	//����ֵ
	void UpdateItem(int nValue);

private:
	Ui::CRealtimeDoWgt ui;
};
