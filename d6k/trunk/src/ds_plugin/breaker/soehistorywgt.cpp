#include "soehistorywgt.h"
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
#include <QProgressBar>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QDateTime>
#include "qftp.h"

CSoeHistoryWgt::CSoeHistoryWgt(CNetManager *pNetManager, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	if (pNetManager == nullptr)
	{
		return;
	}
	//����������

	m_pNetManager = pNetManager;
	m_pFtp = new QFtp(this);
	m_pCommProcess = new QProcess(this);

	connect(m_pFtp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(Slot_DownloadProcess(qint64, qint64)));
	connect(m_pFtp, SIGNAL(done(bool)), this, SLOT(Slot_FtpDone(bool)));
	connect(m_pFtp,SIGNAL(stateChanged(int)),this,SLOT(Slot_FtpStateChange(int)));

	m_pDownLoadFile = new QFile(this);

	m_pSoeStatic = std::make_shared<SOE_EVENT_STATIC>();

	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setColumnWidth(0, 300);
	ui.tableWidget->setColumnWidth(1, 200);
	ui.tableWidget->setColumnWidth(2, 200);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_6->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_5->setSelectionBehavior(QAbstractItemView::SelectRows);

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

	ui.tableWidget_6->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_6, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	//˫����¼���ļ�
	connect(ui.tableWidget_5, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_ItemDoubleClicked(QTableWidgetItem *)));

	InitProcessBar();

	//����
	QHeaderView *headerGoods = ui.tableWidget->horizontalHeader();
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setSectionsClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), ui.tableWidget, SLOT(sortByColumn(int)));

	QHeaderView *headerGoods2 = ui.tableWidget_2->horizontalHeader();
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods2->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods2->setSortIndicatorShown(true);
	headerGoods2->setSectionsClickable(true);
	connect(headerGoods2, SIGNAL(sectionClicked(int)), ui.tableWidget_2, SLOT(sortByColumn(int)));

	QHeaderView *headerGoods3 = ui.tableWidget_3->horizontalHeader();
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods3->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods3->setSortIndicatorShown(true);
	headerGoods3->setSectionsClickable(true);
	connect(headerGoods3, SIGNAL(sectionClicked(int)), ui.tableWidget_3, SLOT(sortByColumn(int)));

	//
	QHeaderView *headerGoods4 = ui.tableWidget_4->horizontalHeader();
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods4->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods4->setSortIndicatorShown(true);
	headerGoods4->setSectionsClickable(true);
	connect(headerGoods4, SIGNAL(sectionClicked(int)), ui.tableWidget_4, SLOT(sortByColumn(int)));

	QHeaderView *headerGoods5 = ui.tableWidget_5->horizontalHeader();
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods5->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods5->setSortIndicatorShown(true);
	headerGoods5->setSectionsClickable(true);
	connect(headerGoods5, SIGNAL(sectionClicked(int)), ui.tableWidget_5, SLOT(sortByColumn(int)));

}

CSoeHistoryWgt::~CSoeHistoryWgt()
{
	m_pFtp->deleteLater();
	m_pDownLoadFile->deleteLater();
	m_pCommProcess->deleteLater();
}

//������������
void CSoeHistoryWgt::OnSendGetInfo(int nSoeType, int SoeIndex)
{
	DEG_GET_SOE dbgSoe;
	dbgSoe.header0 = 0xAA;
	dbgSoe.header1 = 0x55;

	dbgSoe.msgLeg.SetAddr(5);
	//��ȡ������ֵ��Ϣ
	dbgSoe.type = DBG_CODE_GET_SOE;

	dbgSoe.m_SoeType.SetAddr(nSoeType);
	dbgSoe.m_SoeIndex.SetAddr(SoeIndex);

	m_pNetManager->GetSender()->OnSendSoeRequest(&dbgSoe);

	m_pNetManager->GetSender()->SetSoeType(nSoeType);

}

