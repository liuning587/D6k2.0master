#include "filetranswgt.h"
#include "commthread.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include "ftu_module.h"
#include "infoconfigwgt.h"
#include "devicestudio/main_module.h"


QString g_FileName = "";


CFileTransWgt::CFileTransWgt(CCommThread *pCommunicate, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pCommuncate = pCommunicate;
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_RefreshItems()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_DownLoadItems()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(Slot_upLoadItems()));

	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(Slot_GetPointTable()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(Slot_DownLoadPointTable()));


	connect(ui.treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int )),this,SLOT(Slot_ItemDoubleClicke(QTreeWidgetItem *, int)));

	connect(m_pCommuncate,SIGNAL(Signal_FIleCatalogINfo(QList<Catalog_Info>&)),this,SLOT(Slot_upDataITems(QList<Catalog_Info>&)));

	ui.treeWidget->header()->setSortIndicator(0, Qt::AscendingOrder);
	ui.treeWidget->header()->setSortIndicatorShown(true);
	ui.treeWidget->header()->setSectionsClickable(true);

	//ui.treeWidget->setColumnCount(2);
	QHeaderView *head = ui.treeWidget->header();

	head->setSectionResizeMode(QHeaderView::Stretch);
	m_pTimer = new QTimer(this);
	m_pTimer->setInterval(2000);

	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(SLot_TimeOut()));

	//Slot_UpdateConform(0);
}

CFileTransWgt::~CFileTransWgt()
{
}

//获取点表
void CFileTransWgt::Slot_GetPointTable()
{

	QString fileName = qApp->applicationDirPath() + "/ini/d225/remotetable/" + GetFtuModel()->GetConfigwgt()->GetPointTable() + "_prot_list.ini";

	QString strTempName = fileName;

	m_strFilePath = strTempName.remove(strTempName.split("/").last());

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, GetFtuModel()->GetConfigwgt()->GetRootDir().toStdString().c_str());    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tReadAction.m_strFileName = "prot_list.ini";    //文件名称

		tReadAction.m_nOperatorType = 1;

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 9; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		m_pCommuncate->SendCatalogRequest(tCatalogRequest);

		g_FileName = fileName;
	}
}
//下装点表
void CFileTransWgt::Slot_DownLoadPointTable()
{

	QString fileName = qApp->applicationDirPath() + "/ini/d225/remotetable/" + GetFtuModel()->GetConfigwgt()->GetPointTable() + "_prot_list.ini";

	QString tFileOwnName = fileName.split("/").last();

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, "");    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tReadAction.m_strFileName = "/prog/prot_list.ini";    //文件名称

		tReadAction.m_nOperatorType = 9;    //只召唤目录

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 0; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		//m_pCommuncate->SendCatalogRequest(tCatalogRequest);
		//Slot_CheckDeviceTable();

		FILE_ATTR_INFO tWriteAction;

		tWriteAction.asduAddr.SetAddr(1);    //装置地址
		tWriteAction.cot.SetCot(COT_ACT);   //激活

		tWriteAction.type = D_FILE_TRANSPORT;   //文件传输

		tWriteAction.m_nOperatorType = 7;    //写目录

		tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tWriteAction.m_strFileName = GetFtuModel()->GetConfigwgt()->GetRootDir() + "/prog/prot_list.ini";    //文件名称

		tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //目录长度

		QFile tFile(fileName);

		if (!tFile.open(QFile::ReadOnly))
		{
			QMessageBox::warning(this, tr("Warning"), tr("file open file error!"));
			return;
		}

		tWriteAction.m_FileSize = tFile.size();   //文件大小

		tWriteAction.m_btArrayData = tFile.readAll();   //发送数据


		m_pCommuncate->SendWriteActionRequest(tWriteAction);
	}
}



