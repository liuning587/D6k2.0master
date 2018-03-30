#include "sysparawgt.h"
#include "fcifile.h"
#include <QTableWidgetItem>
#include <QLayout>
CSysParaWgt::CSysParaWgt(CFciFile* pFile,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	Q_ASSERT(pFile);
	if (pFile==nullptr)
	{
		return;
	}
	m_pFile = pFile;
	
	InitUI();

	
}

CSysParaWgt::~CSysParaWgt()
{
}

bool CSysParaWgt::InitUI()
{
	QVBoxLayout * pLay = new QVBoxLayout;
	pLay->addWidget(ui.m_pSysParaTab);

	QHBoxLayout * pHLay = new QHBoxLayout;
	pHLay->addWidget(ui.m_BtnAdd);
	pHLay->addWidget(ui.m_BtnDel);
	pHLay->addWidget(ui.m_BtnOk);
	pHLay->addWidget(ui.m_BtnCancel);

	pLay->addLayout(pHLay);
	this->setLayout(pLay);

	ui.m_pSysParaTab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	
	ui.m_pSysParaTab->setRowCount(16);
	
	SYSPARA_DEF pSysPara = nullptr;

	bool bRet=m_pFile->GetSysParaInfo(pSysPara);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}

	for (int i=0;i<16;++i)
	{
		ui.m_pSysParaTab->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
	}

	ui.m_pSysParaTab->setItem(0, 1, new QTableWidgetItem( pSysPara->StationAddrDesc));
	ui.m_pSysParaTab->setItem(0, 2, new QTableWidgetItem(QString::number(pSysPara->StationAddr)));

	ui.m_pSysParaTab->setItem(1, 1, new QTableWidgetItem(pSysPara->IsSaveSoeDesc));
	ui.m_pSysParaTab->setItem(1, 2, new QTableWidgetItem(QString::number(pSysPara->IsSaveSoe)));

	ui.m_pSysParaTab->setItem(2, 1, new QTableWidgetItem(pSysPara->SoeNumDesc));
	ui.m_pSysParaTab->setItem(2, 2, new QTableWidgetItem(QString::number(pSysPara->SoeNum)));

	ui.m_pSysParaTab->setItem(3, 1, new QTableWidgetItem(pSysPara->IsSaveYCDataDesc));
	ui.m_pSysParaTab->setItem(3, 2, new QTableWidgetItem(QString::number(pSysPara->IsSaveYCData)));

	ui.m_pSysParaTab->setItem(4, 1, new QTableWidgetItem(pSysPara->SaveYCFRQDesc));
	ui.m_pSysParaTab->setItem(4, 2, new QTableWidgetItem(QString::number(pSysPara->SaveYCFRQ)));

	ui.m_pSysParaTab->setItem(5, 1, new QTableWidgetItem(pSysPara->SaveYcDaysDesc));
	ui.m_pSysParaTab->setItem(5, 2, new QTableWidgetItem(QString::number(pSysPara->SaveYcDays)));

	ui.m_pSysParaTab->setItem(6, 1, new QTableWidgetItem(pSysPara->DataTypeDesc));
	ui.m_pSysParaTab->setItem(6, 2, new QTableWidgetItem(QString::number(pSysPara->DataType)));

	ui.m_pSysParaTab->setItem(7, 1, new QTableWidgetItem(pSysPara->UpLoadYCFRQDesc));
	ui.m_pSysParaTab->setItem(7, 2, new QTableWidgetItem(QString::number(pSysPara->UpLoadYCFRQ)));

	ui.m_pSysParaTab->setItem(8, 1, new QTableWidgetItem(pSysPara->CallYCFRQDesc));
	ui.m_pSysParaTab->setItem(8, 2, new QTableWidgetItem(QString::number(pSysPara->CallYCFRQ)));

	ui.m_pSysParaTab->setItem(9, 1, new QTableWidgetItem(pSysPara->SyncFRQDesc));
	ui.m_pSysParaTab->setItem(9, 2, new QTableWidgetItem(QString::number(pSysPara->SyncFRQ)));

	ui.m_pSysParaTab->setItem(10, 1, new QTableWidgetItem(pSysPara->GPRSTimeOutDesc));
	ui.m_pSysParaTab->setItem(10, 2, new QTableWidgetItem(QString::number(pSysPara->GPRSTimeOut)));

	ui.m_pSysParaTab->setItem(11, 1, new QTableWidgetItem(pSysPara->VBatLimitDesc));
	ui.m_pSysParaTab->setItem(11, 2, new QTableWidgetItem(QString::number(pSysPara->VBatLimit)));

	ui.m_pSysParaTab->setItem(12, 1, new QTableWidgetItem(pSysPara->LbFileNumDesc));
	ui.m_pSysParaTab->setItem(12, 2, new QTableWidgetItem(QString::number(pSysPara->LbFileNum)));

	ui.m_pSysParaTab->setItem(13, 1, new QTableWidgetItem(pSysPara->ComNumDesc));
	ui.m_pSysParaTab->setItem(13, 2, new QTableWidgetItem(QString::number(pSysPara->ComNum)));

	ui.m_pSysParaTab->setItem(14, 1, new QTableWidgetItem(pSysPara->NetNumDesc));
	ui.m_pSysParaTab->setItem(14, 2, new QTableWidgetItem(QString::number(pSysPara->NetNum)));

	ui.m_pSysParaTab->setItem(15, 1, new QTableWidgetItem(pSysPara->IndNumDesc));
	ui.m_pSysParaTab->setItem(15, 2, new QTableWidgetItem(QString::number(pSysPara->IndNum)));


	return bRet;
}

void CSysParaWgt::slot_OnAdd()
{
	int nIndex = ui.m_pSysParaTab->currentRow();
	if (nIndex==-1)
	{
		ui.m_pSysParaTab->setRowCount(ui.m_pSysParaTab->rowCount()+1);
	}
	else
	{
		ui.m_pSysParaTab->insertRow(nIndex);
	}
}

void CSysParaWgt::slot_OnDel()
{
	int nIndex = ui.m_pSysParaTab->currentRow();
	if (nIndex != -1)
	{
		ui.m_pSysParaTab->removeRow(nIndex);
	}
}

void CSysParaWgt::slot_OnOk()
{
	
}

void CSysParaWgt::slot_OnCancel()
{

}
