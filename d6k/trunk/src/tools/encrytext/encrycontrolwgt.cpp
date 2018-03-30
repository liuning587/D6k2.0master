#include "encrycontrolwgt.h"
#include "ui_encrycontrolwgt.h"
#include "./include/TZhuZhanDll.h"
#include "apdurecver.h"
#include "apdusender.h"
#include "logcache.h"
#include "XAHost.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

CEncryControlWgt::CEncryControlWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CEncryControlWgt)
{
    ui->setupUi(this);

	//服务度接收主站数据
	m_pRecverServer = new CApduRecver(this);
	//客户端发送数据到装置
	m_pSenderClient = new CApduSender(this);
	//
	m_pRecverServer->SetSender(m_pSenderClient);

	m_pTimerLog = new QTimer(this);

    resize(1200,800);
    ui->tableWidget->setColumnWidth(0,300);
	//
	m_pTimerLog->setInterval(500);
	connect(m_pTimerLog, SIGNAL(timeout()), this, SLOT(Slot_UpDateLog()));
	m_pTimerLog->start();
}

CEncryControlWgt::~CEncryControlWgt()
{
    delete ui;
}

//写日志
void CEncryControlWgt::WriteLog(const QString &strLog)
{
    QTableWidgetItem *pNewItemTime = new QTableWidgetItem;
    pNewItemTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz"));

    QTableWidgetItem *pNewItemLog = new QTableWidgetItem;
    pNewItemLog->setText(strLog);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,pNewItemTime);
    ui->tableWidget->setItem(0,1,pNewItemLog);
}

void CEncryControlWgt::Slot_UpDateLog()
{
	//写日志
	for (auto item : CLogCache::InstanceCache()->GetLogInfo())
	{
		WriteLog(item);
	}

	CLogCache::InstanceCache()->ClearLog();

}

#include "./include/TZhuZhanDll.h"
#include <QApplication>
void CEncryControlWgt::on_pushButton_clicked()
{
    int nTimeOut = 2000;
    int nResult = CXAHost::GetInstance()->ConnectHSM(const_cast<char*>(ui->lineEdit->text().toStdString().c_str()),ui->lineEdit_2->text().toInt(),nTimeOut);

    if (nResult <= 0)
    {
        WriteLog(tr("连接加密机失败,错误码:%1").arg(nResult));
    }
	else
	{
		WriteLog(tr("连接加密机成功,连接句柄:%1").arg(nResult));
	}

	int nCerLength = 0;
	char arrCer[1024 * 2];
	QFile tCerFile(qApp->applicationDirPath() + "/F30114301001201707280002.cer");

	if (tCerFile.open(QFile::ReadOnly))
	{
		QByteArray tData = tCerFile.readAll();
		strncpy(arrCer, tData.data(), tData.length());

		int nLength = tData.length();

		tCerFile.close();
	}
	else
	{
		QMessageBox::warning(0, tr("告警"), tr("读取证书文件失败"));
	}
	int nResluttt = ::ImportPriKey(nResult, arrCer);

	m_pSenderClient->SetSenderIp(ui->lineEdit_3->text());
	m_pSenderClient->SetSenderPort(ui->lineEdit_4->text().toInt());

	m_pRecverServer->StartTcpServer();

	//开始连接装置
	m_pSenderClient->ConnectToDevice();
	
}
