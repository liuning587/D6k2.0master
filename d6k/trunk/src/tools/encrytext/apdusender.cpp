#include "apdusender.h"
#include "socketthread.h"
#include "asdu_info.h"
#include "XAHost.h"
#include "logcache.h"

CApduSender::CApduSender(QObject *parent /*= 0*/)
{
	m_pSendThread = new CSocketThread(this);
	m_nDevicePort = 0;
	m_bDeviceConnectFlag = false;

	connect(m_pSendThread, SIGNAL(Signal_ConnectSuccess(QString)), this, SLOT(Slot_DeviceConnectScuess(QString)));
}

CApduSender::~CApduSender()
{

}

CSocketThread * CApduSender::GetSendSocket()
{
	return m_pSendThread;
}

void CApduSender::ConnectToDevice()
{
	m_bDeviceConnectFlag = false;
	m_pSendThread->ConnectSocket(m_strDeviceIp, m_nDevicePort);
}

void CApduSender::SetSenderIp(const QString & strIp)
{
	m_strDeviceIp = strIp;
}

void CApduSender::SetSenderPort(int nPort)
{
	m_nDevicePort = nPort;
}

bool CApduSender::GetDeviceConnectFlag()
{
	return m_bDeviceConnectFlag;
}



void CApduSender::Slot_DeviceConnectScuess(QString strScuessInfo)
{
	m_bDeviceConnectFlag = false;
	//发送数据 发送随机数
	char pBuff[500];
	ASDU_ENCODE *pHeader = (ASDU_ENCODE *)pBuff;
	pHeader->m_nStart = 0xEB;
	pHeader->m_nStart2 = 0xEB;

	pHeader->m_nType0 = 0;
	pHeader->m_nType1.D7 = 0;
	pHeader->m_nType1.D6 = 0;
	pHeader->m_nType1.D5 = 0;
	pHeader->m_nType1.D4 = 0;
	pHeader->m_nType1.D3 = 0;
	pHeader->m_nType1.D2 = 0;
	pHeader->m_nType1.D1 = 0;
	pHeader->m_nType1.D0 = 0;
	//报文长度
	pHeader->m_nLength.SetAddr(13);

	pBuff[sizeof(ASDU_ENCODE)] = 0x50;

	ASDUADDR2 *pAddr = (ASDUADDR2*)(pBuff + sizeof(ASDU_ENCODE)+1);
	//8字节
	pAddr->SetAddr(8);
	//获取随机数
	CXAHost::GetInstance()->GenRand(8, (unsigned char*)(pBuff+sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2)));

	CXAHost::GetInstance()->SetOrignialVer(pBuff + sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2));

	unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pBuff+4), 13);

	pBuff[sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2) + 8] = pCheckData;
	pBuff[sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2) + 8 + 1] = 0xD7;

	//qDebug() << QByteArray(pBuff, sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2) + 8 + 1 + 1).toHex().toUpper().toStdString().c_str();
	//发送数据
	m_pSendThread->WriteSocketData(pBuff, sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2) + 8 + 1 + 1);

	CLogCache::InstanceCache()->InsertLog(tr("中间件发送双向认证数据(随机数)至装置:%1").arg(QByteArray(pBuff, sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2) + 8 + 1 + 1).toHex().toUpper().toStdString().c_str()));

}
