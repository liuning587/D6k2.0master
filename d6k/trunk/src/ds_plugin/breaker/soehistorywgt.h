#pragma once

#include <QWidget>
#include "ui_soehistorywgt.h"
#include <memory>
#include "breaker_data.h"

class CNetManager;
class SOE_ACTION_INFO;
class SOE_NUM_INFO;
class QProgressBar;
class QTableWidgetItem;
class QFtp;
class QFile;
class QProcess;

class CSoeHistoryWgt : public QWidget
{
	Q_OBJECT

public:
	CSoeHistoryWgt(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CSoeHistoryWgt();
	//���ͻ�ȡ����
	void OnSendGetInfo(int nSoeType, int SoeIndex);

	//����soe
	void AnaylseActionData(SOE_ACTION_INFO *pAction);
	//��λsoe
	void AnayseIoData(SOE_IO_INFO *pIO);
	//�쳣SOE
	void AnalyseAbnormalData(SOE_ID_INFO *pSoeInfo);
	//����SOE
	void AnalyseRuningData(SOE_ID_INFO *pSoeInfo);
	//¼��SOE
	void AnalyseRecordData(SOE_ID_INFO *pSoeInfo);

	//soe��������
	void AnalyseNumsData(SOE_NUM_INFO *pNums);

	//������������
	void OnSendRequestData();
	//clear
	void ClearNums()
	{
		m_pSoeStatic->ClearStatic();
	}

	//��ʼ��������
	void InitProcessBar();
	//���½�����
	void UpdateProcess();
	//
	void SetFtpIp(const QString &strFtpIp);
	//
	void SetFtpUserName(const QString &strFtpUserName);
	//
	void SetFtpPasswd(const QString &strFtpPasswd);
	//
	void SetFtpDir(const QString &strFtpDIr);
	//
	void FtpDownLoadFile(const QString &strFilename);
	//����¼�����
	void StartWaveExe(const QString &strFilename);
public slots:

//��ȡ��ϸ��Ϣ
    void Slot_RecvNewRealTimeData(DEG_SOE_DETAIL tSoeDetail);

	//���
	void Slot_ClearTable();
	//�һ��˵�
	void Slot_ContextMenuRequest(const QPoint &point);
	//������Ϣ
	void Slot_UpdateNumInfo();
	//��ʼ��ȡ����
	void Slot_BeginReadHistorySoe();

	//wave doubleclick
	void Slot_ItemDoubleClicked(QTableWidgetItem *item);
	//
	//
	void Slot_DownloadProcess(qint64 bytesReceived, qint64 bytesTotal);
	//error
	void Slot_FtpDone(bool);
	//������ʷ�¼�
	void SaveHistoryEvent();
	//
	void Slot_OpenWaveFile();
	//
	void Slot_FtpStateChange(int);
private:
	Ui::CSoeHistoryWgt ui;
	//����
	CNetManager *m_pNetManager;
	//
	std::shared_ptr<SOE_EVENT_STATIC> m_pSoeStatic;
	//������
	QProgressBar *m_pActionBar;
	//����
	QProgressBar *m_pDiBar;
	//�쳣
	QProgressBar *m_pAbnormalBar;
	//����
	QProgressBar *m_pRunBar;
	//¼��
	QProgressBar *m_pPWaveBar;
	//������Ϣ
	QProgressBar *m_pDebugBar;
	//ftp ip
	QString m_strFtpIp;
	//ftp username
	QString m_strFtpUserName;
	//ftp password
	QString m_strFtpPassWd;
	//ftp dir
	QString m_strDir;
	//ftp
	QFtp *m_pFtp;;
	//��Ҫ���ص��ļ��б�
	QStringList m_lstFilelst;
	//�ļ�
	QFile *m_pDownLoadFile;
	QProcess *m_pCommProcess;
};
