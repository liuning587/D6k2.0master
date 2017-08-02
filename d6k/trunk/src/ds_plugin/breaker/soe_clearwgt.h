#pragma once

#include <QDialog>
#include "ui_soe_clearwgt.h"

class CSoeClearWgt : public QDialog
{
	Q_OBJECT

public:
	CSoeClearWgt(QWidget *parent = Q_NULLPTR);
	~CSoeClearWgt();
public:
	//action
	bool GetAction();
	//dido
	bool GetDIDo();
	//abnromal
	bool GetAbnormal();
	//run
	bool GetRun();
	//Â¼²¨
	bool GetWarve();
	//µ÷ÊÔÐÅÏ¢
	bool GetDebug();

public slots:
    void Slot_SelectAll();
	//
	void Slot_SelectNull();
	//

private:
	Ui::CSoeClearWgt ui;
};
