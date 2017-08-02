#include "soe_clearwgt.h"

CSoeClearWgt::CSoeClearWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("清除装置事件记录"));

	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_SelectAll()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_SelectNull()));

	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(reject()));
}

CSoeClearWgt::~CSoeClearWgt()
{
}

bool CSoeClearWgt::GetAction()
{
	return ui.checkBox->checkState() == Qt::Checked ? true : false;
}

bool CSoeClearWgt::GetDIDo()
{
	return ui.checkBox_2->checkState() == Qt::Checked ? true : false;
}

bool CSoeClearWgt::GetAbnormal()
{
	return ui.checkBox_3->checkState() == Qt::Checked ? true : false;
}

bool CSoeClearWgt::GetRun()
{
	return ui.checkBox_4->checkState() == Qt::Checked ? true : false;
}

bool CSoeClearWgt::GetWarve()
{
	return ui.checkBox_5->checkState() == Qt::Checked ? true : false;
}

bool CSoeClearWgt::GetDebug()
{
	return ui.checkBox_6->checkState() == Qt::Checked ? true : false;
}

void CSoeClearWgt::Slot_SelectAll()
{
	ui.checkBox->setCheckState(Qt::Checked);
	ui.checkBox_2->setCheckState(Qt::Checked);
	ui.checkBox_3->setCheckState(Qt::Checked);
	ui.checkBox_4->setCheckState(Qt::Checked);
	ui.checkBox_5->setCheckState(Qt::Checked);
	ui.checkBox_6->setCheckState(Qt::Checked);
}

void CSoeClearWgt::Slot_SelectNull()
{
	ui.checkBox->setCheckState(Qt::Unchecked);
	ui.checkBox_2->setCheckState(Qt::Unchecked);
	ui.checkBox_3->setCheckState(Qt::Unchecked);
	ui.checkBox_4->setCheckState(Qt::Unchecked);
	ui.checkBox_5->setCheckState(Qt::Unchecked);
	ui.checkBox_6->setCheckState(Qt::Unchecked);
}
