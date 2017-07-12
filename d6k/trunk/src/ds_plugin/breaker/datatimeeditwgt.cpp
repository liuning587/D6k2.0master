#include "datatimeeditwgt.h"

CDataTimeEditWgt::CDataTimeEditWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.timeEdit->setTime(QTime::currentTime());
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

CDataTimeEditWgt::~CDataTimeEditWgt()
{
}

QDateTime CDataTimeEditWgt::GetCurrentTime()
{
	return QDateTime(ui.calendarWidget->selectedDate(), ui.timeEdit->time());
}
