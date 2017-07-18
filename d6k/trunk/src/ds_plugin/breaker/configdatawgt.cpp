#include "configdatawgt.h"

CConfigDataWgt::CConfigDataWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Config"));
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

CConfigDataWgt::~CConfigDataWgt()
{
}

QString  CConfigDataWgt::GetIpAddress()
{
	return ui.lineEdit->text();
}

int CConfigDataWgt::GetPort()
{
	return ui.lineEdit_2->text().toInt();
}

void CConfigDataWgt::SetIpAddress(const QString &strIp)
{
	ui.lineEdit->setText(strIp);
}

void CConfigDataWgt::SetPort(const QString & nPort)
{
	ui.lineEdit_2->setText(nPort);
}

void CConfigDataWgt::Slot_Conform()
{
	accept();
}
