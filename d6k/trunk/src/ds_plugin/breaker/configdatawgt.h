#pragma once

#include <QDialog>
#include "ui_configdatawgt.h"

class CConfigDataWgt : public QDialog
{
	Q_OBJECT

public:
	CConfigDataWgt(QWidget *parent = Q_NULLPTR);
	~CConfigDataWgt();

	QString GetIpAddress();
	int GetPort();
public slots:
    void Slot_Conform();

private:
	Ui::CConfigDataWgt ui;
};
