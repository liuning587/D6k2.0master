#include "soewgt.h"
#include <QHeaderView>
#include "breaker_module.h"
#include "analysepointtable.h"
#include "netmanager.h"
#include "breaksender.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <memory>
#include <QDateTime>
#include <QMenu>

CSoeWgt::CSoeWgt(CNetManager *pNetManager, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	if (pNetManager == nullptr)
	{
		return;
	}
	m_pNetManager = pNetManager;

	//ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setColumnWidth(0, 300);
	ui.tableWidget->setColumnWidth(1, 200);
	ui.tableWidget->setColumnWidth(2, 200);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
// 	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Slot_GetSoeEventInfo()));
// 	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_GetSoeEventNum()));
	//右击菜单
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	ui.tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	ui.tableWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_3, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	ui.tableWidget_4->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_4, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	ui.tableWidget_5->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_5, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
}

CSoeWgt::~CSoeWgt()
{
}

//处理action数据
void CSoeWgt::AnaylseActionData(SOE_ACTION_INFO *pAction)
{
	Q_ASSERT(pAction);
	if (pAction == nullptr)
	{
		return;
	}

	//1位code位   2为soe事件类型 和动作状态为
	int nDataNum = (pAction->msgLeg.GetAddr()-1-sizeof(SOE_INFO_HEADER)-2) / sizeof(SOE_ACTION_INFO::DEG_ACTION_ITEM);

	QTableWidgetItem *pItem0 = new QTableWidgetItem;
	pItem0->setText(pAction->m_Time.Dump());

	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &pActionPointTable = GetBreakerModuleApi()->GetPointTable()->GetActionEventInfo();

	QTableWidgetItem *pItem1 = new QTableWidgetItem;

	if (pActionPointTable.find(pAction->m_cSoeType) != pActionPointTable.end())
	{
		pItem1->setText(pActionPointTable.at(pAction->m_cSoeType)->m_strName);
	}
	else
	{
		pItem1->setText(QString::number(pAction->m_cSoeType));
	}
	

	QTableWidgetItem *pItem2 = new QTableWidgetItem;
	if (pAction->m_cActionStatus % 2 == 0)
	{
		pItem2->setText(QStringLiteral("返回"));
	}
	else if (pAction->m_cActionStatus % 2 != 0)
	{
		pItem2->setText(QStringLiteral("动作"));
	}	
	QTableWidgetItem *pItem3 = new QTableWidgetItem;

	QString strDefalut;

	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetAnalogInfo();


	for (int i=0; i<nDataNum; i++)
	{
		if (pPointTable.find(pAction->m_data[i].dataType.GetAddr()) != pPointTable.end())
		{
			strDefalut += "   " + pPointTable.at(pAction->m_data[i].dataType.GetAddr())->m_strName + "=" + QString::number(*(int*)pAction->m_data[i].measData ) + pPointTable.at(pAction->m_data[i].dataType.GetAddr())->m_strUnit;
		}
		
		pItem3->setText(strDefalut);
	}

	ui.tableWidget->insertRow(0);
	ui.tableWidget->setItem(0, 0, pItem0);
	ui.tableWidget->setItem(0, 1, pItem1);
	ui.tableWidget->setItem(0, 2, pItem2);
	ui.tableWidget->setItem(0, 3, pItem3);

}

