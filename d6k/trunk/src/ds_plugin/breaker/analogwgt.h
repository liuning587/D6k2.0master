#pragma once

#include <QWidget>
#include "ui_analogwgt.h"

class CAnalogWgt : public QWidget
{
	Q_OBJECT

public:
	CAnalogWgt(QWidget *parent = Q_NULLPTR);
	~CAnalogWgt();

private:
	Ui::CAnalogWgt ui;
};
