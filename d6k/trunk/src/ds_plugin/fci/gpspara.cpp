#include "gpspara.h"
#include "fcifile.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidgetItem>

CGpsPara::CGpsPara(CFciFile* pFile, QWidget *parent)
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

CGpsPara::~CGpsPara()
{
}

bool CGpsPara::InitUI()
{
	QHBoxLayout * pHLay = new QHBoxLayout;
	pHLay->addWidget(ui.m_BtnAdd);
	pHLay->addWidget(ui.m_BtnDel);
	pHLay->addWidget(ui.m_BtnOk);
	pHLay->addWidget(ui.m_BtnCancel);

	QVBoxLayout * pVlay = new QVBoxLayout;
	pVlay->addWidget(ui.m_GpsTab);
	pVlay->addLayout(pHLay);
	this->setLayout(pVlay);

	int nNum = m_pFile->GetGpsNum();
	ui.m_GpsTab->setRowCount(nNum);
	GPS_DEF pGps = nullptr;

	for (int i=0;i<nNum;++i)
	{
		bool bRet = m_pFile->GetGpsInfo(i,pGps);
		Q_ASSERT(bRet && pGps);
		if (!bRet || !pGps)
		{
			continue;
		}
		ui.m_GpsTab->setItem(i, 0, new QTableWidgetItem(QString::number(pGps->OccNO)));
		ui.m_GpsTab->setItem(i, 1, new QTableWidgetItem(QString::number(pGps->Enable)));
		ui.m_GpsTab->setItem(i, 2, new QTableWidgetItem(QString::number(pGps->SysInterval)));
		ui.m_GpsTab->setItem(i, 3, new QTableWidgetItem(QString::number(pGps->MaxRecvTime)));
	}

	return true;
}

void CGpsPara::slot_OnAdd()
{

}

void CGpsPara::slot_OnDel()
{

}

void CGpsPara::slot_OnOk()
{

}

void CGpsPara::slot_OnCancel()
{

}
