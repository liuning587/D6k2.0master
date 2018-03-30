#include "protocolpara.h"
#include "fcifile.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidgetItem>

CProtocolPara::CProtocolPara(CFciFile* pFile, QWidget *parent)
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

CProtocolPara::~CProtocolPara()
{
}

bool CProtocolPara::InitUI()
{
	QHBoxLayout * pHLay = new QHBoxLayout;
	pHLay->addWidget(ui.m_BtnAdd);
	pHLay->addWidget(ui.m_BtnDel);
	pHLay->addWidget(ui.m_BtnOk);
	pHLay->addWidget(ui.m_BtnCancel);

	QVBoxLayout * pVlay = new QVBoxLayout;
	pVlay->addWidget(ui.m_ProtocolTab);
	pVlay->addLayout(pHLay);
	this->setLayout(pVlay);

	int nNum = m_pFile->GetProtocolNum();
	ui.m_ProtocolTab->setRowCount(nNum);
	PROTOCOL_DEF pProtocol = nullptr;

	for (int i = 0; i < nNum; ++i)
	{
		bool bRet = m_pFile->GetProtocolInfo(i, pProtocol);
		Q_ASSERT(bRet && pProtocol);
		if (!bRet || !pProtocol)
		{
			continue;
		}
		ui.m_ProtocolTab->setItem(i, 0, new QTableWidgetItem(QString::number(pProtocol->OccNO)));
		ui.m_ProtocolTab->setItem(i, 1, new QTableWidgetItem((pProtocol->ProtocolName)));
		ui.m_ProtocolTab->setItem(i, 2, new QTableWidgetItem(QString::number(pProtocol->LinkAddrLen)));
		ui.m_ProtocolTab->setItem(i, 3, new QTableWidgetItem(QString::number(pProtocol->CotLen)));
		ui.m_ProtocolTab->setItem(i, 4, new QTableWidgetItem(QString::number(pProtocol->CommLen)));
		ui.m_ProtocolTab->setItem(i, 5, new QTableWidgetItem(QString::number(pProtocol->InfoAddrLen)));
		ui.m_ProtocolTab->setItem(i, 6, new QTableWidgetItem(QString::number(pProtocol->LinkAddr)));
		ui.m_ProtocolTab->setItem(i, 7, new QTableWidgetItem(QString::number(pProtocol->CommonAddr)));
		ui.m_ProtocolTab->setItem(i, 8, new QTableWidgetItem(QString::number(pProtocol->ResendTimes)));
		ui.m_ProtocolTab->setItem(i, 9, new QTableWidgetItem(QString::number(pProtocol->T1)));
		ui.m_ProtocolTab->setItem(i, 10, new QTableWidgetItem(QString::number(pProtocol->T2)));
		ui.m_ProtocolTab->setItem(i, 11, new QTableWidgetItem(QString::number(pProtocol->T3)));
		ui.m_ProtocolTab->setItem(i, 12, new QTableWidgetItem(QString::number(pProtocol->T4)));
		}
	return true;
}

void CProtocolPara::slot_OnAdd()
{

}

void CProtocolPara::slot_OnDel()
{

}

void CProtocolPara::slot_OnOk()
{

}

void CProtocolPara::slot_OnCancel()
{

}
