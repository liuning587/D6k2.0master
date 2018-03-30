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
	//����ָ���ļ�  ����
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
	//˫��
	void Slot_ItemDoubleClicke(QTreeWidgetItem *item, int column);
	//
	void Slot_UpdateConform(int nFlag);
	//
	void SLot_TimeOut();
	//��ȡ���
	void Slot_GetPointTable();
	//��װ���
	void Slot_DownLoadPointTable();

private:
	Ui::CFileTransWgt ui;
	//
	CCommThread *m_pCommuncate;
	//�ļ������·��
	QString m_strFilePath;
	//
	QTimer *m_pTimer;
	//�ϴ����ļ�����
	QString m_strUpdateFile;
	//��ǰ�ļ�����Ŀ¼   ��ԼĿ¼
	QString m_strCurrentPath;
};
