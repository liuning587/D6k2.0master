#include "smspara.h"
#include "fcifile.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidgetItem>

CSmsPara::CSmsPara(CFciFile * pFile, QWidget *parent)
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

CSmsPara::~CSmsPara()
{
}

bool CSmsPara::InitUI()
{
	QHBoxLayout * pHLay = new QHBoxLayout;
	pHLay->addWidget(ui.m_BtnAdd);
	pHLay->addWidget(ui.m_BtnDel);
	pHLay->addWidget(ui.m_BtnOk);
	pHLay->addWidget(ui.m_BtnCancel);

	QVBoxLayout * pVlay = new QVBoxLayout;
	pVlay->addWidget(ui.m_SmsTab);
	pVlay->addLayout(pHLay);
	this->setLayout(pVlay);

	int nNum = m_pFile->GetSmsNum();
	ui.m_SmsTab->setRowCount(nNum);
	SMS_DEF pSms = nullptr;

	for (int i = 0; i < nNum; ++i)
	{
		bool bRet = m_pFile->GetSmsInfo(i, pSms);
		Q_ASSERT(bRet && pSms);
		if (!bRet || !pSms)
		{
			continue;
		}
		ui.m_SmsTab->setItem(i, 0, new QTableWidgetItem(QString::number(pSms->OccNO)));
		ui.m_SmsTab->setItem(i, 1, new QTableWidgetItem((pSms->StatationNum)));
		ui.m_SmsTab->setItem(i, 2, new QTableWidgetItem((pSms->StatationNumDesc)));
	}

	return true;
}

void CSmsPara::slot_OnAdd()
{

}

void CSmsPara::slot_OnDel()
{

}

void CSmsPara::slot_OnOk()
{

}

void CSmsPara::slot_OnCancel()
{

}
