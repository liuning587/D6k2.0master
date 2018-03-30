#pragma once

#include <QWidget>
#include "ui_sysparawgt.h"
class CFciFile;
class CSysParaWgt : public QWidget
{
	Q_OBJECT

public:
	CSysParaWgt(CFciFile* pFile, QWidget *parent = Q_NULLPTR);
	~CSysParaWgt();
	bool InitUI();
private Q_SLOTS:
	void slot_OnAdd();
	void slot_OnDel();
	void slot_OnOk();
	void slot_OnCancel();
private:
	CFciFile * m_pFile;
	Ui::CSysParaWgt ui;
};
