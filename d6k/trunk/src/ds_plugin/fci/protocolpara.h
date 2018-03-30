#pragma once

#include <QWidget>
#include "ui_protocolpara.h"
class CFciFile;

class CProtocolPara : public QWidget
{
	Q_OBJECT

public:
	CProtocolPara(CFciFile* pFile, QWidget *parent = Q_NULLPTR);
	~CProtocolPara();
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
	Ui::CProtocolPara ui;
};