//������
void CSoeHistoryWgt::Slot_ClearTable()
{
	QTableWidget *pCurrentWgt = (QTableWidget*)sender()->parent();

	if (pCurrentWgt != nullptr)
	{
		pCurrentWgt->clearContents();
		pCurrentWgt->setRowCount(0);
	}
}

void CSoeHistoryWgt::Slot_ContextMenuRequest(const QPoint & point)
{
	Q_UNUSED(point);
	if ((QTableWidget*)sender() == ui.tableWidget_6)
	{
		//ͳ����Ϣ
		QMenu *pMenu = new QMenu((QTableWidget*)sender());

		QAction *pClearAct = new QAction(QStringLiteral("������Ŀ"), (QTableWidget*)sender());
		pMenu->addAction(pClearAct);

		QAction *pReadAct = new QAction(QStringLiteral("��ȡ��ʷ�¼�"), (QTableWidget*)sender());
		pMenu->addAction(pReadAct);

		QAction *pSaveAct = new QAction(QStringLiteral("������ʷ�¼�"), (QTableWidget*)sender());
		pMenu->addAction(pSaveAct);


		connect(pClearAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateNumInfo()));
		connect(pReadAct, SIGNAL(triggered()), this, SLOT(Slot_BeginReadHistorySoe()));
		connect(pSaveAct, SIGNAL(triggered()), this, SLOT(SaveHistoryEvent()));

		pMenu->exec(QCursor::pos());
		pMenu->deleteLater();

	}
	else
	{
		QMenu *pMenu = new QMenu((QTableWidget*)sender());


		if ((QTableWidget*)sender() == ui.tableWidget_5)
		{
			QAction *pOpenAct = new QAction(QStringLiteral("��"), (QTableWidget*)sender());
			pMenu->addAction(pOpenAct);

			connect(pOpenAct, SIGNAL(triggered()), this, SLOT(Slot_OpenWaveFile()));

		}

		QAction *pClearAct = new QAction(QStringLiteral("���"), (QTableWidget*)sender());
		pMenu->addAction(pClearAct);

		connect(pClearAct, SIGNAL(triggered()), this, SLOT(Slot_ClearTable()));

		pMenu->exec(QCursor::pos());
		pMenu->deleteLater();

	}
}

//����������Ϣ
void CSoeHistoryWgt::Slot_UpdateNumInfo()
{
	OnSendGetInfo(0XAA55, 0);
}


void CSoeHistoryWgt::Slot_BeginReadHistorySoe()
{
	//ֹͣ��ʱ��
	GetBreakerModuleApi()->StopTimer();

	m_pSoeStatic->ClearStatic();
	m_pSoeStatic->m_nCurrentStatic = SOE_EVENT_STATIC::SOE_GET_NUMBER;
	//�����������
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);

	ui.tableWidget_2->clearContents();
	ui.tableWidget_2->setRowCount(0);

	ui.tableWidget_3->clearContents();
	ui.tableWidget_3->setRowCount(0);

	ui.tableWidget_4->clearContents();
	ui.tableWidget_4->setRowCount(0);

	ui.tableWidget_5->clearContents();
	ui.tableWidget_5->setRowCount(0);
	//�ڶ�ȡ��ʷ�¼�ʱ��Ҫ���¸�������
	OnSendGetInfo(0XAA55, 0);

}

void CSoeHistoryWgt::Slot_ItemDoubleClicked(QTableWidgetItem *item)
{
	m_lstFilelst.clear();


	m_lstFilelst.append(ui.tableWidget_5->item(item->row(),1)->text().split("   "));

	if (m_lstFilelst.isEmpty())
	{
		return;
	}

	GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("׼������FTP������:%1").arg(m_strFtpIp).toLocal8Bit().data(), 1);

	if (m_pFtp->state() == QFtp::Unconnected)
	{
		m_pFtp->connectToHost(m_strFtpIp, 21);
		m_pFtp->login(m_strFtpUserName, m_strFtpPassWd);
		m_pFtp->setTransferMode(QFtp::Active);
	}
	else
	{
		FtpDownLoadFile(m_lstFilelst.first());
	}
	
}

