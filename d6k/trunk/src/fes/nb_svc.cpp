/*! @file nb_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  nb_svc.cpp
文件实现功能 :  网络接口模块
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   网络接口模块
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "nb_svc.h"
#include "db_svc.h"
#include "fes.h"
#include "log/log.h"
#include "mail/mail.h"
#include "netbus/nbdef.h"
#include "netbus/nbapi.h"
#include "stl_util-inl.h"
#include "rtnet_msg_packer.h"

#include "message_executer.h"

#include "red_task.h"

#include <QObject> 
#include <QString> 
#include <QHostAddress>
#include <vector>
#include <memory>

CNetbusSvc::CNetbusSvc(CFesSvc* pServer, const std::string& szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	m_pNodes = nullptr;
	m_nNodeCount = 0;
	m_pNodeConfigs = nullptr;
	m_pNetConfig = std::make_shared<NET_CONFIG>();
	m_pBuf = new EMSG_BUF;
}

CNetbusSvc::~CNetbusSvc(void)
{
	Shutdown();

	if (m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = nullptr;
	}
	if (m_pNodeConfigs)
	{
		delete[] m_pNodeConfigs;
		m_pNodeConfigs = nullptr;
	}
}
/*! \fn bool CNetbusSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbgSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CNetbusSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{	
	CFesSvc *pSvr = GetFesSvc();
	Q_ASSERT(pSvr);
	if (pSvr)
	{
		std::shared_ptr<CDbSvc>& pDBSvc = pSvr->GetDBSvc();
		Q_ASSERT(pDBSvc);
		if (pDBSvc == nullptr)
			return false;

		m_pNetMsgPacker = std::make_shared<CRTNetMsgPacker<CDbSvc>>(pDBSvc);

		int32u nMyNodeOccNo =  pDBSvc->GetMyNodeOccNo();
		Q_ASSERT(nMyNodeOccNo > INVALID_OCCNO && nMyNodeOccNo < MAX_NODE_OCCNO);
		if (nMyNodeOccNo == INVALID_OCCNO || nMyNodeOccNo > MAX_NODE_OCCNO)
		{
			return false;
		}
		m_pNetConfig->MyNodeOccNo = nMyNodeOccNo;
		// 判断本节点是否为双机冗余节点
		NODE *pMyNode = pDBSvc->GetNodeByOccNo(nMyNodeOccNo);
		Q_ASSERT(pMyNode);
		if (pMyNode)
		{
			m_bIsRedNode = (pMyNode->SlaveOccNo != INVALID_OCCNO && pMyNode->SlaveOccNo <= MAX_NODE_OCCNO) ? true : false;
		}		

		std::tie(m_nNodeCount, m_pNodes) = pDBSvc->GetNodeData();
		Q_ASSERT(m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO);
		Q_ASSERT(m_pNodes);
		m_pNetConfig->NodeCount = static_cast<int32u> (m_nNodeCount);
	//	m_pNetConfig->IsDoubleNet = true;
		if (m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO && m_pNodes)
		{
			Q_ASSERT(m_pNodeConfigs == nullptr);
			m_pNodeConfigs = new NODE_CONFIG[m_nNodeCount];
			std::memset(m_pNodeConfigs, 0, sizeof(NODE_CONFIG)*m_nNodeCount);

			// 从共享内存中拷贝数据出来
			size_t i = 0;
			for (i = 0; i < m_nNodeCount; i++)
			{// TODO
				m_pNodeConfigs[i].OccNo = m_pNodes[i].OccNo;
				m_pNodeConfigs[i].SlaveOccNo = m_pNodes[i].SlaveOccNo;
				m_pNodeConfigs[i].NodeType = m_pNodes[i].NodeType;
				m_pNodeConfigs[i].NetAIPAddr = m_pNodes[i].NetAIPAddr;
				m_pNodeConfigs[i].NetBIPAddr = m_pNodes[i].NetBIPAddr;
				memset(m_pNodeConfigs[i].chIP[0], 0, sizeof m_pNodeConfigs[i].chIP[0]);
				strncpy(m_pNodeConfigs[i].chIP[0], QHostAddress(m_pNodes[i].NetAIPAddr).toString().toStdString().c_str(), 
							qMin(NAME_SIZE + 1, QHostAddress(m_pNodes[i].NetAIPAddr).toString().length()));

				memset(m_pNodeConfigs[i].chIP[1], 0, sizeof m_pNodeConfigs[i].chIP[1]);
				strncpy(m_pNodeConfigs[i].chIP[1], QHostAddress(m_pNodes[i].NetBIPAddr).toString().toStdString().c_str(),
					qMin(NAME_SIZE + 1, QHostAddress(m_pNodes[i].NetBIPAddr).toString().length()));
			
				m_pNodeConfigs[i].HostSlave = m_pNodes[i].State;

				strncpy(m_pNodeConfigs[i].HostName, m_pNodes[i].HostName, NAME_SIZE);

				m_pNodeConfigs[i].SlaveOccNo = m_pNodes[i].SlaveOccNo;

				m_pNodeConfigs[i].CardStatus[0] = m_pNodes[i].Net1State;

				m_pNodeConfigs[i].CardStatus[1] = m_pNodes[i].Net2State;
			}

			m_pNetConfig->pNodeConfig = m_pNodeConfigs;

			if (m_bIsRedNode)
			{// 如果是主从冗余节点，则启动冗余任务
				if (m_pRedTask != nullptr)
				{
					m_pRedTask.reset(new (CRedTask)(pDBSvc));
				}
				else
				{
					m_pRedTask = std::make_shared<CRedTask>(pDBSvc);
				}
			}

			// 启动网络总线模块
			return StartNetBus(m_pNetConfig.get(), pszDataPath, "FES", NODE_FES, nMode);
		}
	}

	return false;
}

void CNetbusSvc::Run()
{
	StartModule();

	if (m_bIsRedNode)
	{// 如果是主从冗余节点，则启动冗余任务
		if (m_pRedTask != nullptr)
		{
			m_pRedTask->Start();
		}
	}
}

void CNetbusSvc::Shutdown()
{
	if (m_bIsRedNode)
	{// 如果是主从冗余节点，则启动冗余任务
		if (m_pRedTask != nullptr)
		{
			m_pRedTask->Stop();
		}		 
	}

	StopNetBus("FES");
	StopModule();
}
/*! \fn void  CNetbusSvc::MainLoop()
********************************************************************************************************* 
** \brief CNetbusSvc::MainLoop 
** \details 在主任务中调用
** \return void 
** \author LiJin 
** \date 2016年10月21日 
** \note 1.转发内部邮件到以太网总线上， 2.接收以太网下行的报文，3.更新节点状态
********************************************************************************************************/
void  CNetbusSvc::MainLoop()
{
	RecvNetData();
//!<1>先检测邮件
	TransEmails();
//!<2>数据同步，策略待定，当前是全库同步
	TransDataInfo();
//!<3>定时更新节点状态
	UpdateNetState();	
//!<4> 处理主从同步
	RedBackupSvc();
}

