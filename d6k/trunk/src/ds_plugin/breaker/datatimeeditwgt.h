#pragma once

#include <QDialog>
#include <QDateTime>
#include "ui_datatimeeditwgt.h"

class CDataTimeEditWgt : public QDialog
{
	Q_OBJECT

public:
	CDataTimeEditWgt(QWidget *parent = Q_NULLPTR);
	~CDataTimeEditWgt();

	QDateTime GetCurrentTime();


private:
	Ui::CDataTimeEditWgt ui;
};
