#include "gprspara.h"
#include "fcifile.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidgetItem>

CGprsPara::CGprsPara(CFciFile* pFile, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Q_ASSERT(pFile);
	if (!pFile)
	{
		return;
	}
	m_pFile = pFile;

	InitUI();
}

CGprsPara::~CGprsPara()
{
}

bool CGprsPara::InitUI()
{
	QHBoxLayout * pHLay = new QHBoxLayout;
	pHLay->addWidget(ui.m_BtnAdd);
	pHLay->addWidget(ui.m_BtnDel);
	pHLay->addWidget(ui.m_BtnOk);
	pHLay->addWidget(ui.m_BtnCancel);

	QVBoxLayout * pVlay = new QVBoxLayout;
	pVlay->addWidget(ui.m_pGprsTab);
	pVlay->addLayout(pHLay);
	this->setLayout(pVlay);

	int nNum = m_pFile->GetGprsNum();
	ui.m_pGprsTab->setRowCount(nNum);
	GPRS_DEF pGprs = nullptr;

	for (int i = 0; i < nNum; ++i)
	{
		bool bRet = m_pFile->GetGprsInfo(i , pGprs);
		Q_ASSERT(bRet && pGprs);
		if (!bRet || !pGprs)
		{
			continue;
		}
		ui.m_pGprsTab->setItem(i, 0, new QTableWidgetItem(QString::number(pGprs->OccNO)));
		ui.m_pGprsTab->setItem(i, 1, new QTableWidgetItem(QString::number(pGprs->WorkMode)));
		ui.m_pGprsTab->setItem(i, 2, new QTableWidgetItem((pGprs->IPA)));
		ui.m_pGprsTab->setItem(i, 3, new QTableWidgetItem(QString::number(pGprs->PortA)));
		ui.m_pGprsTab->setItem(i, 4, new QTableWidgetItem((pGprs->IPB)));
		ui.m_pGprsTab->setItem(i, 5, new QTableWidgetItem(QString::number(pGprs->PortB)));

		ui.m_pGprsTab->setItem(i, 6, new QTableWidgetItem((pGprs->APN)));
		ui.m_pGprsTab->setItem(i, 7, new QTableWidgetItem((pGprs->UserName)));
		ui.m_pGprsTab->setItem(i, 8, new QTableWidgetItem((pGprs->UserPwd)));
		ui.m_pGprsTab->setItem(i, 9, new QTableWidgetItem(QString::number(pGprs->ProtocolID)));
	}
	return true;
}

void CGprsPara::slot_OnAdd()
{

}

void CGprsPara::slot_OnDel()
{

}

void CGprsPara::slot_OnOk()
{

}

void CGprsPara::slot_OnCancel()
{

}
