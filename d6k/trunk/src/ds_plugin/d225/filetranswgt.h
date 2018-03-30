#pragma once

#include <QWidget>
#include "ui_filetranswgt.h"
#include "asdu_info.h"

extern QString g_FileName;

class CCommThread;
class QTimer;
class CFileTransWgt : public QWidget
{
	Q_OBJECT

public:
	CFileTransWgt(CCommThread *pCommunicate,QWidget *parent = Q_NULLPTR);
	~CFileTransWgt();
	//发送指定文件  升级
	void SendUpdateFile(const QString &strFileName, const QString &Dir);
	//
	void SetUpdateFile();
public slots:
    //refresh
    void Slot_RefreshItems();
	//downloa
	void Slot_DownLoadItems();
	//upload
	void Slot_upLoadItems();

	//update items
	void Slot_upDataITems(QList<Catalog_Info>& lstItems);
	//双击
	void Slot_ItemDoubleClicke(QTreeWidgetItem *item, int column);
	//
	void Slot_UpdateConform(int nFlag);
	//
	void SLot_TimeOut();
	//获取点表
	void Slot_GetPointTable();
	//下装点表
	void Slot_DownLoadPointTable();

private:
	Ui::CFileTransWgt ui;
	//
	CCommThread *m_pCommuncate;
	//文件保存的路径
	QString m_strFilePath;
	//
	QTimer *m_pTimer;
	//上传的文件名称
	QString m_strUpdateFile;
	//当前文件所处目录   规约目录
	QString m_strCurrentPath;
};