/*! \fn void CNetbusSvc::TransEmails()
********************************************************************************************************* 
** \brief CNetbusSvc::TransEmails 
** \details 收到的所有邮件 进行转发至内部总线，供其他模块接收 
** \return void 
** \author xingzhibing
** \date 2017年3月14日 
** \note  当前处理逻辑是 一直接收 直到收不到 执行下一步数据同步
********************************************************************************************************/
void CNetbusSvc::TransEmails()
{
	DMSG msg;
	msg.RecverID = m_nMailBoxID;

	static unsigned int nCount = 0;

	//! 建议先接收本地所有的邮件，然后统一打包一次发送，避免小数据包
	//! 报文需要进一步压缩，目前报文中无效部分过多。

	while (true)
	{
		bool bRet = false;

		std::memset(&msg, 0, sizeof(DMSG));
		msg.RecverID = m_nMailBoxID;

		bRet = RecvMail("FES", &msg, 0);
		//成功收到一封邮件,收到邮件立即转发到scada中
		if (bRet)
		{
			qDebug() <<"RECVID::"<< msg.RecverID << "SENDID::"<<msg.SenderID;
			memset(m_pBuf, 0, EMSG_BUF_SIZE);

			m_pBuf->MsgPath = TO_SERVER;
			m_pBuf->MsgType = 1;

			if (msg.Type == MSG_EVT_RT_SETVAL)
			{
				m_pBuf->FuncCode = COT_RTSETVAL;
			}
			else
			{
				m_pBuf->FuncCode = COT_ALARM;
			}
			
			m_pBuf->SrcOccNo=m_pNetConfig->MyNodeOccNo;

			int32u nSize = EMSG_BUF_HEAD_SIZE + sizeof DMSG;
			m_pBuf->MsgDataSize = nSize;

			memcpy(m_pBuf->BuffData,&msg,sizeof DMSG);

			int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);

			if (!nRet)
			{
				//TODO LOG
			}
			nCount = 0;
		}
		else
		{			
			nCount++;
			std::chrono::milliseconds timespan(10);
			std::this_thread::sleep_for(timespan);
		}

		//运行尝试多接收几次 查看是否有邮件继续接收
		if (nCount>=3)
		{
			nCount = 0;
			return;
		}
	}
}
/*! \fn void CNetbusSvc::RecvNetData()
********************************************************************************************************* 
** \brief CNetbusSvc::RecvNetData 
** \details 接收下行的报文
** \return void 
** \author xingzhibing 
** \date 2017年3月22日 
** \note 
********************************************************************************************************/
void CNetbusSvc::RecvNetData()
{
	int8u EMsgBuff[MAX_EMSG_L *2] = { 0 };

	int32u nCopySize = 0;

	bool bRet = NBRecvData("FES", EMsgBuff, MAX_EMSG_L, &nCopySize, 0);

	if (bRet)
	{
		EMSG_BUF* pEMsg = (EMSG_BUF*)EMsgBuff;
		
		if (pEMsg)
		{
			switch (pEMsg->FuncCode)
			{
				case  COT_SETVAL:
				{// 设值信文-直接转发到DB_SVC
					DMSG * pMailMsg = reinterpret_cast<DMSG*>(pEMsg->BuffData);

					auto fn = [this] (DMSG*pLocal)->bool
					{
						Q_ASSERT(pLocal);
						if (pLocal)
						{
							Q_ASSERT(GetFesSvc()->GetDBSvc()->GetMailBoxID() != 0);

							pLocal->RecverID = GetFesSvc()->GetDBSvc()->GetMailBoxID();
							bool bRet = SendMail("FES", pLocal, 0);
							return bRet;
						}
						return false;
					};

					CMessageExecutor MsgExer(pMailMsg);
					bRet = MsgExer.Run(fn);


#if 0
					SETVAL_MSG * pMsg = (SETVAL_MSG*)(pMailMsg->Buff);

					switch (pMsg->IddType)
					{
						case IDD_AIN:
							break;
						case IDD_DIN:
							break;
						case IDD_AOUT:
						{
							GetFesSvc()->GetDBSvc()->FesSetAoutValue(pMsg->Occno, V_FLOAT(pMsg->Value[0]), 0);
							break;
						}
						case IDD_DOUT:
						{
							GetFesSvc()->GetDBSvc()->FesSetAoutValue(pMsg->Occno, V_CHAR(pMsg->Value[0]), 0);
							break;
						}
						default:
							break;
					}

#endif
					break;
				}
				default:
					break;
			}
		}
	}
}

