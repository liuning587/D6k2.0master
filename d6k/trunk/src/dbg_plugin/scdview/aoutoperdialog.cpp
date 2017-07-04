#include "aoutoperdialog.h"
#include "fesapi/fesdatadef.h"
#include "scadaapi/scdsvcapi.h"
#include "fesdb.h"
#include "memdb.h"
#include <QTimer>


CAoutOperDialog::CAoutOperDialog(QWidget *parent,int32u nNodeOccNo, int32u nOccNo, std::shared_ptr<CViewMemDB>& pMem)
	: QDialog(parent),m_nNodeOccNo(nNodeOccNo), m_nAoutOccNo(nOccNo),m_pMemDB(pMem)
{
	ui.setupUi(this);
	Q_ASSERT(m_nAoutOccNo != INVALID_OCCNO && m_nAoutOccNo <= MAX_OCCNO);
	if (m_nAoutOccNo == INVALID_OCCNO || m_nAoutOccNo > MAX_OCCNO)
	{
		return;
	}
	Q_ASSERT(m_nNodeOccNo!=INVALID_OCCNO && m_nNodeOccNo <=MAX_NODE_OCCNO);
	if (m_nNodeOccNo==INVALID_OCCNO || m_nNodeOccNo >MAX_NODE_OCCNO)
	{
		return;
	}
	Q_ASSERT(m_pMemDB);
	if (!m_pMemDB)
	{
		return;
	}
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	InitView();
	m_pTimer = new QTimer(this);
	connect(m_pTimer, &QTimer::timeout, this, &CAoutOperDialog::RefreshView);
	connect(ui.m_btnAutoExec, &QPushButton::clicked, this, &CAoutOperDialog::slot_OnBtnAutoExec);
	m_pTimer->start(1000);
}

CAoutOperDialog::~CAoutOperDialog()
{
}

void CAoutOperDialog::InitView()
{
	AOUT *pAout=nullptr;
	CFesDBR * pFes=m_pMemDB->GetFesByOccNo(m_nNodeOccNo);
	Q_ASSERT(pFes);
	 
	if (!pFes)
	{
		return;
	}
	pAout = pFes->GetAoutByOccNo(m_nAoutOccNo);
	Q_ASSERT(pAout);

	if (!pAout)
	{
		return;
	}

	//基本信息
	ui.m_labOccNo->setText(QString::number(pAout->OccNo));
	ui.m_labBlockNo->setText(QString::number(pAout->BlockNo));
	ui.m_labNameOccNo->setText(QString::number(pAout->NameOccNo));
	ui.m_labSpare->setText(QString::number(pAout->IsSpare));
	ui.m_labAddress->setText(QString(pAout->Address));

	ui.m_labNodeOccNo->setText(QString::number(pAout->NodeOccNo));
	ui.m_labChannelOccNo->setText(QString::number(pAout->ChannelOccNo));
	ui.m_labDeviceOccNo->setText(QString::number(pAout->DeviceOccNo));
	ui.m_labPinOccNo->setText(QString::number(pAout->PinLabelOccNo));

	//实时信息
	ui.m_labState->setText(QString::number(pAout->State));
	ui.m_labIsdefined->setText(QString::number(pAout->IsDefined));
	ui.m_labScan->setText(QString::number(pAout->ScanEnable));
	ui.m_labInit->setText(QString::number(pAout->Init));
	ui.m_labQuality->setText(QString::number(pAout->Quality));

	ui.m_labOutput->setText(QString::number(pAout->Output));
	ui.m_labRawData->setText(QString::number(pAout->RawData));
	ui.m_labDataSource->setText(QString::number(pAout->DataSource));
}

void CAoutOperDialog::slot_OnBtnAutoExec()
{
	if (ui.m_EditYtVal->text().isEmpty())
	{
		return;
	}
	float fVal = ui.m_EditYtVal->text().toFloat();

	IO_VARIANT nVar;
	S_FLOAT(&nVar,&fVal);
	
	PutRTData(IDD_AOUT,  m_nNodeOccNo, m_nAoutOccNo, ATT_AOUT, &nVar ,nullptr,nullptr);
}

void CAoutOperDialog::RefreshView()
{
	AOUT *pAout = nullptr;
	CFesDBR * pFes = m_pMemDB->GetFesByOccNo(m_nNodeOccNo);
	Q_ASSERT(pFes);

	if (!pFes)
	{
		return;
	}
	pAout = pFes->GetAoutByOccNo(m_nAoutOccNo);
	Q_ASSERT(pAout);

	if (!pAout)
	{
		return;
	}
	//实时信息
	ui.m_labState->setText(QString::number(pAout->State));
	ui.m_labIsdefined->setText(QString::number(pAout->IsDefined));
	ui.m_labScan->setText(QString::number(pAout->ScanEnable));
	ui.m_labInit->setText(QString::number(pAout->Init));
	ui.m_labQuality->setText(QString::number(pAout->Quality));

	ui.m_labOutput->setText(QString::number(pAout->Output));
	ui.m_labRawData->setText(QString::number(pAout->RawData));
	ui.m_labDataSource->setText(QString::number(pAout->DataSource));
}
