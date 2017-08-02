#include "addstationdialog.h"

CAddStationDialog::CAddStationDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CAddStationDialog::~CAddStationDialog()
{
	
}

void CAddStationDialog::OnOk()
{
	m_nStationID = ui.lineEditStationID->text().toUInt();
	m_strDescrition = ui.lineEditDescription->text();
	m_strAlgorithm = ui.lineEditAlgorithm->text();
	m_strReverseIsolationPath = ui.lineEditReverseIsolationPath->text();

	accept();
}

void CAddStationDialog::OnCancel()
{
	reject();

	this->close();
}
