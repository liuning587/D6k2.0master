#pragma once

#include <QDialog>
#include "ui_addstationdialog.h"

class CAddStationDialog : public QDialog
{
	Q_OBJECT

public:
	CAddStationDialog(QWidget *parent = Q_NULLPTR);
	~CAddStationDialog();

public:
	int m_nStationID;
	QString m_strDescrition;
	QString m_strAlgorithm;
	QString m_strReverseIsolationPath;
private slots:
	void OnOk();
	void OnCancel();
private:
	Ui::CAddStationDialog ui;
};