void CFileTransWgt::SendUpdateFile(const QString & fileName, const QString & tDir)
{


	QString tFileOwnName = fileName.split("/").last();

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tReadAction.m_strFileName = tDir + "/" + tFileOwnName;    //文件名称

		tReadAction.m_nOperatorType = 9;    //只召唤目录

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 0; //文件

		tCatalogRequest.m_fileInfo = tReadAction;


		//m_pCommuncate->SendCatalogRequest(tCatalogRequest);
		//Slot_CheckDeviceTable();

		FILE_ATTR_INFO tWriteAction;

		tWriteAction.asduAddr.SetAddr(1);    //装置地址
		tWriteAction.cot.SetCot(COT_ACT);   //激活

		tWriteAction.type = D_FILE_TRANSPORT;   //文件传输

		tWriteAction.m_nOperatorType = 7;    //写目录

		tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		tWriteAction.m_strFileName = tDir + "/" + tFileOwnName; ;    //文件名称

		tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //目录长度

		QFile tFile(fileName);

		if (!tFile.open(QFile::ReadOnly))
		{
			QMessageBox::warning(this, tr("Warning"), tr("file open file error!"));
			return;
		}

		tWriteAction.m_FileSize = tFile.size();   //文件大小

		tWriteAction.m_btArrayData = tFile.readAll();   //发送数据


		m_pCommuncate->SendWriteActionRequest(tWriteAction);
	}

}
void CFileTransWgt::SetUpdateFile()
{
	QDir tDir(qApp->applicationDirPath() + "/ini/d225/updatefile/");

	QString strPath = qApp->applicationDirPath() + "/ini/d225/updatefile/";

	m_strUpdateFile = QFileDialog::getOpenFileName(NULL, tr("Open File"), strPath);
	if (!m_strUpdateFile.isEmpty())
	{
		ASDU211_UPDATE pUpateInfo;
		pUpateInfo.asduAddr.SetAddr(GetFtuModel()->GetConfigwgt()->GetDeviceAddr());
		pUpateInfo.m_qds.IV = 1;

		GetFtuModel()->GetCommThread()->SendUpdateRequest(pUpateInfo);

		QByteArray byDestr = tr("Send Update Request").toLocal8Bit();

		GetFtuModel()->GetMainModule()->LogString(GetFtuModel()->GetDeviceName().toLocal8Bit().data(), byDestr.data(), 1);

	}

}
#include <QLabel>
#include <QLineEdit>
void CFileTransWgt::Slot_RefreshItems()
{
	/*
	QLabel *pLabelCattalog = new QLabel;
	pLabelCattalog->setText(tr("目录"));
	QLineEdit *pLineEdCatalog = new QLineEdit;
	QPushButton *pPushBtn = new QPushButton;
	pPushBtn->setText(tr("确定"));

	QDialog *pCatalogDlg = new QDialog;
	QHBoxLayout *playout = new QHBoxLayout(pCatalogDlg);
	playout->addWidget(pLabelCattalog);
	playout->addWidget(pLineEdCatalog);
	playout->addWidget(pPushBtn);
	pCatalogDlg->resize(300, 60);

	connect(pPushBtn,SIGNAL(clicked()),pCatalogDlg,SLOT(accept()));

	QString strCatalog;
	if (pCatalogDlg->exec())
	{
		strCatalog.append(pLineEdCatalog->text().remove(" "));
	}
	else
	{
		return;
	}

	*/
	QString strCatalog = ui.lineEdit->text().remove(" ");

	if (strCatalog.isEmpty())
	{
		strCatalog = "/tffs0";
		ui.lineEdit->setText(strCatalog);
	}

	m_strCurrentPath = strCatalog;
	/*
	if (strCatalog.split("/").count() > 2)
	{
		m_strCurrentPath = strCatalog.split("/").last();
		strCatalog = m_strCurrentPath;

	}
	else
	{
		if (!strCatalog.isEmpty())
		{
			m_strCurrentPath = strCatalog.split("/").last();
			strCatalog = m_strCurrentPath;

		}
		else
		{
			m_strCurrentPath.clear();
			strCatalog.clear();

		}

	}

	*/
	ui.pushButton->setEnabled(false);
	m_pTimer->setSingleShot(true);
	m_pTimer->start();

	ui.treeWidget->clear();

	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, strCatalog.toStdString().c_str());    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

	tReadAction.m_strFileName = "";    //文件名称

	tReadAction.m_nOperatorType = 8;    //召唤数据

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);

	if (m_strCurrentPath != "/tffs0" && ui.treeWidget->topLevelItemCount() == 0)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setText(0, "...");
		pItem->setData(0, Qt::UserRole + 2, 1);
		ui.treeWidget->addTopLevelItem(pItem);
	}
}

