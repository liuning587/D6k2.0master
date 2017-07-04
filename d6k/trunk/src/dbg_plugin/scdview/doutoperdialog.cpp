#include "doutoperdialog.h"
#include "fesapi/fesdatadef.h"
#include "scadaapi/scdsvcapi.h"
#include "fesdb.h"
#include "memdb.h"
#include <QTimer>

CDoutOperDialog::CDoutOperDialog(QWidget *parent, int32u nNodeOccNo, int32u nOccNo, std::shared_ptr<CViewMemDB>& pMem)
	: QDialog(parent), m_nNodeOccNo(nNodeOccNo), m_nDoutOccNo(nOccNo), m_pMemDB(pMem)
{
	ui.setupUi(this);

	Q_ASSERT(m_nDoutOccNo!=INVALID_OCCNO && m_nDoutOccNo<=MAX_OCCNO);
	if (m_nDoutOccNo==INVALID_OCCNO || m_nDoutOccNo >MAX_OCCNO)
	{
		return;
	}

	Q_ASSERT(m_nNodeOccNo != INVALID_OCCNO && m_nNodeOccNo <= MAX_NODE_OCCNO);
	if (m_nNodeOccNo == INVALID_OCCNO || m_nNodeOccNo > MAX_NODE_OCCNO)
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
	connect(m_pTimer, &QTimer::timeout, this, &CDoutOperDialog::RefreshView);
	connect(ui.m_btnSelect,&QPushButton::clicked,this,&CDoutOperDialog::slot_on_BtnSelect);
	connect(ui.m_btnPre, &QPushButton::clicked, this, &CDoutOperDialog::slot_on_BtnPre);
	connect(ui.m_btnExec, &QPushButton::clicked, this, &CDoutOperDialog::slot_on_BtnExec);
	connect(ui.m_btnAutoExec, &QPushButton::clicked, this, &CDoutOperDialog::slot_on_AutoExec);
	m_pTimer->start(1000);
}

CDoutOperDialog::~CDoutOperDialog()
{

}

/*! \fn void CDoutOperDialog::InitView()
********************************************************************************************************* 
** \brief CDoutOperDialog::InitView 
** \details 初始化遥控界面
** \return void 
** \author xingzhibing
** \date 2017年2月28日 
** \note 
********************************************************************************************************/
void CDoutOperDialog::InitView()
{
	DOUT *pDout = nullptr;
	
	CFesDBR * pFes = m_pMemDB->GetFesByOccNo(m_nNodeOccNo);
	Q_ASSERT(pFes);

	if (!pFes)
	{
		return;
	}
	pDout = pFes->GetDoutByOccNo(m_nDoutOccNo);
	Q_ASSERT(pDout);

	if (!pDout)
	{
		return;
	}

	//基本信息
	ui.m_labOccNo->setText(QString::number(pDout->OccNo));
	ui.m_labBlockNo->setText(QString::number(pDout->BlockNo));
	ui.m_labNameOccNo->setText(QString::number(pDout->NameOccNo));
	ui.m_labSpare->setText(QString::number(pDout->IsSpare));
	ui.m_labAddress->setText(QString(pDout->Address));

	ui.m_labNodeOccNo->setText(QString::number(pDout->NodeOccNo));
	ui.m_labChannelOccNo->setText(QString::number(pDout->ChannelOccNo));
	ui.m_labDeviceOccNo->setText(QString::number(pDout->DeviceOccNo));
	ui.m_labPinOccNo->setText(QString::number(pDout->PinLabelOccNo));
	
	//实时信息
	ui.m_labState->setText(QString::number(pDout->State));
	ui.m_labIsdefined->setText(QString::number(pDout->IsDefined));
	ui.m_labScan->setText(QString::number(pDout->ScanEnable));
	ui.m_labInit->setText(QString::number(pDout->Init));
	ui.m_labQuality->setText(QString::number(pDout->Quality));

	ui.m_labOutput->setText(QString::number(pDout->Output));
	ui.m_labRelayState->setText(QString::number(pDout->RelayState));
	ui.m_labCtrlType->setText(QString::number(pDout->CtrlType));
	ui.m_labFilter->setText(QString::number(pDout->Filter));
	ui.m_labDataSource->setText(QString::number(pDout->DataSource));
}

void CDoutOperDialog::RefreshView()
{
	DOUT *pDout = nullptr;

	CFesDBR * pFes = m_pMemDB->GetFesByOccNo(m_nNodeOccNo);
	Q_ASSERT(pFes);

	if (!pFes)
	{
		return;
	}
	pDout = pFes->GetDoutByOccNo(m_nDoutOccNo);
	Q_ASSERT(pDout);

	if (!pDout)
	{
		return;
	}
	//实时信息
	ui.m_labState->setText(QString::number(pDout->State));
	ui.m_labIsdefined->setText(QString::number(pDout->IsDefined));
	ui.m_labScan->setText(QString::number(pDout->ScanEnable));
	ui.m_labInit->setText(QString::number(pDout->Init));
	ui.m_labQuality->setText(QString::number(pDout->Quality));

	ui.m_labOutput->setText(QString::number(pDout->Output));
	ui.m_labRelayState->setText(QString::number(pDout->RelayState));
	ui.m_labCtrlType->setText(QString::number(pDout->CtrlType));
	ui.m_labFilter->setText(QString::number(pDout->Filter));
	ui.m_labDataSource->setText(QString::number(pDout->DataSource));
}

void CDoutOperDialog::slot_on_BtnSelect()
{
	IO_VARIANT nVar;
	int nIndex = OPER_TYPE_SELECT;
	S_INT(&nVar, &nIndex);
	PutRTData(IDD_DOUT, m_nNodeOccNo, m_nDoutOccNo, ATT_DOUT, &nVar, nullptr, nullptr);
}

void CDoutOperDialog::slot_on_BtnPre()
{
	IO_VARIANT nVar;
	int nIndex = OPER_TYPE_PRESET;
	S_INT(&nVar, &nIndex);
	PutRTData(IDD_DOUT, m_nNodeOccNo, m_nDoutOccNo, ATT_DOUT, &nVar, nullptr, nullptr);
}

void CDoutOperDialog::slot_on_BtnExec()
{
	IO_VARIANT nVar;
	int nIndex = OPER_TYPE_EXEC;
	S_INT(&nVar, &nIndex);
	PutRTData(IDD_DOUT, m_nNodeOccNo, m_nDoutOccNo, ATT_DOUT, &nVar, nullptr, nullptr);
}

void CDoutOperDialog::slot_on_AutoExec()
{
	IO_VARIANT nVar;
	int nIndex = OPER_TYPE_DEXEC;
	S_INT(&nVar, &nIndex);
	PutRTData(IDD_DOUT, m_nNodeOccNo, m_nDoutOccNo, ATT_DOUT, &nVar, nullptr, nullptr);
}