void CSoeHistoryWgt::Slot_DownloadProcess(qint64 bytesReceived, qint64 bytesTotal)
{
	if (bytesReceived == bytesTotal)
	{
		if (bytesTotal == 0)
		{
			return;
		}

		m_pDownLoadFile->close();

		if (m_lstFilelst.isEmpty())
		{
			return;
		}
		QString strFilename = m_lstFilelst.first().split(".").first();

		m_lstFilelst.removeFirst();

		if (m_lstFilelst.count() == 0)
		{
			GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("FTP�ļ����سɹ�").toLocal8Bit().data(), 1);

			//ȫ���������
			StartWaveExe(qApp->applicationDirPath() + LB_FILE_DIR + strFilename + ".cfg");
			return;
		}

		FtpDownLoadFile(m_lstFilelst.first());

	}
}


void CSoeHistoryWgt::Slot_FtpDone(bool bFlag)
{
	if (bFlag)
	{
		QMessageBox::warning(this, tr("Warning"), m_pFtp->errorString());
	}
	else
	{
	}

}

//������ʷ�¼�
void CSoeHistoryWgt::SaveHistoryEvent()
{
	QString fileName = qApp->applicationDirPath() + LB_HSITORY_DIR + QDateTime::currentDateTime().toString("yyyyMmddhhmmss") + ".csv";

	//��.csv�ļ�
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << QStringLiteral("�ļ�д��ʧ��");
		return;
	}

	QTextStream out(&file);
	//��ȡ����

	//1111111111
	out << QStringLiteral("SOEʱ��,") << QStringLiteral("�¼�����,") << QStringLiteral("����״̬,") << QStringLiteral("���ϲ�ֵ,")  << "\n";//��ͷ          
																 //��ȡ�������
	int row = ui.tableWidget->rowCount();//���������

	for (int i = 0; i < row; i++)
	{
		for (int col = 0; col < 4; col++)
		{
			QString string = ui.tableWidget->item(i, col)->text();
			out << string.remove("\n").remove("\r") << ",";// д���ļ�
		}
		out << "\n";
	}

	//2222222222
	out << QStringLiteral("SOEʱ��,") << QStringLiteral("ң������,") << QStringLiteral("��λ����,") << QStringLiteral("��λ����,") << "\n";//��ͷ          
																															 //��ȡ�������
	int row2 = ui.tableWidget_2->rowCount();//���������

	for (int i = 0; i < row2; i++)
	{
		for (int col = 0; col < 4; col++)
		{
			QString string = ui.tableWidget_2->item(i, col)->text();
			out << string.remove("\n").remove("\r") << ",";// д���ļ�
		}
		out << "\n";
	}

	//3333333333333
	out << QStringLiteral("SOEʱ��,") << QStringLiteral("�쳣��Ϣ,")<< "\n";//��ͷ          
																															 //��ȡ�������
	int row3 = ui.tableWidget_3->rowCount();//���������

	for (int i = 0; i < row3; i++)
	{
		for (int col = 0; col < 2; col++)
		{
			QString string = ui.tableWidget_3->item(i, col)->text();
			out << string.remove("\n").remove("\r") << ",";// д���ļ�
		}
		out << "\n";
	}

	//4444444
	out << QStringLiteral("SOEʱ��,") << QStringLiteral("������Ϣ,") << "\n";//��ͷ          
																	   //��ȡ�������
	int row4 = ui.tableWidget_4->rowCount();//���������

	for (int i = 0; i < row4; i++)
	{
		for (int col = 0; col < 2; col++)
		{
			QString string = ui.tableWidget_4->item(i, col)->text();
			out << string.remove("\n").remove("\r") << ",";// д���ļ�
		}
		out << "\n";
	}

	//555555555
	out << QStringLiteral("SOEʱ��,") << QStringLiteral("¼���ļ���,") << "\n";//��ͷ          
																	   //��ȡ�������
	int row5 = ui.tableWidget_5->rowCount();//���������

	for (int i = 0; i < row5; i++)
	{
		for (int col = 0; col < 2; col++)
		{
			QString string = ui.tableWidget_5->item(i, col)->text();
			out << string.remove("\n").remove("\r") << ",";// д���ļ�
		}
		out << "\n";
	}

	QMessageBox::information(this, QStringLiteral("�������ݳɹ�"), QStringLiteral("��Ϣ�ѱ�����%1��").arg(fileName), QStringLiteral("ȷ��"));
	file.close();
}

