#include "uartpara.h"
#include "fcifile.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

CUartPara::CUartPara(CFciFile* pFile,QWidget *parent)
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

CUartPara::~CUartPara()
{
}

bool CUartPara::InitUI()
{
	QHBoxLayout * pHLay = new QHBoxLayout;
	pHLay->addWidget(ui.m_BtnAdd);
	pHLay->addWidget(ui.m_BtnDel);
	pHLay->addWidget(ui.m_BtnOk);
	pHLay->addWidget(ui.m_BtnCancel);

	QVBoxLayout * pVlay = new QVBoxLayout;
	pVlay->addWidget(ui.m_UartTab);
	pVlay->addLayout(pHLay);
	this->setLayout(pVlay);


	UART_DEF pUart = nullptr;

	int nNum = m_pFile->GetUartNum();
	ui.m_UartTab->setRowCount(nNum);

	for (int i = 0; i < nNum; ++i)
	{
		bool bRet = m_pFile->GetUartInfo(i,pUart);
		Q_ASSERT(bRet);
		if (!bRet)
		{
			continue;
		}
		Q_ASSERT(pUart);
		if (!pUart)
		{
			continue;
		}
		ui.m_UartTab->setItem(i, 0, new QTableWidgetItem(QString::number(pUart->OccNO)));
		ui.m_UartTab->setItem(i, 1, new QTableWidgetItem(QString::number(pUart->ComID)));
		ui.m_UartTab->setItem(i, 2, new QTableWidgetItem(QString::number(pUart->UseType)));
		ui.m_UartTab->setItem(i, 3, new QTableWidgetItem(QString::number(pUart->Band)));
		ui.m_UartTab->setItem(i, 4, new QTableWidgetItem(QString::number(pUart->ParityBit)));
		ui.m_UartTab->setItem(i, 5, new QTableWidgetItem(QString::number(pUart->DataBit)));
		ui.m_UartTab->setItem(i, 6, new QTableWidgetItem(QString::number(pUart->StopBit)));
		ui.m_UartTab->setItem(i, 7, new QTableWidgetItem(QString::number(pUart->ProtocolID)));
	}

	return true;
}

void CUartPara::slot_OnAdd()
{

}

void CUartPara::slot_OnDel()
{

}

void CUartPara::slot_OnOk()
{

}

void CUartPara::slot_OnCancel()
{

}
