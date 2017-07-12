#pragma once

#include <QWidget>
#include "ui_filetranswgt.h"
#include "asdu_info.h"

extern QString g_FileName;

class CCommThread;
class CFileTransWgt : public QWidget
{
	Q_OBJECT

public:
	CFileTransWgt(CCommThread *pCommunicate,QWidget *parent = Q_NULLPTR);
	~CFileTransWgt();
	//发送指定文件  升级
	void SendUpdateFile(const QString &strFileName, const QString &Dir);
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


private:
	Ui::CFileTransWgt ui;
	//
	CCommThread *m_pCommuncate;
};
