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
int CSoeWgt::AnaylseActionData(SOE_ACTION_INFO *pAction)
{
	Q_ASSERT(pAction);
	if (pAction == nullptr)
	{
		return 0;
	}

	//最多10个故障值
	int nDataNum = 10;

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
		if (pAction->m_data[i].dataType.GetAddr() == 0xffff)
		{
			//最后一个侧值
			nDataNum = i+1;
			break;
		}
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

	return nDataNum * sizeof(SOE_ACTION_INFO::DEG_ACTION_ITEM) + sizeof(CPTimeMs) + 1 + 2; //1为类型   2为soe类型and动作状态
}

void CSoeWgt::AnayseIoData(SOE_IO_INFO * pIO)
{
	const std::map<int, std::shared_ptr<CDIDOInfo> > &pDiPointTable = GetBreakerModuleApi()->GetPointTable()->GetDIInfo();

	const std::map<int, std::shared_ptr<CDIDOInfo> > &pDoPointTable = GetBreakerModuleApi()->GetPointTable()->GetDOInfo();

	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &pSoftPointTable = GetBreakerModuleApi()->GetPointTable()->GetSoftInfo();


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

		for (auto item : pSoftPointTable)
		{
			if (item.second->m_nOwnId == pIO->m_cID)
			{
				pItem2->setText(item.second->m_strName);
				break;
			}
			else
			{
				pItem2->setText(QString::number(pIO->m_cID));
			}
		}
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
	QTableWidget *pCurrentWgt = (QTableWidget*)sender()->parent();

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

#include <QDebug>
//更新数据
void CSoeWgt::Slot_RecvNewRealTimeData(DEG_SOE_DETAIL tSoeDetail)
{
	//数据长度，减去code字段
	int nDataLength = tSoeDetail.msgLeg.GetAddr() - 1;

	//开始数据解析
	if (m_pNetManager->GetSender()->GetSoeType() != 0xFFFF)
	{
		return;
	}

	//位移计算
	int nOffSet = 0;
	char *pStart = tSoeDetail.SOEDATA;

	while (nOffSet <nDataLength)
	{
		pStart = tSoeDetail.SOEDATA + nOffSet;

		if (pStart[0] == SOE_ACTION)
		{
			/*!< 保护动作事件 */
			SOE_ACTION_INFO *pAction = (SOE_ACTION_INFO *)(pStart - sizeof(DBG_HEADER));
			nOffSet += AnaylseActionData(pAction);
		}
		else if (pStart[0] == SOE_DIEVENT)
		{
			/*!< 开入变位事件 */
			SOE_IO_INFO *pAction = (SOE_IO_INFO *)(pStart - sizeof(DBG_HEADER));
			AnayseIoData(pAction);
			nOffSet += sizeof(CPTimeMs) + 1 + 3;  //1soe类型  3实际的三个字段
		}
		else if (pStart[0] == SOE_ABNORMAL)
		{
			/*!< 异常事件 */
			SOE_ID_INFO *pAction = (SOE_ID_INFO *)(pStart - sizeof(DBG_HEADER));
			AnalyseAbnormalData(pAction);
			nOffSet += sizeof(CPTimeMs) + 1 + 1;  //1soe类型  1实际的1个字段

		}
		else if (pStart[0] == SOE_RUN)
		{
			/*!< 运行事件 */
			SOE_ID_INFO *pAction = (SOE_ID_INFO *)(pStart - sizeof(DBG_HEADER));
			AnalyseRuningData(pAction);
			nOffSet += sizeof(CPTimeMs) + 1 + 1;  //1soe类型  1实际的1个字段

		}
		else if (pStart[0] == SOE_WAVE_FILE)
		{
			/*!< 录波事件 */
			SOE_ID_INFO *pAction = (SOE_ID_INFO *)(pStart - sizeof(DBG_HEADER));
			AnalyseRecordData(pAction);
			nOffSet += sizeof(CPTimeMs) + 1 + 1;  //1soe类型  1实际的1个字段
		}
		else if (pStart[0] == SOE_TRACE)
		{
			/*!< 调试信息 */
			SOE_ID_INFO *pAction = (SOE_ID_INFO *)(pStart - sizeof(DBG_HEADER));
			nOffSet += sizeof(CPTimeMs) + 1 + 1;  //1soe类型  1实际的1个字段

		}
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
