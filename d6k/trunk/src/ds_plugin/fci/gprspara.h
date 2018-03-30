#pragma once

#include <QWidget>
#include "ui_gprspara.h"
class CFciFile;

class CGprsPara : public QWidget
{
	Q_OBJECT

public:
	CGprsPara(CFciFile* pFile,QWidget *parent = Q_NULLPTR);
	~CGprsPara();
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
	Ui::CGprsPara ui;
};