void CSoeHistoryWgt::Slot_OpenWaveFile()
{
	if (ui.tableWidget_5->currentRow() > -1)
	{
		Slot_ItemDoubleClicked(ui.tableWidget_5->currentItem());
	}
	else
	{
		QMessageBox::warning(this,QStringLiteral("����"),QStringLiteral("��ѡ��һ�н��в���!"));
	}
}

void CSoeHistoryWgt::Slot_FtpStateChange(int nIndex)
{

	if (nIndex == QFtp::Unconnected)
	{
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ǰFTP״̬:Unconnected").toLocal8Bit().data(),1);
	}
	else if (nIndex == QFtp::HostLookup)
	{
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ǰFTP״̬:HostLookup").toLocal8Bit().data(), 1);
	}
	else if (nIndex == QFtp::Connecting)
	{
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ǰFTP״̬:Connecting").toLocal8Bit().data(), 1);
	}
	else if (nIndex == QFtp::Connected)
	{
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ǰFTP״̬:Connected").toLocal8Bit().data(), 1);
	}
	else if (nIndex == QFtp::LoggedIn)
	{
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ǰFTP״̬:LoggedIn").toLocal8Bit().data(), 1);

		FtpDownLoadFile(m_lstFilelst.first());
	}
	else if (nIndex == QFtp::Closing)
	{
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ǰFTP״̬:Closing").toLocal8Bit().data(), 1);
	}

}

//����action����
void CSoeHistoryWgt::AnaylseActionData(SOE_ACTION_INFO *pAction)
{
	Q_ASSERT(pAction);
	if (pAction == nullptr)
	{
		return;
	}

	//1λcodeλ   2Ϊsoe�¼����� �Ͷ���״̬Ϊ
	int nDataNum = (pAction->msgLeg.GetAddr() - 1 - sizeof(SOE_INFO_HEADER) - 2) / sizeof(SOE_ACTION_INFO::DEG_ACTION_ITEM);

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
	if (pAction->m_cActionStatus %2 == 0)
	{
		pItem2->setText(QStringLiteral("����"));
	}
	else if (pAction->m_cActionStatus % 2 != 0)
	{
		pItem2->setText(QStringLiteral("����"));
	}
	

	QTableWidgetItem *pItem3 = new QTableWidgetItem;

	QString strDefalut;

	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetAnalogInfo();


	for (int i = 0; i < nDataNum; i++)
	{
		if (pPointTable.find(pAction->m_data[i].dataType.GetAddr()) != pPointTable.end())
		{
			strDefalut += "   " + pPointTable.at(pAction->m_data[i].dataType.GetAddr())->m_strName + "=" + QString::number(*(int*)pAction->m_data[i].measData) + pPointTable.at(pAction->m_data[i].dataType.GetAddr())->m_strUnit;;
		}

		pItem3->setText(strDefalut);
	}

	ui.tableWidget->insertRow(0);
	ui.tableWidget->setItem(0, 0, pItem0);
	ui.tableWidget->setItem(0, 1, pItem1);
	ui.tableWidget->setItem(0, 2, pItem2);
	ui.tableWidget->setItem(0, 3, pItem3);

}

