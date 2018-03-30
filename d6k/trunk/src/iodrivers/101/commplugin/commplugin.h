/********************************************************************************
模块名       :  APDU数据管理类
文件名       :  commplugin.h
文件实现功能 :  APDU结构
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      xingzhibing        修改内容

********************************************************************************/
#ifndef COMMPLUGIN_H
#define COMMPLUGIN_H

#include <QObject>
#include <QThread>
#include <QAbstractSocket>
#include "commplugin_global.h"
#include "101cache.h"
/*
#include "101recver.h"
#include "101sender.h"*/
#include "icommplugin.h"
#include "asdu_info.h"
#include "ftu_module.h"


class CSocketThread;
class QTimer;
class C101Sender;
class C101Recver;
class CFtuModule;
struct NBM_TELECTRL;

class  CCommPlugin :public ICommPlugin
{
    Q_OBJECT
public:
    CCommPlugin(CFtuModule *pModule);
    ~CCommPlugin();
    //开始运行
	void StartRun(const QString &strIP, int iPort);
	//结束运行
	void StopRun();

    bool OnReceive(char *pBuff, int nLen);
    //写入数据
    bool WriteApduData(const char *pSendData, int iLength);

	C101Sender* getSender();

	C101Recver *GetRecver()
	{
		Q_ASSERT(m_pApduRecver);
		return m_pApduRecver;
	}

    bool OnCommand(NBM_TELECTRL* pTelectrl);
	// 遥控命令
	void SetControlBckCommand(int iControlType, int ipointNum, int OperateType);
    //
    CFtuModule *GetFCIModule();
    //
    void SetGeneralTime(int nGeneralTime, int nSynTime, int nKwhTime);
    //
	void SetTimerx(int nTime0, int nTime1, int nTime2, int nTime3);

public:
    bool IsRunning() { return m_bIsRunning; }
    bool m_bIsRunning;

private slots:
    void Slot_OnRecvData();

    void Slot_SocketError(QString error);
    //连接成功
	void Slot_sockeConnectSuccess(QString strLocalInfo);
	//t0超时
	void Slot_TimeOutT0();
	//t1超时
	void Slot_TimeOutT1();
	//t2超时
	void Slot_TimeOutT2();
	//t3超时
	void Slot_timeOutT3();
	//总召超时
	void Slot_SendGeneralResquestMsg();
    //时间同步
    void Slot_SendSyncRequestMsg();

	void slot_RdTimeRequest();

	void slot_TcCommand();
    //电度召唤
    void Slot_SendKwhTimeRequestMsg();
    //时间手动同步
    void Slot_SetSycsTime(QDateTime tSycsTime);
    //复位进程
    void Slot_ResetProcess();
	//断连
	void Slot_DisConnect();
	//
	void Slot_AllCallRespond();

public slots:
	// 遥控命令
    void Slot_SetControlCommand(int iControlType, int ipointNum, int OperateType, int nMessType);
	//定值指令
	void Slot_setDevOrder(DEV_BASE &devData);
	//iec指令
	void Slot_SetIecOrder(IEC_BASE &iecData);
    //录波文件获取指令
    void Slot_SetRecordOrder(QList<LB_DATA> &lbData);
    //死区
    void Slot_SetZoomArea(ZOOM_BASE &zoomBaseInfo);
	//切换定值区
	void Slot_SwitchFixArea(unsigned short uFixArea);
	//读取当前定值区号
	void Slot_ReadCurrentFixArea();

	//目录召唤
	void Slot_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest);
	//文件写入激活
	void Slot_WriteAction(FILE_ATTR_INFO &tFileInfo);
	//文件读取激活
	void Slot_ReadAction(FILE_ATTR_INFO &tFileInfo);

	void slot_OnCkeckList();
signals:
	//发送出去的数据
	void Signal_Write16Data(QByteArray btbuff);
	//接收到的数据报文
	void Signal_recv16Data(QByteArray btData, int nLenght);

private:
    CSocketThread *m_pSocketThread;
    //发送类
    C101Sender *m_pApduSender;
    //接收类
    C101Recver *m_pApduRecver;
	//IP地址
	QString m_strIP;
	//端口号
	int m_iPort;
	
	QTimer * m_pCycleCheck;

	QTime * m_pResendTimer;

	static int m_nReSendTimer;

	//四个超时设置
	QTimer *m_pTimerOut0;
	QTimer *m_pTimerOut1;
	QTimer *m_pTimerOut2;
	QTimer *m_pTimerOut3;
	//定时发送总召
	QTimer *m_pTimeGeneralSendMsg;
    //定时发送时间同步
    QTimer *m_pTimerSyncTimeMsg;
    //定时发送电度召唤
    QTimer *m_pTimerKwhMsg;
	//心跳报文测试
	QTimer *m_pTimerHeatBeat;

    CFtuModule *m_pModule;
};

#endif // COMMPLUGIN_H
