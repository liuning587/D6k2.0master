#include "analogwgt.h"
#include <QHeaderView>

CAnalogWgt::CAnalogWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CAnalogWgt::~CAnalogWgt()
{
}