void CFileTransWgt::Slot_DownLoadItems()
{
	//
	if (ui.treeWidget->currentItem() == nullptr)
	{
		return;
	}

	if (ui.treeWidget->currentItem()->data(0,Qt::UserRole+1).toInt() == 1)
	{
		//目录
		QMessageBox::warning(0, tr("Warning"), tr("Please Choose File"));
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"), m_strFilePath + ui.treeWidget->currentItem()->text(0).split("/").last());

	QString strTempName = fileName;

	m_strFilePath = strTempName.remove(strTempName.split("/").last());

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, "");    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		if (m_strCurrentPath.isEmpty())
		{
			tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0);    //文件名称

		}
		else
		{
			tReadAction.m_strFileName = m_strCurrentPath + "/" + ui.treeWidget->currentItem()->text(0);    //文件名称

		}
		tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0);

		tReadAction.m_nOperatorType = 1;

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		//9 为下载文件
		tReadAction.m_cFileAttr = 9; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		m_pCommuncate->SendCatalogRequest(tCatalogRequest);

		g_FileName = fileName;
	}
}

void CFileTransWgt::Slot_upLoadItems()
{

	if (ui.treeWidget->currentItem() == nullptr)
	{
		return;
	}

	if (ui.treeWidget->currentItem()->data(0, Qt::UserRole + 1).toInt() != 1)
	{
		//目录
		QMessageBox::warning(0, tr("Warning"), tr("Please Choose Folder"));
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open File"));

	QString tFileOwnName = fileName.split("/").last();

	if (!fileName.isEmpty())
	{
		//
		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, "");    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

											   //tWriteAction.m_nOperatorType = 7;    //写目录

											   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0
		//if (m_strCurrentPath.isEmpty())
		//{
			tReadAction.m_strFileName = ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName;    //文件名称

		//}
		//else
		//{
		//	tReadAction.m_strFileName = m_strCurrentPath + "/" + ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName;    //文件名称

		//}

		
		tReadAction.m_nOperatorType = 9;    //只召唤目录

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 0; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		//m_pCommuncate->SendCatalogRequest(tCatalogRequest);
		//Slot_CheckDeviceTable();

		FILE_ATTR_INFO tWriteAction;

		tWriteAction.asduAddr.SetAddr(1);    //装置地址
		tWriteAction.cot.SetCot(COT_ACT);   //激活

		tWriteAction.type = D_FILE_TRANSPORT;   //文件传输

		tWriteAction.m_nOperatorType = 7;    //写目录

		tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

		if (m_strCurrentPath.isEmpty())
		{
			tWriteAction.m_strFileName = ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName; ;    //文件名称

		}
		else
		{
			tWriteAction.m_strFileName =  ui.treeWidget->currentItem()->text(0) + "/" + tFileOwnName; ;    //文件名称

		}

		tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //目录长度

		QFile tFile(fileName);

		if (!tFile.open(QFile::ReadOnly))
		{
			QMessageBox::warning(this, tr("Warning"), tr("file open file error!"));
			return;
		}

		tWriteAction.m_FileSize = tFile.size();   //文件大小

		tWriteAction.m_btArrayData = tFile.readAll();   //发送数据


		m_pCommuncate->SendWriteActionRequest(tWriteAction);
	}


}

void CFileTransWgt::Slot_upDataITems(QList<Catalog_Info>& lstItems)
{
	//清空树
	/*
	int i = 0;
	if (lstItems.isEmpty())
	{
		if (!m_strCurrentPath.isEmpty())
		{
			QTreeWidgetItem *pItem = new QTreeWidgetItem;
			pItem->setText(0, "...");
			pItem->setData(0, Qt::UserRole + 2, 1);
			ui.treeWidget->addTopLevelItem(pItem);
			i = 1;
		}
	}
	*/

	if (m_strCurrentPath != "/tffs0"&& ui.treeWidget->topLevelItemCount() == 0)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setText(0, "...");
		pItem->setData(0, Qt::UserRole + 2, 1);
		ui.treeWidget->addTopLevelItem(pItem);
	}

	for (auto fileInfo : lstItems)
	{
		//遍历所有树 和 子节点
		/*
		if (!m_strCurrentPath.isEmpty() && i == 0)
		{
			QTreeWidgetItem *pItem = new QTreeWidgetItem;
			pItem->setText(0, "...");
			pItem->setData(0, Qt::UserRole + 2, 1);
			ui.treeWidget->addTopLevelItem(pItem);
			i = 1;
		}
		*/
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setText(0,fileInfo.m_strFileName);
		//add by wangwei 20180131
		pItem->setText(1, fileInfo.m_strTimeFile);
		pItem->setData(0,Qt::UserRole+1,fileInfo.m_cFileAttr);
		ui.treeWidget->addTopLevelItem(pItem);
	}
}

