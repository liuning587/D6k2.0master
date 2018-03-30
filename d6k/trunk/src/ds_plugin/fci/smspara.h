#pragma once

#include <QWidget>
#include "ui_smspara.h"
class CFciFile;

class CSmsPara : public QWidget
{
	Q_OBJECT

public:
	CSmsPara(CFciFile * pFile,QWidget *parent = Q_NULLPTR);
	~CSmsPara();

private:
	bool InitUI();
private Q_SLOTS:
	void slot_OnAdd();
	void slot_OnDel();
	void slot_OnOk();
	void slot_OnCancel();
private:
	CFciFile * m_pFile;


private:
	Ui::CSmsPara ui;
};