/*! \fn void CNetbusSvc::TransDataInfo()
********************************************************************************************************* 
** \brief CNetbusSvc::TransDataInfo 
** \details 传输数据包 策略待定 当前全数据居同步
** \return void 
** \author xingzhibing
** \date 2017年3月14日 
** \note 
********************************************************************************************************/
void CNetbusSvc::TransDataInfo()
{
	SendDinToSvr();
	SendAinToSvr();
	SendUserVarToSvr();
	SendSysVarToSvr();
	SyncChannelInfo();
	SyncDeviceInfo();
}

/*! \fn void CNetbusSvc::UpdateNetState()
********************************************************************************************************* 
** \brief CNetbusSvc::UpdateNetState 
** \details 更新节点状态，定时从网络总线中获取系统中各个节点的主从状态
** \return void 
** \author LiJin 
** \date 2016年10月22日 
** \note 
********************************************************************************************************/
void CNetbusSvc::UpdateNetState()
{
	// 更新前，需要判断下 实时内存库是否已经退出
	CFesSvc *pSvr = GetFesSvc();
	Q_ASSERT(pSvr);
	if (pSvr)
	{
		std::shared_ptr<CDbSvc> pDBSvc = pSvr->GetDBSvc();
		Q_ASSERT(pDBSvc);
		if (pDBSvc)
		{
			if (pDBSvc->IsDBAlive(0))
			{
				Q_ASSERT(m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO);
				Q_ASSERT(m_pNodes);

				if (m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO && m_pNodes && m_pNodeConfigs && m_pNetConfig)
				{
					unsigned int i = 0;
					int32u nOccNo = 0;
					for (i = 0; i < m_nNodeCount; i++)
					{
						nOccNo = m_pNodeConfigs[i].OccNo;
						Q_ASSERT(m_pNodeConfigs[i].OccNo > INVALID_OCCNO && m_pNodeConfigs[i].OccNo <= MAX_NODE_OCCNO);
						if (m_pNodeConfigs[i].OccNo > INVALID_OCCNO && m_pNodeConfigs[i].OccNo <= MAX_NODE_OCCNO)
						{
							m_pNodes[nOccNo - 1].State = m_pNodeConfigs[i].HostSlave;
							m_pNodes[nOccNo - 1].Net1State = m_pNodeConfigs[i].CardStatus[0];
							m_pNodes[nOccNo - 1].Net2State = m_pNodeConfigs[i].CardStatus[1];
						}
					}
				}
			}
		}
	
	}	
}
#if 0
/*! \fn size_t CNetbusSvc::PackageAllRTData(std::shared_ptr<CDbSvc> pDB, unsigned char *pInBuff, size_t nBuffLen)
********************************************************************************************************* 
** \brief CNetbusSvc::PackageAllRTData 
** \details 获取全部的测值信息（AI\DI\...）
** \param pDB 
** \param pInBuff 
** \param nBuffLen 
** \return size_t 
** \author LiJin 
** \date 2016年11月29日 
** \note 
********************************************************************************************************/
size_t CNetbusSvc::PackageAllRTData(std::shared_ptr<CDbSvc> pDB, unsigned char *pInBuff, size_t nBuffLen)
{
	Q_ASSERT(pInBuff);
	if (pInBuff == nullptr)
		return 0;
	Q_ASSERT(pDB);
	if (pDB == nullptr)
		return 0;

	if (pDB->IsDBAlive(0))
	{

	}


	return 0;
}