void CFileTransWgt::Slot_ItemDoubleClicke(QTreeWidgetItem * item, int column)
{
	//双击
	if (item->data(0,Qt::UserRole+1) == 1 || item->data(0, Qt::UserRole + 2).toInt() == 1)
	{
		//如果是目录就更新
		//目录名称
		if (m_strCurrentPath!= "/tffs0" && item->data(0,Qt::UserRole+2).toInt() == 1)
		{
			QString strFilename = ui.lineEdit->text();
			m_strCurrentPath = strFilename;

			if (m_strCurrentPath.contains("/" + m_strCurrentPath.split("/").last()))
			{
				//返回上一级目录
				m_strCurrentPath.remove("/" + m_strCurrentPath.split("/").last());
				//m_strCurrentPath.clear()
				ui.lineEdit->setText(strFilename.remove("/" + strFilename.split("/").last()));

			}
			else
			{
				//返回上一级目录
				m_strCurrentPath.remove( m_strCurrentPath.split("/").last());
				//m_strCurrentPath.clear()
				ui.lineEdit->setText(strFilename.remove(strFilename.split("/").last()));

			}
		}
		else
		{
			QString strCatalogName = item->text(0);
			m_strCurrentPath = strCatalogName;

			ui.lineEdit->setText(m_strCurrentPath);

		}
		//ui.lineEdit->setText(m_strCurrentPath);

		ui.treeWidget->clear();

		FILE_CATALOG_REQUEST_1 tCatalogRequest;

		tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
		tCatalogRequest.cot.SetCot(COT_REQ);   //激活

		tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

		tCatalogRequest.m_nOperatorType = 1;    //读目录
		tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

		strcpy(tCatalogRequest.m_cCatalogName, m_strCurrentPath.toStdString().c_str());    //目录
		tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

		tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
										   //默认时间为所有时间

										   //发送数据


		FILE_ATTR_INFO tReadAction;

		tReadAction.asduAddr.SetAddr(1);    //装置地址
		tReadAction.cot.SetCot(COT_ACT);   //激活

		tReadAction.type = D_FILE_TRANSPORT;   //文件传输

		tReadAction.m_strFileName = "";    //文件名称

		tReadAction.m_nOperatorType = 8;    //召唤数据

		tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

		tReadAction.m_cFileAttr = 0; //文件

		tCatalogRequest.m_fileInfo = tReadAction;

		m_pCommuncate->SendCatalogRequest(tCatalogRequest);

		if (m_strCurrentPath != "/tffs0")
		{
			QTreeWidgetItem *pItem = new QTreeWidgetItem;
			pItem->setText(0, "...");
			pItem->setData(0, Qt::UserRole + 2, 1);
			ui.treeWidget->addTopLevelItem(pItem);
		}
	}
}

void CFileTransWgt::Slot_UpdateConform(int nFlag)
{
	Q_UNUSED(nFlag);



	if (m_strUpdateFile.isEmpty())
	{
		return;
	}
	else
	{
		SendUpdateFile(m_strUpdateFile, "");

	}
	/*
	QFileInfoList lstFiles = tDir.entryInfoList(QDir::Files);

	if (lstFiles.count() > 1)
	{
		QMessageBox::warning(this,tr("告警"),tr("updatefile 目录下存在多个文件,请检查!"));
		return;
	}
	for (int i = 0; i < lstFiles.count(); i++)
	{
		if (lstFiles.at(i).fileName().endsWith(".bin"))
		{
			SendUpdateFile(qApp->applicationDirPath() + "/ini/f220/updatefile/" + lstFiles.at(i).fileName(), "");
		}
		else
		{
			QMessageBox::warning(this, tr("告警"), tr("updatefile 目录下存下文件名格式不对!"));
			return;

		}

	}
	*/
}

void CFileTransWgt::SLot_TimeOut()
{
	ui.pushButton->setEnabled(true);
}
