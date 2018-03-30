// #include "secconfigwgt.h"
// #include "TZhuZhanDll.h"
// #include <QDateTime>
// 
// CSecConfigWgt::CSecConfigWgt(QWidget *parent)
// 	: QDialog(parent)
// {
// 	ui.setupUi(this);
// 	connect(ui.pushButton, SIGNAL(), this, SLOT(Slot_ConnectSec()));
// 
// 	connect(ui.pushButton_4, SIGNAL(), this, SLOT(reject()));
// }
// 
// CSecConfigWgt::~CSecConfigWgt()
// {
// }
// 
// void CSecConfigWgt::Slot_ConnectSec()
// {
// 	char pAddr[20];
// 	strcpy(pAddr, ui.lineEdit->text().toStdString().c_str());
// 
// 	int nResult = ConnectHSM(pAddr,ui.lineEdit_2->text().toInt(),5000);
// 
// 	if (nResult > 0)
// 	{
// 		m_nConnect = nResult;
// 
// 	}
// 	else if (nResult == -3)
// 	{
// 
// 	}
// 	else if (nResult == -2)
// 	{
// 	}
// 	else if (nResult == -1)
// 	{
// 	}
// 	else
// 	{
// 
// 	}
// }