#endif
 

void  CNetbusSvc::SendDinToSvr()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//第一个pair 为起始OccNo 第二个为元素数目
	std::vector<std::pair<int32u, int32u > > arrDiNums;
	m_pNetMsgPacker->GetDIPackageNum(arrDiNums);
	
	int32u nSize;

	for (auto iter : arrDiNums)
	{
		m_pNetMsgPacker->PackageAllDI(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}


void CNetbusSvc::SendAinAlarmData(EMSG_BUF *pEMsgBuf)
{
	Q_ASSERT(pEMsgBuf);
	if (pEMsgBuf == nullptr)
		return;

	size_t nNeedLen = 0;
	size_t nCopyNum = 0;
	// 计算下需要多大缓冲区
	for ( auto it : m_arrAinAlarmMsg)
	{
		Q_ASSERT(it->Len <= sizeof(AINALARM_MSG));
		nNeedLen += it->Len;
		if (nNeedLen > MAX_EMSG_L)
		{
			nNeedLen -= it->Len;
			break;
		}
		nCopyNum++;
	}

	if (nNeedLen == 0)
		return;


/*
	AIN_ALARM_DATA *pData = reinterpret_cast<AIN_ALARM_DATA*>(pEMsgBuf->MsgData);
	pData->DataCount = nCopyNum;
	pData->DataTotalLen = nNeedLen;

	size_t i = 0;
	for (auto it : m_arrAinAlarmMsg)
	{

	}
*/


	int nRet = NBSendData("FES", reinterpret_cast<int8u*>(pEMsgBuf), 100);
	// 如果发送不成功，则log？log会不会塞爆？

}

void CNetbusSvc::SendAinToSvr()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath  = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType  = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//第一个pair 为起始OccNo 第二个为元素数目
	std::vector<std::pair<int32u, int32u > > arrAiNums;
	m_pNetMsgPacker->GetAIPackageNum(arrAiNums);

	int32u nSize;

	for (auto iter : arrAiNums)
	{
		m_pNetMsgPacker->PackageAllAI(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}

void CNetbusSvc::SendUserVarToSvr()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//第一个pair 为起始OccNo 第二个为元素数目
	std::vector<std::pair<int32u, int32u > > arrUserVarNums;
	m_pNetMsgPacker->GetUserVarPackageNum(arrUserVarNums);

	int32u nSize;

	for (auto iter : arrUserVarNums)
	{
		m_pNetMsgPacker->PackageAllUserVarInfo(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}

void CNetbusSvc::SendSysVarToSvr()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//第一个pair 为起始OccNo 第二个为元素数目
	std::vector<std::pair<int32u, int32u > > arrSysVarNums;
	m_pNetMsgPacker->GetSysVarPackageNum(arrSysVarNums);

	int32u nSize;

	for (auto iter : arrSysVarNums)
	{
		m_pNetMsgPacker->PackageAllSysVarInfo(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}

void CNetbusSvc::SyncDeviceInfo()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//第一个pair 为起始OccNo 第二个为元素数目
	std::vector<std::pair<int32u, int32u > > arrDeviceNums;
	m_pNetMsgPacker->GetDevicePackageNum(arrDeviceNums);

	int32u nSize;

	for (auto iter : arrDeviceNums)
	{
		m_pNetMsgPacker->PackageSyncDeviceInfo(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}

void CNetbusSvc::SyncChannelInfo()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//第一个pair 为起始OccNo 第二个为元素数目
	std::vector<std::pair<int32u, int32u > > arrChannelNums;
	m_pNetMsgPacker->GetChannelPackageNum(arrChannelNums);

	int32u nSize;

	for (auto iter : arrChannelNums)
	{
		m_pNetMsgPacker->PackageSyncChannelInfo(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}
void CNetbusSvc::RedBackupSvc()
{
	if (m_bIsRedNode == false)
		return;

	if (m_pRedTask == nullptr)
		return;

	CFesSvc *pSvr = GetFesSvc();
	Q_ASSERT(pSvr);
	if (pSvr == nullptr)
		return;

	std::shared_ptr<CDbSvc> pDBSvc = pSvr->GetDBSvc();
	Q_ASSERT(pDBSvc);
	if (pDBSvc)
	{
		if (pDBSvc->IsDBAlive(0))
		{
			if (pDBSvc->GetMyHostState() == STATE_MAIN)
			{

			}
			else if (pDBSvc->GetMyHostState() == STATE_SLAVE)		 
			{

			}
		}
	}

	
}

/** @}*/
