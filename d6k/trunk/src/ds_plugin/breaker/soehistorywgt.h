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
	//发送获取报文
	void OnSendGetInfo(int nSoeType, int SoeIndex);

	//动作soe
	void AnaylseActionData(SOE_ACTION_INFO *pAction);
	//变位soe
	void AnayseIoData(SOE_IO_INFO *pIO);
	//异常SOE
	void AnalyseAbnormalData(SOE_ID_INFO *pSoeInfo);
	//运行SOE
	void AnalyseRuningData(SOE_ID_INFO *pSoeInfo);
	//录波SOE
	void AnalyseRecordData(SOE_ID_INFO *pSoeInfo);

	//soe数量解析
	void AnalyseNumsData(SOE_NUM_INFO *pNums);

	//发送数据请求
	void OnSendRequestData();
	//clear
	void ClearNums()
	{
		m_pSoeStatic->ClearStatic();
	}

	//初始化精度条
	void InitProcessBar();
	//更新进度条
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
	//调用录波软件
	void StartWaveExe(const QString &strFilename);
public slots:

//收取详细信息
    void Slot_RecvNewRealTimeData(DEG_SOE_DETAIL tSoeDetail);

	//清除
	void Slot_ClearTable();
	//右击菜单
	void Slot_ContextMenuRequest(const QPoint &point);
	//更新信息
	void Slot_UpdateNumInfo();
	//开始读取数据
	void Slot_BeginReadHistorySoe();

	//wave doubleclick
	void Slot_ItemDoubleClicked(QTableWidgetItem *item);
	//
	//
	void Slot_DownloadProcess(qint64 bytesReceived, qint64 bytesTotal);
	//error
	void Slot_FtpDone(bool);
	//保存历史事件
	void SaveHistoryEvent();
	//
	void Slot_OpenWaveFile();
	//
	void Slot_FtpStateChange(int);
private:
	Ui::CSoeHistoryWgt ui;
	//网络
	CNetManager *m_pNetManager;
	//
	std::shared_ptr<SOE_EVENT_STATIC> m_pSoeStatic;
	//精度条
	QProgressBar *m_pActionBar;
	//开入
	QProgressBar *m_pDiBar;
	//异常
	QProgressBar *m_pAbnormalBar;
	//运行
	QProgressBar *m_pRunBar;
	//录波
	QProgressBar *m_pPWaveBar;
	//调试信息
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
	//需要下载的文件列表
	QStringList m_lstFilelst;
	//文件
	QFile *m_pDownLoadFile;
	QProcess *m_pCommProcess;
};
