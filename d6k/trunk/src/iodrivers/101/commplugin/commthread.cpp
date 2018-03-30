#include "commthread.h"
#include "commplugin.h"
#include "101sender.h"
#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include <windows.h>

CCommThread::CCommThread(QObject *parent)
	: QObject(parent)
{
    m_pModule = qobject_cast<CFtuModule*>(parent);

    Q_ASSERT(m_pModule);
    if (m_pModule == nullptr)
    {
        return;
    }

	m_pThread= new QThread(this);
	this->moveToThread(m_pThread);
	m_pThread->start(QThread::TimeCriticalPriority);

	qRegisterMetaType<DEV_BASE>("DEV_BASE&");
	qRegisterMetaType<IEC_BASE>("IEC_BASE&");
	qRegisterMetaType<QMap<int, float>>("QMap<int,float>");
    qRegisterMetaType<QList<LB_DATA>>("QList<LB_DATA>&");
    qRegisterMetaType<ZOOM_BASE>("ZOOM_BASE&");
	qRegisterMetaType<FILE_CATALOG_REQUEST_1>("FILE_CATALOG_REQUEST_1&");
	qRegisterMetaType<FILE_ATTR_INFO>("FILE_ATTR_INFO&");
	qRegisterMetaType<QList<Catalog_Info>>("QList<Catalog_Info>&");
	qRegisterMetaType<ASDUGZ>("ASDUGZ");
	qRegisterMetaType<ASDU211_UPDATE>("ASDU211_UPDATE&");

	m_nTimerConnect = new QTimer;
	m_nTimerConnect->setInterval(1000);
	m_nTimerConnect->setSingleShot(true);
	m_pCommPlugin = new CCommPlugin(m_pModule);
	connect(m_nTimerConnect,SIGNAL(timeout()),this,SLOT(Solt_Connect()));
}

CCommThread::~CCommThread()
{
	stop();
	m_pThread->quit();
	m_pThread->wait();
	m_pThread->deleteLater();
}

void CCommThread::Solt_Connect()
{
	emit Signal_ConnectSocket();
}