void CSoeHistoryWgt::AnayseIoData(SOE_IO_INFO * pIO)
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
		//ʵ�ʿ���
		pItem1->setText(QStringLiteral("����"));
		if (pDiPointTable.find(pIO->m_cID) != pDiPointTable.end())
		{
			pItem2->setText(pDiPointTable.at(pIO->m_cID)->m_nName);
		}
	}
	else if (pIO->m_CBinaryType == DI_SOE_TYPE_REAL_DO)
	{
		//����
		pItem1->setText(QStringLiteral("����"));
		if (pDoPointTable.find(pIO->m_cID) != pDoPointTable.end())
		{
			pItem2->setText(pDoPointTable.at(pIO->m_cID)->m_nName);
		}
	}
	else if (pIO->m_CBinaryType == DI_SOE_TYPE_VIRPT)
	{
		//�����
		pItem1->setText(QStringLiteral("�����"));

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

void CSoeHistoryWgt::AnalyseAbnormalData(SOE_ID_INFO * pSoeInfo)
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

void CSoeHistoryWgt::AnalyseRuningData(SOE_ID_INFO * pSoeInfo)
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

void CSoeHistoryWgt::AnalyseRecordData(SOE_ID_INFO * pSoeInfo)
{
	QTableWidgetItem *pItem0 = new QTableWidgetItem;
	pItem0->setText(pSoeInfo->m_Time.Dump());
	pItem0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QTableWidgetItem *pItem1 = new QTableWidgetItem;
	char buff[128];
	sprintf(buff, "%.2d", pSoeInfo->m_cType);

	QString strData = QString(buff) + ".cfg   " + QString(buff) + ".dat";
	pItem1->setText(strData);
	pItem1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	ui.tableWidget_5->insertRow(0);
	ui.tableWidget_5->setItem(0, 0, pItem0);
	ui.tableWidget_5->setItem(0, 1, pItem1);
}



void CSoeHistoryWgt::AnalyseNumsData(SOE_NUM_INFO *pNums)
{
	//����������Ϣ
	ui.tableWidget_6->item(0, 1)->setText(QString::number(pNums->m_actionLeg.GetAddr()));
	ui.tableWidget_6->item(1, 1)->setText(QString::number(pNums->m_diLeg.GetAddr()));
	ui.tableWidget_6->item(2, 1)->setText(QString::number(pNums->m_abnormalleg.GetAddr()));
	ui.tableWidget_6->item(3, 1)->setText(QString::number(pNums->m_runleg.GetAddr()));
	ui.tableWidget_6->item(4, 1)->setText(QString::number(pNums->m_waveleg.GetAddr()));
	ui.tableWidget_6->item(5, 1)->setText(QString::number(pNums->m_traceleg.GetAddr()));
}

void CSoeHistoryWgt::OnSendRequestData()
{
	//action
	if (m_pSoeStatic->m_nActionTotalNum > 0)
	{
		if (m_pSoeStatic->m_nActionCurrentNum != m_pSoeStatic->m_nActionTotalNum)
		{
			//��������
			OnSendGetInfo(SOE_ACTION, m_pSoeStatic->m_nActionCurrentNum);
			//m_pSoeStatic->m_nActionCurrentNum = m_pSoeStatic->m_nActionCurrentNum + 1;
			return;
		}
	}

	//di
	if (m_pSoeStatic->m_nDiTotalNum > 0)
	{
		if (m_pSoeStatic->m_nDiCurrentNum != m_pSoeStatic->m_nDiTotalNum)
		{
			//��������
			OnSendGetInfo(SOE_DIEVENT, m_pSoeStatic->m_nDiCurrentNum);
			//m_pSoeStatic->m_nDiCurrentNum = m_pSoeStatic->m_nDiCurrentNum + 1;
			return;
		}
	}

	//abnormal
	if (m_pSoeStatic->m_nAbnormalTotalNum > 0)
	{
		if (m_pSoeStatic->m_nAbnoramlCurrentNum != m_pSoeStatic->m_nAbnormalTotalNum)
		{
			//��������
			OnSendGetInfo(SOE_ABNORMAL, m_pSoeStatic->m_nAbnoramlCurrentNum);
			//m_pSoeStatic->m_nAbnoramlCurrentNum = m_pSoeStatic->m_nAbnoramlCurrentNum + 1;
			return;
		}
	}

	//run
	if (m_pSoeStatic->m_nRunTotalNum > 0)
	{
		if (m_pSoeStatic->m_nRunCurrentNum != m_pSoeStatic->m_nRunTotalNum)
		{
			//��������
			OnSendGetInfo(SOE_RUN, m_pSoeStatic->m_nRunCurrentNum);
			//m_pSoeStatic->m_nRunCurrentNum = m_pSoeStatic->m_nRunCurrentNum + 1;
			return;
		}
	}

	//wave
	if (m_pSoeStatic->m_nWaveTotaolNum > 0)
	{
		if (m_pSoeStatic->m_nVaveCurrentNum != m_pSoeStatic->m_nWaveTotaolNum)
		{
			//��������
			OnSendGetInfo(SOE_WAVE_FILE, m_pSoeStatic->m_nVaveCurrentNum);
			//m_pSoeStatic->m_nVaveCurrentNum = m_pSoeStatic->m_nVaveCurrentNum + 1;
			return;
		}
	}

}

void CSoeHistoryWgt::InitProcessBar()
{
	m_pActionBar = new QProgressBar;
	m_pDiBar = new QProgressBar;
	m_pAbnormalBar = new QProgressBar;
	m_pRunBar = new QProgressBar;
	m_pPWaveBar = new QProgressBar;
	m_pDebugBar = new QProgressBar;

	ui.tableWidget_6->setCellWidget(0, 2, m_pActionBar);
	ui.tableWidget_6->setCellWidget(1, 2, m_pDiBar);
	ui.tableWidget_6->setCellWidget(2, 2, m_pAbnormalBar);
	ui.tableWidget_6->setCellWidget(3, 2, m_pRunBar);
	ui.tableWidget_6->setCellWidget(4, 2, m_pPWaveBar);
	ui.tableWidget_6->setCellWidget(5, 2, m_pDebugBar);

	ui.tableWidget_6->setColumnHidden(2, true);

}

void CSoeHistoryWgt::UpdateProcess()
{
	ui.tableWidget_6->setColumnHidden(2, false);

	m_pActionBar->setMaximum(m_pSoeStatic->m_nActionTotalNum+1);
	m_pActionBar->setMinimum(0);
	m_pActionBar->setValue(m_pSoeStatic->m_nActionCurrentNum+1);

	m_pDiBar->setMaximum(m_pSoeStatic->m_nDiTotalNum+1);
	m_pDiBar->setMinimum(0);
	m_pDiBar->setValue(m_pSoeStatic->m_nDiCurrentNum+1);


	m_pAbnormalBar->setMaximum(m_pSoeStatic->m_nAbnormalTotalNum+1);
	m_pAbnormalBar->setMinimum(0);
	m_pAbnormalBar->setValue(m_pSoeStatic->m_nAbnoramlCurrentNum+1);


	m_pRunBar->setMaximum(m_pSoeStatic->m_nRunTotalNum+1);
	m_pRunBar->setMinimum(0);
	m_pRunBar->setValue(m_pSoeStatic->m_nRunCurrentNum+1);


	m_pPWaveBar->setMaximum(m_pSoeStatic->m_nWaveTotaolNum+1);
	m_pPWaveBar->setMinimum(0);
	m_pPWaveBar->setValue(m_pSoeStatic->m_nVaveCurrentNum+1);


	m_pDebugBar->setMaximum(10);
	m_pDebugBar->setMinimum(0);
	m_pDebugBar->setValue(10);

	if (m_pSoeStatic->m_nActionTotalNum == m_pSoeStatic->m_nActionCurrentNum && m_pSoeStatic->m_nDiTotalNum == m_pSoeStatic->m_nDiCurrentNum && m_pSoeStatic->m_nAbnormalTotalNum == m_pSoeStatic->m_nAbnoramlCurrentNum && m_pSoeStatic->m_nRunTotalNum == m_pSoeStatic->m_nRunCurrentNum  && m_pSoeStatic->m_nWaveTotaolNum == m_pSoeStatic->m_nVaveCurrentNum)
	{
		GetBreakerModuleApi()->StartTimer();
	}
}

void CSoeHistoryWgt::SetFtpIp(const QString &strFtpIp)
{
	m_strFtpIp = strFtpIp;
}

void CSoeHistoryWgt::SetFtpUserName(const QString &strFtpUserName)
{
	m_strFtpUserName = strFtpUserName;
}

void CSoeHistoryWgt::SetFtpPasswd(const QString &strFtpPasswd)
{
	m_strFtpPassWd = strFtpPasswd;
}



void CSoeHistoryWgt::SetFtpDir(const QString &strFtpDIr)
{
	m_strDir = strFtpDIr;
}
	
void CSoeHistoryWgt::FtpDownLoadFile(const QString &strFilename)
{

	if (strFilename.isEmpty())
	{
		return;
	}
	//�����ļ�,Ȼ���



	QDir directory;
	directory.setPath(qApp->applicationDirPath() + LB_FILE_DIR);
	if (!directory.exists())
	{
		directory.mkdir(qApp->applicationDirPath() + LB_FILE_DIR);
	};

	//QFile *pDownFile = new QFile(qApp->applicationDirPath() + LB_FILE_DIR + m_lstFilelst.first(), this);
	if (m_pDownLoadFile->isOpen())
	{
		m_pDownLoadFile->close();
	}
	
	m_pDownLoadFile->setFileName(qApp->applicationDirPath() + LB_FILE_DIR + m_lstFilelst.first());

	if (m_pDownLoadFile->open(QIODevice::WriteOnly))
	{
		//��ʼ����
		GetBreakerModuleApi()->WriteRunLog(tr("Breaker").toLocal8Bit().data(), QStringLiteral("��ʼ����FTP�������ļ�:%1").arg(m_strDir + strFilename).toLocal8Bit().data(), 1);

		m_pFtp->get(m_strDir + strFilename, m_pDownLoadFile);
	}

}

void CSoeHistoryWgt::StartWaveExe(const QString &strFilename)
{
	//����¼�����
	QString strRun = qApp->applicationDirPath() + LB_EXEC_FILE;

	QFile tRunFile(strRun);
	if (!tRunFile.exists())
	{
		QMessageBox::warning(0, tr("Warning"), tr("can not find :%1").arg(strRun));
		return;
	}

	m_pCommProcess->setWorkingDirectory(qApp->applicationDirPath() + LB_EXEC_DIR);
	m_pCommProcess->start(strRun, QStringList(strFilename));

}

//��������
void CSoeHistoryWgt::Slot_RecvNewRealTimeData(DEG_SOE_DETAIL tSoeDetail)
{
	//���ݳ��ȣ���ȥcode�ֶ�
	int nDataLength = tSoeDetail.msgLeg.GetAddr() - 1;

	//��ʼ���ݽ���
	if (m_pNetManager->GetSender()->GetSoeType() == 0xFFFF)
	{
		//ʵʱ��Ϣ  ��������
		return;
	}

	if (m_pNetManager->GetSender()->GetSoeType() == 0XAA55)
	{
		//��Ŀ��Ϣ
		SOE_NUM_INFO *pNums = (SOE_NUM_INFO*)(&tSoeDetail);
		AnalyseNumsData(pNums);

		if (m_pSoeStatic->m_nCurrentStatic == SOE_EVENT_STATIC::SOE_GET_NUMBER)
		{
			//ͳ�Ƹ���
			//action
			m_pSoeStatic->m_nActionTotalNum = pNums->m_actionLeg.GetAddr();
			//di
			m_pSoeStatic->m_nDiTotalNum = pNums->m_diLeg.GetAddr();
			//abnormal
			m_pSoeStatic->m_nAbnormalTotalNum = pNums->m_abnormalleg.GetAddr();
			//run
			m_pSoeStatic->m_nRunTotalNum = pNums->m_runleg.GetAddr();
			//wave
			m_pSoeStatic->m_nWaveTotaolNum = pNums->m_waveleg.GetAddr();
			//trace
			m_pSoeStatic->m_nTraceTotalNum = pNums->m_traceleg.GetAddr();

			OnSendRequestData();
		}
		return;
	}

	if (tSoeDetail.SOEDATA[0] == SOE_ACTION)
	{
		/*!< ���������¼� */

		SOE_ACTION_INFO *pAction = (SOE_ACTION_INFO *)(&tSoeDetail);
		AnaylseActionData(pAction);

		if (m_pSoeStatic->m_nCurrentStatic == SOE_EVENT_STATIC::SOE_GET_NUMBER)
		{
			m_pSoeStatic->m_nActionCurrentNum = m_pSoeStatic->m_nActionCurrentNum + 1;
			OnSendRequestData();
		}
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_DIEVENT)
	{
		/*!< �����λ�¼� */
		SOE_IO_INFO *pAction = (SOE_IO_INFO *)(&tSoeDetail);
		AnayseIoData(pAction);

		if (m_pSoeStatic->m_nCurrentStatic == SOE_EVENT_STATIC::SOE_GET_NUMBER)
		{
			m_pSoeStatic->m_nDiCurrentNum = m_pSoeStatic->m_nDiCurrentNum + 1;
			OnSendRequestData();
		}
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_ABNORMAL)
	{
		/*!< �쳣�¼� */
		SOE_ID_INFO *pAction = (SOE_ID_INFO *)(&tSoeDetail);
		AnalyseAbnormalData(pAction);

		if (m_pSoeStatic->m_nCurrentStatic == SOE_EVENT_STATIC::SOE_GET_NUMBER)
		{
			m_pSoeStatic->m_nAbnoramlCurrentNum = m_pSoeStatic->m_nAbnoramlCurrentNum + 1;
			OnSendRequestData();
		}
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_RUN)
	{
		/*!< �����¼� */
		SOE_ID_INFO *pAction = (SOE_ID_INFO *)(&tSoeDetail);
		AnalyseRuningData(pAction);

		if (m_pSoeStatic->m_nCurrentStatic == SOE_EVENT_STATIC::SOE_GET_NUMBER)
		{
			m_pSoeStatic->m_nRunCurrentNum = m_pSoeStatic->m_nRunCurrentNum + 1;
			OnSendRequestData();
		}
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_WAVE_FILE)
	{
		/*!< ¼���¼� */
		SOE_ID_INFO *pAction = (SOE_ID_INFO *)(&tSoeDetail);
		AnalyseRecordData(pAction);

		if (m_pSoeStatic->m_nCurrentStatic == SOE_EVENT_STATIC::SOE_GET_NUMBER)
		{
			m_pSoeStatic->m_nVaveCurrentNum = m_pSoeStatic->m_nVaveCurrentNum + 1;
			OnSendRequestData();
		}
	}
	else if (tSoeDetail.SOEDATA[0] == SOE_TRACE)
	{
		/*!< ������Ϣ */
	}

	UpdateProcess();
}
