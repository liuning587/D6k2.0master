#pragma once

#include <QWidget>
#include "ui_uartpara.h"
class CFciFile;
class CUartPara : public QWidget
{
	Q_OBJECT

public:
	CUartPara(CFciFile* pFile, QWidget *parent = Q_NULLPTR);
	~CUartPara();
private:
	bool InitUI();
	private Q_SLOTS:
	void slot_OnAdd();
	void slot_OnDel();
	void slot_OnOk();
	void slot_OnCancel();
private:
	CFciFile* m_pFile;
private:
	Ui::CUartPara ui;
};