//开启线程
void CCommThread::RunFunc()
{
	
	m_pCommPlugin->SetGeneralTime(m_nGeneranTime, m_nSyncGeneralTime, m_nKwhCallTime);
	m_pCommPlugin->SetTimerx(m_nTime0,m_nTime1,m_nTime2,m_nTime3);
	m_pCommPlugin->StartRun(m_strIP, m_iPort);

	//发出信号
	connect(m_pCommPlugin, SIGNAL(Signal_SocketError(QString)), this, SIGNAL(Signal_SocketError(QString)),Qt::QueuedConnection);
	//连接成功
	connect(m_pCommPlugin, SIGNAL(Signal_SocketConnectSuccess(QString)), this, SLOT(Slot_ConnectSuccess(QString)),Qt::QueuedConnection);
	//总召唤确认
	connect(m_pCommPlugin, SIGNAL(Signal_AllCallRespond()), this, SIGNAL(Signal_AllCallRespond()));
	connect(m_pCommPlugin, SIGNAL(Signal_onePointDisRemote(int, int, int)), this, SIGNAL(Signal_onePointDisRemote(int, int, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_OnePointRemote(int, int, int)), this, SIGNAL(Signal_OnePointRemote(int, int, int)));

	connect(m_pCommPlugin, SIGNAL(Signal_DoublePointRemote(int, int, int)), this, SIGNAL(Signal_DoublePointRemote(int, int, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), this, SIGNAL(Signal_DoublePointDisRemote(int, int, int)));

	connect(m_pCommPlugin, SIGNAL(Signal_BinaryGroupPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupPoint(int, int, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)));


	//SOE
    connect(m_pCommPlugin, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)));
    connect(m_pCommPlugin, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)), this, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)));
    connect(m_pCommPlugin, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)));
    connect(m_pCommPlugin, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)));

	//kwh
	connect(m_pCommPlugin, SIGNAL(Signal_KwhNormal(int, int, int)), this, SIGNAL(Signal_KwhNormal(int, int, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_KwhDisNormal(int, int, int)), this, SIGNAL(Signal_KwhDisNormal(int, int, int)));

	connect(m_pCommPlugin, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));
	connect(m_pCommPlugin, SIGNAL(Signal_KwhLongTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));

	//遥测
	connect(m_pCommPlugin, SIGNAL(Signal_AnalogNormal(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));

	connect(m_pCommPlugin, SIGNAL(Signal_AnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_DisAnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));
	connect(m_pCommPlugin, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)), this, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)));
    //故障
    connect(m_pCommPlugin, SIGNAL(Signal_MalFuction(ASDUGZ)), this, SIGNAL(Signal_MalFuction(ASDUGZ)));

	//发送和接收到的数据
	connect(m_pCommPlugin, SIGNAL(Signal_Write16Data(QByteArray)), this, SIGNAL(Signal_Write16Data(QByteArray)));
    connect(m_pCommPlugin, SIGNAL(Signal_recv16Data(QByteArray, int)), this, SIGNAL(Signal_recv16Data(QByteArray, int)));
	//遥控指令
	connect(this, SIGNAL(Signal_Control(int, int, int,int)), m_pCommPlugin, SLOT(Slot_SetControlCommand(int, int, int,int)));
	//定值指令
	connect(this, SIGNAL(Signal_DevSendInfo(DEV_BASE &)), m_pCommPlugin, SLOT(Slot_setDevOrder(DEV_BASE &)));
	//
	connect(this, SIGNAL(Signal_IecSendInfo(IEC_BASE &)), m_pCommPlugin, SLOT(Slot_SetIecOrder(IEC_BASE &)));
    //死区设置
    connect(this, SIGNAL(Signal_ZoomArea(ZOOM_BASE &)), m_pCommPlugin, SLOT(Slot_SetZoomArea(ZOOM_BASE &)));

	//遥控反馈
	connect(m_pCommPlugin, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)), this, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)));
	//定值
	connect(m_pCommPlugin, SIGNAL(Signal_DevReadBack(QMap<int, float>)), this, SIGNAL(Signal_DevReadBack(QMap<int, float>)));
    //死区
    connect(m_pCommPlugin, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)));
	//定值
	connect(m_pCommPlugin, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)));

	//定值设定
	connect(m_pCommPlugin, SIGNAL(Signal_devWriteBack(int, int, int)), this, SIGNAL(Signal_devWriteBack(int, int, int)));
    //录波文件获取
    connect(this, SIGNAL(Signal_RecordSendInfo(QList<LB_DATA> &)), m_pCommPlugin, SLOT(Slot_SetRecordOrder(QList<LB_DATA> &)));
    //手动总召
    connect(this, SIGNAL(Signal_GeneralCall()), m_pCommPlugin, SLOT(Slot_SendGeneralResquestMsg()));
    //电度召唤
    connect(this, SIGNAL(Signal_KwhCall()), m_pCommPlugin, SLOT(Slot_SendKwhTimeRequestMsg()));
    //对时
    connect(this, SIGNAL(Signal_SycsTime(QDateTime)), m_pCommPlugin, SLOT(Slot_SetSycsTime(QDateTime)));
    //复位进程
    connect(this, SIGNAL(Signal_ResetProcess()), m_pCommPlugin, SLOT(Slot_ResetProcess()));

	//切换定制区
	connect(this,SIGNAL(Signal_SwitchFixArea(unsigned short)), m_pCommPlugin,SLOT(Slot_SwitchFixArea(unsigned short)));
	//读取当前定值区号
	connect(this,SIGNAL(Signal_ReadFixArea()), m_pCommPlugin, SLOT(Slot_ReadCurrentFixArea()));

	//目录召唤
	connect(this,SIGNAL(Signal_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &)), m_pCommPlugin,SLOT(Slot_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &)));

	//写入激活
	connect(this,SIGNAL(SIGNAL_WriteActionRequest(FILE_ATTR_INFO &)), m_pCommPlugin,SLOT(Slot_WriteAction(FILE_ATTR_INFO &)));
	//读取激活
	connect(this, SIGNAL(Signal_ReadActionRequest(FILE_ATTR_INFO &)), m_pCommPlugin, SLOT(Slot_ReadAction(FILE_ATTR_INFO &)));
	//定值读取
	connect(m_pCommPlugin,SIGNAL(Signal_ReadFixData(DEV_BASE &)),this,SIGNAL(Signal_ReadFixData(DEV_BASE &)));
	//连接
	connect(this,SIGNAL(Signal_ConnectSocket()), m_pCommPlugin,SLOT(Slot_TimeOutT1()));
	//断连
	connect(this, SIGNAL(Signal_DisConnectSocket()), m_pCommPlugin, SLOT(Slot_DisConnect()));
}

