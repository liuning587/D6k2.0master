#include <QTimer>
#include <QDebug>
#include "commplugin.h"
#include "socketthread.h"
#include "asdu_info.h"
#include "define_101.h"
#include "101recver.h"
#include "101sender.h"
#include "ftu_module.h"
#include "devicestudio/main_module.h"

int CCommPlugin::m_nReSendTimer = 1000;

/*********************************************************************************************************
** \brief CCommPlugin()
** \details 发送类构造函数
** \param argl   父类指针
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
CCommPlugin::CCommPlugin(CFtuModule *pModule)
{
    m_pModule = pModule;

	m_pApduSender = new C101Sender(this);

	m_pApduRecver = new C101Recver(this);

	m_pSocketThread = new CSocketThread(this);

	m_pCycleCheck = new QTimer;

	m_pResendTimer = new QTime;

	m_pCycleCheck->setInterval(500);

	m_pSocketThread->setApduRecver(m_pApduRecver);

	m_pSocketThread->setSender(m_pApduSender);

	m_pSocketThread->SetModule(pModule);

	connect(m_pSocketThread, SIGNAL(Signal_SocketError(QString)), this, SLOT(Slot_SocketError(QString)),Qt::DirectConnection);
	//连接成功
	connect(m_pSocketThread, SIGNAL(Signal_ConnectSuccess(QString)), this, SLOT(Slot_sockeConnectSuccess(QString)), Qt::QueuedConnection);

	QObject::connect(m_pCycleCheck, SIGNAL(timeout()), this, SLOT(slot_OnCkeckList()));

	//总召唤确认
	connect(m_pApduRecver, SIGNAL(Signal_AllCallRespond()), this, SLOT(Slot_AllCallRespond()));
	connect(m_pApduRecver, SIGNAL(Signal_onePointDisRemote(int, int, int)), this, SIGNAL(Signal_onePointDisRemote(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_OnePointRemote(int, int, int)), this, SIGNAL(Signal_OnePointRemote(int, int, int)));

	connect(m_pApduRecver, SIGNAL(Signal_DoublePointRemote(int, int, int)), this, SIGNAL(Signal_DoublePointRemote(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), this, SIGNAL(Signal_DoublePointDisRemote(int, int, int)));

	connect(m_pApduRecver, SIGNAL(Signal_BinaryGroupPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupPoint(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)));


	//SOE
	connect(m_pApduRecver, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)));
	connect(m_pApduRecver, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)));
	connect(m_pApduRecver, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)), this, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)));
	connect(m_pApduRecver, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)));

	//kwh
	connect(m_pApduRecver, SIGNAL(Signal_KwhNormal(int, int, int)), this, SIGNAL(Signal_KwhNormal(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_KwhDisNormal(int, int, int)), this, SIGNAL(Signal_KwhDisNormal(int, int, int)));

	connect(m_pApduRecver, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));
	connect(m_pApduRecver, SIGNAL(Signal_KwhLongTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));

	//遥测
	connect(m_pApduRecver, SIGNAL(Signal_AnalogNormal(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(m_pApduRecver, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));

	connect(m_pApduRecver, SIGNAL(Signal_AnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(m_pApduRecver, SIGNAL(Signal_DisAnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));

	connect(m_pApduRecver, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)), this, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)));
	//遥控
	connect(m_pApduRecver, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)), this, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)));

	//发送的数据
	connect(m_pSocketThread, SIGNAL(Signal_Write16Data(QByteArray)), this, SIGNAL(Signal_Write16Data(QByteArray)));
	//接收的数据
	connect(m_pApduRecver, SIGNAL(Signal_recv16Data(QByteArray, int)), this, SIGNAL(Signal_recv16Data(QByteArray, int)));
	//定值
	connect(m_pApduRecver, SIGNAL(Signal_DevReadBack(QMap<int, float>)), this, SIGNAL(Signal_DevReadBack(QMap<int, float>)));
	//定值设定
	connect(m_pApduRecver, SIGNAL(Signal_devWriteBack(int, int, int)), this, SIGNAL(Signal_devWriteBack(int, int, int)));
	//故障
	connect(m_pApduRecver, SIGNAL(Signal_MalFuction(ASDUGZ)), this, SIGNAL(Signal_MalFuction(ASDUGZ)));
	//死区
	connect(m_pApduRecver, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)));
	//定值读取
	connect(m_pApduRecver, SIGNAL(Signal_ReadFixData(DEV_BASE &)), this, SIGNAL(Signal_ReadFixData(DEV_BASE &)));
	
}

CCommPlugin::~CCommPlugin()
{
	int nn = 0;
	m_pCycleCheck->stop();
	m_pCycleCheck->deleteLater();
}

/*********************************************************************************************************
** \brief RunStart(const QString &strIP, int iPort)
** \details 启动接收数据
** \param argl   IP地址
** \param arg2   端口号
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::StartRun(const QString &strIP, int iPort)
{
    m_pSocketThread->ConnectSocket(strIP,iPort);
	
	
}

/*********************************************************************************************************
** \brief StopRun()
** \details  关闭socket
** \param argl   IP地址
** \param arg2   端口号
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::StopRun()
{
	m_pCycleCheck->stop();
}

/*********************************************************************************************************
** \brief OnReceive(char *pBuff, int nLen)
** \details 接收网络数据
** \param argl   数据
** \param arg2   长度
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnReceive(char *pBuff, int nLen)
{	
    if ((unsigned)nLen < 0 )
    {
        return false;
    }

    m_pApduRecver->OnReceive(pBuff,nLen);

    return true;
}

/*********************************************************************************************************
** \brief WriteApduData(const char *pSendData, int iLength)
** \details 写入网络数据
** \param argl   数据
** \param arg2   长度
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::WriteApduData(const char *pSendData, int iLength)
{
    return m_pSocketThread->WriteSocketData(pSendData,iLength);
}

C101Sender *CCommPlugin::getSender()
{
	Q_ASSERT(m_pApduSender);
    return m_pApduSender;
}

/*********************************************************************************************************
** \brief Slot_OnRecvData()
** \details 接收网络数据
** \param argl   U格式类型
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_OnRecvData()
{
    //处理接收到的数据

    QByteArray bRecvData = m_pSocketThread->GetNetworkData();
	
    if (!bRecvData.isEmpty())
    {
        OnReceive(bRecvData.data(), bRecvData.length());
    }

}

/*********************************************************************************************************
** \brief Slot_SocketError(QString error)
** \details socket错误
** \param argl   错误原因
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/

void CCommPlugin::Slot_SocketError(QString error)
{
	m_pCycleCheck->stop();

	//清空数据
	m_bIsRunning = false;

    m_pSocketThread->CloseSocket();
 
	m_pModule->m_pMainModule->LogString("FCI",error.toStdString().c_str(),error.length());
}

/*********************************************************************************************************
** \brief Slot_sockeConnectSuccess()
** \details socket连接成功
** \param argl   
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_sockeConnectSuccess(QString strLocalInfo)
{
    m_bIsRunning = true;

	emit Signal_SocketConnectSuccess(strLocalInfo);

	m_pCycleCheck->start();
}


/*********************************************************************************************************
** \brief OnCommand(NBM_TELECTRL* pTelectrl)
** \details 发送召唤信息
** \param argl  召唤结构
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnCommand(NBM_TELECTRL* pTelectrl)
{
	getSender()->OnControl(pTelectrl);
	return true;
}

// 遥控命令
void CCommPlugin::SetControlBckCommand(int iControlType, int ipointNum, int OperateType)
{
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = m_pModule->GetDeviceAddr();
	telectrl.m_nCtrlType = TELECTRL_REQUEST_EXECUTE;

	telectrl.m_nDataID = ipointNum;
	if (OperateType == 0)
	{
		//合
		telectrl.m_fValue = 1;
	}
	else
	{
		//分
		telectrl.m_fValue = 0;
	}

	if (iControlType == 0)
	{
		//单点遥控
		telectrl.m_nCommandType = C_SC_NA_1;
	}
	else
	{
		//双点遥控  1
		telectrl.m_nCommandType = C_DC_NA_1;
	}

	bool IsOK = OnCommand(&telectrl);

	if (!IsOK)
	{
		return;
	}
}

// 遥控命令
/*********************************************************************************************************
** \brief Slot_SetControlCommand(int iControlType, int ipointNum, int OperateType)
** \details 遥控命令
** \param iControlType
** \param ipointNum
** \param OperateType
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_SetControlCommand(int iControlType, int ipointNum, int OperateType,int nMessType)
{
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = m_pModule->GetDeviceAddr();
    if (nMessType == 1)
    {
        //预置
        telectrl.m_nCtrlType = TELECTRL_REQUEST_SELECT;
    }
    else if (nMessType == 2)
    {
        //执行
        telectrl.m_nCtrlType = TELECTRL_REQUEST_EXECUTE;
    }
	else if (nMessType == 3)
	{
		//撤销
		telectrl.m_nCtrlType = TELECTRL_REQUEST_UNSELECT;

	}
	

	telectrl.m_nDataID = ipointNum;
	if (OperateType == 0)
	{
		//合
		telectrl.m_fValue = 1;
	}
	else
	{
		//分
		telectrl.m_fValue = 0;
	}

	if (iControlType == 0)
	{
		//单点遥控
		telectrl.m_nCommandType = C_SC_NA_1;
	}
	else
	{
		//双点遥控  1
		telectrl.m_nCommandType = C_DC_NA_1;
	}

	bool IsOK = OnCommand(&telectrl);

	if (!IsOK)
	{
		return;
	}

}

//定值指令
void CCommPlugin::Slot_setDevOrder(DEV_BASE &devData)
{
	//m_pApduSender->OnSendDevDataRequest(&devData);
}

void CCommPlugin::Slot_SetIecOrder(IEC_BASE &iecData)
{
	//m_pApduSender->OnSendIecDataRequest(&iecData);
}

void CCommPlugin::Slot_SetZoomArea(ZOOM_BASE &zoomBaseInfo)
{
    //m_pApduSender->OnSendZoomDataRequest(&zoomBaseInfo);
}




//t0超时
/*********************************************************************************************************
** \brief Slot_TimeOut0
** \details t0超时
** \param argl  
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_TimeOutT0()
{
	emit Signal_SocketError("trying Open COM.....");

	m_pTimeGeneralSendMsg->stop();
	m_pTimerSyncTimeMsg->stop();
	m_pTimerKwhMsg->stop();


	m_bIsRunning = false;

	this->StopRun();

	this->StartRun("", m_iPort);
}
/*********************************************************************************************************
** \brief Slot_TimeOutT1
** \details t
** \param argl  召唤结构
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_TimeOutT1()
{
	emit Signal_SocketError("trying Open COM2.....");
	//t1超时 认为tcp连接发生错误 重新建立连接
	m_bIsRunning = false;

	this->StopRun();
	//关闭所有的定时器
	m_pTimerOut1->stop();
	m_pTimerOut2->stop();
	m_pTimerOut3->stop();

	m_pTimeGeneralSendMsg->stop();
	m_pTimerSyncTimeMsg->stop();
	m_pTimerKwhMsg->stop();

	this->StartRun("", m_iPort);

}

//t2超时
void CCommPlugin::Slot_TimeOutT2()
{
	
}

//t3超时
void CCommPlugin::Slot_timeOutT3()
{
	
}

//复位进程
void CCommPlugin::Slot_ResetProcess()
{
    NBM_TELECTRL telectrl;
    telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = 0;
    telectrl.m_nCtrlType = TELECTRL_RESET_PROCESSCALL;
    bool IsOK = OnCommand(&telectrl);
    if (!IsOK)
    {
        return;
    }
}

void CCommPlugin::Slot_DisConnect()
{
	StopRun();
}

void CCommPlugin::Slot_AllCallRespond()
{
	emit Signal_AllCallRespond();

	if (!m_pTimerSyncTimeMsg->isActive())
	{
		m_pTimerSyncTimeMsg->start();
		Slot_SendSyncRequestMsg();
	}
}

//切换定值区
void CCommPlugin::Slot_SwitchFixArea(unsigned short uFixArea)
{
	ASDU200 tAsdu200;
	tAsdu200.type = D_FIX_SWITCH;
	tAsdu200.vsq = 0x01;
	tAsdu200.cot.SetCot(COT_ACT);
	tAsdu200.asduAddr.SetAddr(m_pModule->GetDeviceAddr());
	tAsdu200.m_infoaddr.SetAddr(0);
	tAsdu200.m_infAreaIndex.SetAddr(uFixArea);
	//m_pApduSender->OnSendFixAreaCode(&tAsdu200);
}

//读取当前定值区号
void CCommPlugin::Slot_ReadCurrentFixArea()
{
	//m_pApduSender->OnSendFixSearch();
}


void CCommPlugin::Slot_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest)
{
	//m_pApduSender->OnSendGetCatalogRequest(catalogRequest);
}

void CCommPlugin::Slot_WriteAction(FILE_ATTR_INFO &tFileInfo)
{
	//m_pApduSender->m_nStartNode = 0;
	//m_pApduSender->OnSendWriteFileAction(tFileInfo);
}

void CCommPlugin::Slot_ReadAction(FILE_ATTR_INFO &tFileInfo)
{
	//m_pApduSender->OnSendReadFileAction(tFileInfo);
}

void CCommPlugin::slot_OnCkeckList()
{
	C101Sender * pSender = getSender();
	Q_ASSERT(pSender);
	if (!pSender)
	{
		return;
	}
	CDeviceData * pDev = GetFCIModule()->GetDeviceInfo();
	Q_ASSERT (pDev);
	if (pDev == Q_NULLPTR)
	{
		return;
	}

	if (pDev->m_bIsHalt )
	{
		if (m_pResendTimer->elapsed()< 5000)
		{
			return;
		}	
		else
		{
			m_pModule->m_pMainModule->LogString("FCI", "device has halt and reset", 100);
			pDev->Reset();
		}
	}
	

	if (pDev->canSend())
	{
		if (pSender->OnSend())
		{
			pDev->m_nRunState = DEVICE_RUN_STATE_SEND;
		}
	}	
	else 
	{
		if (pDev->m_bIsHalt)
		{
			//装置错误超过三次，休息5s，然后重发启动帧
			m_pResendTimer->restart();
		}
	}
}

//总召超时
void CCommPlugin::Slot_SendGeneralResquestMsg()
{
	//启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
	//召唤全数据
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
	bool IsOK = OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}
//	m_pTimerSyncTimeMsg->stop();
}

//时间对时
void CCommPlugin::Slot_SendSyncRequestMsg()
{
    //启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
    //召唤全数据
    NBM_TELECTRL telectrl;
    telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = 0;

    //对时请求
    telectrl.m_nCtrlType = TELECTRL_REQUEST_SYNCTIME;
    bool IsOK = OnCommand(&telectrl);

    if (!IsOK)
    {
        return;
    }
}

void CCommPlugin::slot_RdTimeRequest()
{
	CDeviceData* pDeviceData = GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return ;
	}
	char buf[BUFFER_SIZE_101];

	//链路控制域
	AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
	pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
	pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB;
	pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
	pHead->m_ctrl.m_FCV = 1;
	pHead->m_ctrl.m_FUNCCODE = 3;
	//链路地址
	pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	//组织ASDU103
	ASDU103* pAsdu103 = (ASDU103*)((char*)buf + sizeof(AVDU_HEAD));

	pAsdu103->type = C_CS_NA_1;
	pAsdu103->vsq = 0x01;

	pAsdu103->cot.SetCot(COT_REQ);
	pAsdu103->asduAddr.SetAddr(pDeviceData->m_nDeviceAddr);
	pAsdu103->m_infoaddr.SetAddr(0x00);

	struct CP56Time2a* cp56time2a = (struct CP56Time2a*)(&(pAsdu103->m_time));

	QDateTime tvTime = QDateTime::currentDateTime();
	cp56time2a->Initialize(tvTime.date().year(), tvTime.date().month(), tvTime.date().day(), tvTime.time().hour(), tvTime.time().hour(), tvTime.time().second(), tvTime.time().msec());

	getSender()->Send_V((char*)(buf), sizeof(ASDU103));
}

void CCommPlugin::slot_TcCommand()
{
	CDeviceData* pDeviceData = GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return;
	}
	char buf[BUFFER_SIZE_101];

	//链路控制域
	AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
	pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
	pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB;
	pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
	pHead->m_ctrl.m_FCV = 1;
	pHead->m_ctrl.m_FUNCCODE = 3;
	//链路地址
	pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	//组织ASDU104
	ASDU104* pAsdu104 = (ASDU104*)((char*)buf + sizeof(AVDU_HEAD));

	pAsdu104->type = C_CS_NA_1;
	pAsdu104->vsq = 0x01;

	pAsdu104->cot.SetCot(COT_REQ);
	pAsdu104->asduAddr.SetAddr(pDeviceData->m_nDeviceAddr);
	pAsdu104->m_infoaddr.SetAddr(0x00);

	pAsdu104->m_fbp.SetCOI(0x55AA);

	getSender()->Send_V((char*)(buf), sizeof(ASDU104));
}

void CCommPlugin::Slot_SendKwhTimeRequestMsg()
{
    //启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
    //召唤全数据
    NBM_TELECTRL telectrl;
    telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = 0;

    telectrl.m_nCtrlType = TELECTRL_REQUEST_KWHGENERALCALL;
    bool IsOK = OnCommand(&telectrl);
    if (!IsOK)
    {
        return;
    }
}


void CCommPlugin::Slot_SetRecordOrder(QList<LB_DATA> &lbData)
{
    if (lbData.count() > 0)
    {
        //m_pApduSender->SetLbAllInfo(lbData);
        //m_pApduSender->OnSendRecordCatalog(lbData.at(0));
    }
}

CFtuModule * CCommPlugin::GetFCIModule()
{
    return m_pModule;
}

void CCommPlugin::SetGeneralTime(int nGeneralTime, int nSynTime,int nKwhTime)
{
   /* m_pTimeGeneralSendMsg->setInterval(nGeneralTime * 60 * 1000);
    m_pTimerSyncTimeMsg->setInterval(nSynTime * 60 * 1000);
    m_pTimerKwhMsg->setInterval(nKwhTime * 60 * 1000);*/
}