void CSoeWgt::AnayseIoData(SOE_IO_INFO * pIO)
{
	const std::map<int, std::shared_ptr<CDIDOInfo> > &pDiPointTable = GetBreakerModuleApi()->GetPointTable()->GetDIInfo();

	const std::map<int, std::shared_ptr<CDIDOInfo> > &pDoPointTable = GetBreakerModuleApi()->GetPointTable()->GetDOInfo();

	QTableWidgetItem *pItem0 = new QTableWidgetItem;
	pItem0->setText(pIO->m_Time.Dump());

	QTableWidgetItem *pItem1 = new QTableWidgetItem;
	

	QTableWidgetItem *pItem2 = new QTableWidgetItem;

	if (pIO->m_CBinaryType == DI_SOE_TYPE_REAL_DI)
	{
		//实际开入
		pItem1->setText(QStringLiteral("开入"));
		if (pDiPointTable.find(pIO->m_cID) != pDiPointTable.end())
		{
			pItem2->setText(pDiPointTable.at(pIO->m_cID)->m_nName);
		}
	}
	else if (pIO->m_CBinaryType == DI_SOE_TYPE_REAL_DO)
	{
		//开出
		pItem1->setText(QStringLiteral("开出"));
		if (pDoPointTable.find(pIO->m_cID) != pDoPointTable.end())
		{
			pItem2->setText(pDoPointTable.at(pIO->m_cID)->m_nName);
		}
	}
	else if (pIO->m_CBinaryType == DI_SOE_TYPE_VIRPT)
	{
		//虚拟点
		pItem1->setText(QStringLiteral("虚拟点"));
		pItem2->setText(QString::number(pIO->m_cID));
	}
	else
	{
		pItem1->setText(QString::number(pIO->m_CBinaryType));
		pItem2->setText(QString::number(pIO->m_cID));

	}

	QTableWidgetItem *pItem3 = new QTableWidgetItem;
	if (pIO->m_cChangeType % 2 == 0)
	{
		pItem3->setText("1->0");
	}
	else if (pIO->m_cChangeType % 2 == 1)
	{
		pItem3->setText("0->1");
	}
	else
	{
		pItem3->setText(QString::number(pIO->m_cChangeType));
	}
	

	ui.tableWidget_2->insertRow(0);
	ui.tableWidget_2->setItem(0, 0, pItem0);
	ui.tableWidget_2->setItem(0, 1, pItem1);
	ui.tableWidget_2->setItem(0, 2, pItem2);
	ui.tableWidget_2->setItem(0, 3, pItem3);
}

void CSoeWgt::AnalyseAbnormalData(SOE_ID_INFO * pSoeInfo)
{
	QTableWidgetItem *pItem0 = new QTableWidgetItem;
	pItem0->setText(pSoeInfo->m_Time.Dump());

	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetAbnromalEventInfo();

	QTableWidgetItem *pItem1 = new QTableWidgetItem;
	if (pPointTable.find(pSoeInfo->m_cType) != pPointTable.end())
	{
		pItem1->setText(pPointTable.at(pSoeInfo->m_cType)->m_strName);
	}
	else
	{
		pItem1->setText(QString::number(pSoeInfo->m_cType));
	}

	ui.tableWidget_3->insertRow(0);
	ui.tableWidget_3->setItem(0, 0, pItem0);
	ui.tableWidget_3->setItem(0, 1, pItem1);
}

void CSoeWgt::AnalyseRuningData(SOE_ID_INFO * pSoeInfo)
{
	QTableWidgetItem *pItem0 = new QTableWidgetItem;
	pItem0->setText(pSoeInfo->m_Time.Dump());

	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetRunEventInfo();

	QTableWidgetItem *pItem1 = new QTableWidgetItem;

	if (pPointTable.find(pSoeInfo->m_cType) != pPointTable.end())
	{
		pItem1->setText(pPointTable.at(pSoeInfo->m_cType)->m_strName);
	}
	else
	{
		pItem1->setText(QString::number(pSoeInfo->m_cType));
	}

	ui.tableWidget_4->insertRow(0);
	ui.tableWidget_4->setItem(0, 0, pItem0);
	ui.tableWidget_4->setItem(0, 1, pItem1);
}

void CSoeWgt::AnalyseRecordData(SOE_ID_INFO * pSoeInfo)
{
	QTableWidgetItem *pItem0 = new QTableWidgetItem;
	pItem0->setText(pSoeInfo->m_Time.Dump());

	QTableWidgetItem *pItem1 = new QTableWidgetItem;
	QString strData = QString::number(pSoeInfo->m_cType) + ".cfg   " + QString::number(pSoeInfo->m_cType) + ".dat";
	pItem1->setText(strData);

	ui.tableWidget_5->insertRow(0);
	ui.tableWidget_5->setItem(0, 0, pItem0);
	ui.tableWidget_5->setItem(0, 1, pItem1);
}