//获取发送方信息
QString CCommThread::GetOwnNetInfo()
{
	return m_LocalInfo;
}
//获取对方信息
QString CCommThread::GetOtherInfo()
{
	return m_strIP + ":" + QString::number(m_iPort);
}

void CCommThread::Slot_ConnectSuccess(QString strLocalInfo)
{
	m_LocalInfo = strLocalInfo;
	emit Signal_SocketConnectSuccess();
}



//发送遥控命令接口
void CCommThread::SendRemoteControl(int iControlType, int iPointNum, int iOpType,int iMessType)
{
	emit Signal_Control(iControlType, iPointNum, iOpType,iMessType);
}

//发送定值指令
void CCommThread::SendDevOrder(DEV_BASE &dveBaseInfo)
{
	emit Signal_DevSendInfo(dveBaseInfo);
}

//iec
void CCommThread::SendIecOrder(IEC_BASE &iecBaseInfo)
{
	emit Signal_IecSendInfo(iecBaseInfo);
}

//死区设置
void CCommThread::SetZoomArea(ZOOM_BASE &zoomBaseInfo)
{
    emit Signal_ZoomArea(zoomBaseInfo);
}


void CCommThread::SetFixAreaCall(unsigned short uFixArea)
{
	emit Signal_SwitchFixArea(uFixArea);
}

void CCommThread::ReadFixArea()
{
	emit Signal_ReadFixArea();
}



void CCommThread::SendCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest)
{
	emit Signal_ReadCatalogRequest(catalogRequest);
}

//文件写入激活
void CCommThread::SendWriteActionRequest(FILE_ATTR_INFO &writeAction)
{
	emit SIGNAL_WriteActionRequest(writeAction);;
}

void CCommThread::SendReadActionRequest(FILE_ATTR_INFO &ReadAction)
{
	emit Signal_ReadActionRequest(ReadAction);
}

void CCommThread::SendConnectRequest()
{
	RunFunc();
	m_pThread->start();
}

void CCommThread::SendDisConnectRequest()
{
	m_pThread->quit();
	m_pThread->wait();
}

void CCommThread::SendUpdateRequest(ASDU211_UPDATE & updateData)
{
	//更新程序
	emit Singal_updateProcess(updateData);
}

void CCommThread::SetTimer(int nTime0, int nTime1, int nTime2, int nTime3)
{
	m_nTime0 = nTime0;
	m_nTime1 = nTime1;
	m_nTime2 = nTime2;
	m_nTime3 = nTime3;
}

void CCommThread::stop()
{
	m_pCommPlugin->StopRun();
	emit sig_Stop();
}

//开始运行
void CCommThread::StartRun(const char* strIP, int iPort)
{
	m_strIP = strIP;
	m_iPort = iPort;
	RunFunc();
	m_pThread->start();
}
//

void CCommThread::SendRecordOrder(QList<LB_DATA> &lbInfo)
{
    emit Signal_RecordSendInfo(lbInfo);
}

void CCommThread::SetGeneralTimes(int nGeneralTime, int nSyncTime, int nKwhTime)
{
    m_nGeneranTime = nGeneralTime;
    m_nSyncGeneralTime = nSyncTime;
    m_nKwhCallTime = nKwhTime;
}

void CCommThread::SetGeneralCall()
{
    emit Signal_GeneralCall();
}

//手动电度召唤
void CCommThread::SetRdTime()
{
    emit sig_RdTime();
}

void CCommThread::SetTcCommand()
{
	emit sig_TcCommand();
}

