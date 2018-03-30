#pragma once

#include <QWidget>
#include "ui_gpspara.h"

class CFciFile;

class CGpsPara : public QWidget
{
	Q_OBJECT

public:
	CGpsPara(CFciFile* pFile,QWidget *parent = Q_NULLPTR);
	~CGpsPara();
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
	Ui::CGpsPara ui;
};