void CCommPlugin::SetTimerx(int nTime0, int nTime1, int nTime2, int nTime3)
{
/*
	m_pTimerOut0->setInterval(nTime0);
	m_pTimerOut1->setInterval(nTime1);
	m_pTimerOut2->setInterval(nTime2);
	m_pTimerOut3->setInterval(nTime3);*/
}

void CCommPlugin::Slot_SetSycsTime(QDateTime tSycsTime)
{
	CDeviceData* pDeviceData = GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return ;
	}
	char buf[BUFFER_SIZE_101];

	//链路控制域
	AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
	pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
	pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB;
	pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
	pHead->m_ctrl.m_FCV = 1;
	pHead->m_ctrl.m_FUNCCODE = 3;
	//链路地址
	pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	//组织ASDU103
	ASDU103* pAsdu103 = (ASDU103*)((char*)buf + sizeof(AVDU_HEAD));

	pAsdu103->type = C_CS_NA_1;
	pAsdu103->vsq = 0x01;

	pAsdu103->cot.SetCot(COT_ACT);
	pAsdu103->asduAddr.SetAddr(pDeviceData->m_nDeviceAddr);
	pAsdu103->m_infoaddr.SetAddr(0x00);

	struct CP56Time2a* cp56time2a = (struct CP56Time2a*)(&(pAsdu103->m_time));

	QDateTime tvTime = tSycsTime;
	cp56time2a->Initialize(tvTime.date().year(), tvTime.date().month(), tvTime.date().day(), tvTime.time().hour(), tvTime.time().hour(), tvTime.time().second(), tvTime.time().msec());

	getSender()->Send_V((char*)(buf), sizeof(ASDU103));

}