//发送soe数量请求信息
void CSoeWgt::Slot_GetSoeEventNum()
{
	DEG_GET_SOE dbgSoe;
	dbgSoe.header0 = 0xAA;
	dbgSoe.header1 = 0x55;

	dbgSoe.msgLeg.SetAddr(5);
	//获取保护定值信息
	dbgSoe.type = DBG_CODE_GET_SOE;

	dbgSoe.m_SoeType.SetAddr(0xAA55);

	//m_pNetManager->GetSender()->OnSendSoeRequest(&dbgSoe);

}

void CSoeWgt::Slot_SoeUpdate(int nType)
{
	if (nType == FLAG_DEVSTATUS_SOE || nType == FLAG_DEVSTATUS_RUN || nType == FLAG_DEVSTATUS_REMOTE || nType == FLAG_DEVSTATUS_LOCKSTATUS || FLAG_DEVSTATUS_ACTSTATUS)
	{
		//有新SOE事件产生
		Slot_GetSoeEventInfo();
	}
}

//情况表格
void CSoeWgt::Slot_ClearTable()
{
	QTableWidget *pCurrentWgt = (QTableWidget*)sender()->parent()->parent();

	if (pCurrentWgt != nullptr)
	{
		pCurrentWgt->clearContents();
		pCurrentWgt->setRowCount(0);
	}
}

void CSoeWgt::Slot_ContextMenuRequest(const QPoint & point)
{
	Q_UNUSED(point);
	QMenu *pMenu = new QMenu((QTableWidget*)sender());

	QAction *pClearAct = new QAction(QStringLiteral("清空"), (QTableWidget*)sender());
	pMenu->addAction(pClearAct);

	connect(pClearAct, SIGNAL(triggered()), this, SLOT(Slot_ClearTable()));

	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}

//更新数据
void CSoeWgt::Slot_RecvNewRealTimeData(DEG_SOE_DETAIL &tSoeDetail)
{
	//数据长度，减去code字段
	int nDataLength = tSoeDetail.msgLeg.GetAddr() - 1;

	//开始数据解析
	if (m_pNetManager->GetSender()->GetSoeType() != 0xFFFF)
	{
		return;
	}

	if (tSoeDetail.SOEDATA[0] == SOE_ACTION)
	{
		/*!< 保护动作事件 */
		SOE_ACTION_INFO *pAction = (SOE_ACTION_INFO *)(&tSoeDetail);
		AnaylseActionData(pAction);
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_DIEVENT)
	{
		/*!< 开入变位事件 */
		SOE_IO_INFO *pAction = (SOE_IO_INFO *)(&tSoeDetail);
		AnayseIoData(pAction);
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_ABNORMAL)
	{
		/*!< 异常事件 */
		SOE_ID_INFO *pAction = (SOE_ID_INFO *)(&tSoeDetail);
		AnalyseAbnormalData(pAction);
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_RUN)
	{
		/*!< 运行事件 */
		SOE_ID_INFO *pAction = (SOE_ID_INFO *)(&tSoeDetail);
		AnalyseRuningData(pAction);
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_WAVE_FILE)
	{
		/*!< 录波事件 */
		SOE_ID_INFO *pAction = (SOE_ID_INFO *)(&tSoeDetail);
		AnalyseRecordData(pAction);
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_TRACE)
	{
		/*!< 调试信息 */
	}

	
}

//发送soe请求数据
void CSoeWgt::Slot_GetSoeEventInfo()
{
	DEG_GET_SOE dbgSoe;
	dbgSoe.header0 = 0xAA;
	dbgSoe.header1 = 0x55;

	dbgSoe.msgLeg.SetAddr(5);
	//获取保护定值信息
	dbgSoe.type = DBG_CODE_GET_SOE;

	dbgSoe.m_SoeType.SetAddr(0xffff);

	m_pNetManager->GetSender()->OnSendSoeRequest(&dbgSoe);

	//设置当前类型
	m_pNetManager->GetSender()->SetSoeType(0xffff);
}
