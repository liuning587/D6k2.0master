#pragma once

#include "datatypes.h"
#include "fesapi/fescnst.h"
#include <QDialog>
#include <memory>
#include <QDialog>
#include "ui_aoutoperdialog.h"
class CDbSvc;
class QTimer;
class CViewMemDB;

class CAoutOperDialog : public QDialog
{
	Q_OBJECT

public:
	CAoutOperDialog(QWidget *parent, int32u nNodeOccNo , int32u nOccNo, std::shared_ptr<CViewMemDB>& pMem);
	~CAoutOperDialog();

protected:
	void InitView();
protected Q_SLOTS:
	void slot_OnBtnAutoExec();
	void RefreshView();
private:
	Ui::CAoutOperDialog ui;
	int32u m_nAoutOccNo;
	int32u m_nNodeOccNo;
	std::shared_ptr<CViewMemDB>& m_pMemDB;

	QTimer* m_pTimer;
};
