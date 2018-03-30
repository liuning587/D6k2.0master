#include "apdurecver.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include "socketthread.h"
#include "asdu_info.h"
#include "define_104.h"
#include "apdusender.h"
#include "singlecontrol.h"
#include "logcache.h"
#include "apdusender.h"
#include "XAHost.h"


/*********************************************************************************************************
** \brief CApduRecver(QObject *parent)
** \details 构造函数
** \param argl   父类指针
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
CApduRecver::CApduRecver(QObject *parent):
    QObject(parent)
{
	m_pTcpServer = new QTcpServer(this);
	m_pSender = nullptr;

	m_pzMidTcp = nullptr;

	//StartTcpServer();
}


void CApduRecver::StartTcpServer()
{
	bool tReslut = m_pTcpServer->listen(QHostAddress::Any, 2600); //监听任何连上2600端口的ip  

	connect(m_pTcpServer, SIGNAL(newConnection()), this, SLOT(Slot_NewConnect())); 
	
 	if (tReslut)
 	{
		CLogCache::InstanceCache()->InsertLog(tr("服务端监听成功,IP:all,端口:%2").arg(m_pTcpServer->serverPort()));
	}
 	else
 	{
		CLogCache::InstanceCache()->InsertLog(tr("服务端监听失败,IP:all,端口:%2,失败原因:%3").arg(m_pTcpServer->serverAddress().toString()).arg(m_pTcpServer->serverPort()).arg(m_pTcpServer->errorString()));
 	}
	
}

void CApduRecver::Slot_NewConnect()
{
	//主站连接成功后自动清空数据
	m_104Cache.Clear();
	m_104DeviceCache.Clear();

	m_pzMidTcp = m_pTcpServer->nextPendingConnection(); //得到每个连进来的socket  
	connect(m_pzMidTcp, SIGNAL(readyRead()), this, SLOT(Slot_ReadMessage())); //有可读的信息，触发读函数槽  

	CLogCache::InstanceCache()->InsertLog(tr("主站连接成功,IP:%1").arg(m_pzMidTcp->peerAddress().toString()));

	//开始连接装置
	//m_pSender->ConnectToDevice();
}


void CApduRecver::Slot_ReadMessage()
{
	QTcpSocket* pTcpsocket = qobject_cast<QTcpSocket*>(sender()); //读取  

	Q_ASSERT(pTcpsocket);
	if (pTcpsocket == nullptr)
	{
		return;
	}

	//if (!m_pSender->GetDeviceConnectFlag())
	//{
	//	m_btArray.append(pTcpsocket->readAll());
	//	return;
	//}

	m_btArray.append(pTcpsocket->readAll());

	QByteArray btArray = m_btArray; //读取  

	OnReceive(m_btArray.data(), m_btArray.length());

	CLogCache::InstanceCache()->InsertLog(tr("收到主站数据:%1").arg(QString(m_btArray.toHex().toStdString().c_str())));
	m_btArray.clear();
}


/*********************************************************************************************************
** \brief OnReceive(char *pBuff, int nLen)
** \details 接收网络数据 进行解析
** \param argl   父类指针
** \param arg2
** \return bool   处理成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduRecver::OnReceive(char *pBuff, int nLen)
{
    if(nLen <=0)
    {
        return false;
    }

	qDebug() << QByteArray(pBuff, nLen).toHex();

    if(!m_104Cache.IsEmpty())
    {
        if(m_104Cache.GetCurByteLength() == 1)//只有一个启动字符
        {
            m_104Cache.CopyData(pBuff,1);//复制长度域
            pBuff++;
            nLen--;
        }

        int nNeedBytes = m_104Cache.GetNeedByteLength();
        if(nNeedBytes <= 0)
        {
            return false;
        }

        if(nNeedBytes > nLen)//需要的长度大于接收到的长度
        {
            m_104Cache.CopyData(pBuff,nLen);
            return true;
        }
        else//接收到的长度大于需要的长度
        {
            //复制需要的部分
            m_104Cache.CopyData(pBuff,nNeedBytes);
            pBuff += nNeedBytes;
            nLen -= nNeedBytes;

            bool ISOK = AnalyseApdu(m_104Cache.GetDataBuff(),m_104Cache.GetDataLen());
            m_104Cache.Clear();
            if(!ISOK)
            {
				return false;
            }

            //解析剩余的部分
            return OnReceive(pBuff,nLen);
        }
    }
    else//缓冲区为空
    {
        //在接收区寻找启动字符
        char cStartChar = STARTCHAR;
        char* pNewBuff = std::find(pBuff,pBuff+nLen,cStartChar);

        if(pNewBuff == pBuff+nLen)
            return false;//没有找到启动字符

        nLen = nLen - (pNewBuff - pBuff);
        pBuff = pNewBuff;

        //复制启动字符至缓冲区
        m_104Cache.CopyData(pBuff,1);
        pBuff += 1;
        nLen  -= 1;

        return OnReceive(pBuff,nLen);
    }

    return true;
}

/*********************************************************************************************************
** \brief AnalyseApdu(char *pBuff, int nLength)
** \details 解析应用协议数据单位数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return bool   处理成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduRecver::AnalyseApdu(char *pBuff, int nLength)
{

    if((pBuff==NULL) || (nLength<sizeof(APCI)) || (pBuff[0] != STARTCHAR))
    {
        return false;
    }
	//用于做统计
    QByteArray btData(pBuff, nLength);

    APCI* pApci = (APCI*)(pBuff);
    int nType = pApci->GetType();

    if( (nType != TYPE_I) && (nType != TYPE_U) &&  (nType != TYPE_S) )
    {
        return false;
    }

    switch(nType)
    {
    case TYPE_I:
    {
        if(!AnalyseI(pBuff,nLength))
        {
            return false;
        }
    }
        break;
    case TYPE_U:
    {
        if(!AnalyseU(pBuff,nLength))
        {
            return false;
        }
    }
     break;
    case TYPE_S:
    {
        //
		if (!AnalyseS(pBuff,nLength))
		{
			return false;
		}
    }
        break;
    default:
        break;
    }

    return true;
}

bool CApduRecver::AnalyseDeviceApdu(char * pBuff, int nLength)
{
	//直接发送数据
	m_pzMidTcp->write(pBuff, nLength);
	//发送给主站的数据
	CLogCache::InstanceCache()->InsertLog(tr("收到装置侧的数据:%1").arg(QByteArray(pBuff,nLength).toHex().toStdString().c_str()));

	return true;
}

bool CApduRecver::AnalyseI(char *pBuff, int nLength)
{
    //解析数据
    AnalyseAsdu(pBuff,nLength);
    return true;
}

/*********************************************************************************************************
** \brief AnalyseAsdu(char *pBuff, int nLength)
** \details 解析应用服务数据单元数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return bool   处理成功失败标识
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::AnalyseAsdu(char *pBuff, int nLength)
{
    ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff+sizeof(APCI));


	//加密  组包
	//发送数据 发送随机数
	char pDeCodeBuffAll[2048];
	ASDU_ENCODE *pHeader = (ASDU_ENCODE *)pDeCodeBuffAll;
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

	//应用数据区
	int nBuffLen = 0;
	int nEncodeResult = CXAHost::GetInstance()->EnData(nLength, (unsigned char*)pBuff, (unsigned char*)pDeCodeBuffAll[sizeof(ASDU_ENCODE) + 1 + 1], &nBuffLen);

	unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pDeCodeBuffAll+4), 2 + 1 + 1 + nBuffLen + 2);

	pDeCodeBuffAll[sizeof(ASDU_ENCODE) + 1 + 1 + nBuffLen + 2] = pCheckData;
	pDeCodeBuffAll[sizeof(ASDU_ENCODE) + 1 + 1 + nBuffLen + 3] = 0xD7;

	pHeader->m_nLength.SetAddr(2 + 1 + 1 + nBuffLen + 2);

	pDeCodeBuffAll[sizeof(ASDU_ENCODE)] = 0x00;

	pDeCodeBuffAll[sizeof(ASDU_ENCODE) + 1] = nBuffLen;

	ASDUADDR2 *pAddr = (ASDUADDR2*)(pDeCodeBuffAll + sizeof(ASDU_ENCODE) + 1 + 1 + nBuffLen);
	pAddr->SetAddr(0);

	//发送数据
	m_pSender->GetSendSocket()->WriteSocketData(pDeCodeBuffAll, sizeof(ASDU_ENCODE) + 1 + sizeof(ASDUADDR2) + nBuffLen + 1 + 1 + 1);

	CLogCache::InstanceCache()->InsertLog(tr("中间件发送数据(加密后)至装置:%1").arg(QByteArray(pDeCodeBuffAll, sizeof(ASDU_ENCODE) + 1 + 1 + nBuffLen + 2 + 1 + 1).toHex().toStdString().c_str()));

	return;

	switch (pAsduBase->type)
	{
		//遥信
	case M_SP_NA_1: // 单点信息  ASDU1
	{
		OnRecvBinarySinglePoint(pBuff, nLength);
		break;
	}
	case M_DP_NA_1://双点信息 ASDU3
	{
		OnRecvBinaryDoublePoint(pBuff, nLength);
		break;
	}
	case M_PS_NA_1: // 成组单点信息   ASDU20
	{
		OnRecvBinaryGroupPoint(pBuff, nLength);
		break;
	}
	case M_SP_TA_1://带短时标的单点信息 ASDU2   SOE    M_SP_NA_1
	{
		OnRecvBinarySinglePointShortTime(pBuff, nLength);
		break;
	}
	case M_SP_TB_1://带长时标的单点信息 ASDU30   SOE
	{
		OnRecvBinarySinglePointTime(pBuff, nLength);
		break;
	}
	case M_DP_TA_1://带短时标的双点信息 ASDU4    SOE
	{
		OnRecvBinaryDoublePointShortTime(pBuff, nLength);
		break;
	}
	case M_DP_TB_1://带长时标的双点信息 ASDU31   SOE
	{
		OnRecvBinaryDoublePointTime(pBuff, nLength);
		break;
	}
	//遥测
	case M_ME_NA_1://归一化测量值 ASDU9 M_ME_NC_1
	{
		OnRecvAnalogNormalized(pBuff, nLength);
		break;
	}
	case M_ME_TA_1://带短时标的归一化测量值 ASDU10
	{
	    OnRecvAnalogNormalizedShortTime(pBuff, nLength);
	    break;
	}
	case M_ME_NB_1://标度化测量值 ASDU11   TODO  暂时改为定值
	{
		OnRecvAnalogScaled(pBuff, nLength);
		break;
	}
	case M_ME_TB_1://带短时标的标度化测量值 ASDU12
	{
		OnRecvAnalogScaledShortTime(pBuff, nLength);
		break;
	}
	case M_ME_NC_1://短浮点数测量值 ASDU13
	{
		OnRecvAnalogShortFloat(pBuff, nLength);
		break;
	}
	case M_ME_TC_1://带短时标的短浮点数测量值 ASDU14
	{
		OnRecvAnalogShortFloatShortTime(pBuff, nLength);
		break;
	}
    case M_ME_ND_1://不带品质描述词的归一化测量值 ASDU21
    {
        OnRecvAnalogNormalizedNoQuality(pBuff, nLength);
        break;
    }
	//累计量   遥脉
	case M_IT_NA_1: //累计量 ASDU15
	{
		OnRecvKwh(pBuff, nLength);
		break;
	}
	case M_IT_TA_1: //带短时标的累计量 ASDU16
	{
		OnRecvKwhShortTime(pBuff, nLength);
		break;
	}
	case M_IT_TB_1: //带长时标的累计量 ASDU37
	{
		OnRecvKwhLongTime(pBuff, nLength);
		
		break;
	}
    case C_IC_NA_1:// 总召唤命令 ASDU100
    {
		//手否需要加密  //TODO
		m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);
        break;
    }
    case C_RP_NA_1:  //复位进程
    {
        OnRecvResetProcess(pBuff, nLength);
        break;
    }
	case C_SC_NA_1://遥控(单点命令) ASDU45
	{
		OnRecvSetBinarySPAck(pBuff, nLength);
		break;
	}
	case C_DC_NA_1://遥控(双点命令) ASDU46
	{
		OnRecvSetBinaryDPAck(pBuff, nLength);
		break;
	}
	case C_RD_NA_1:// 定值获取 单参
	case D_DV_RD_1://多参
	{
		//OnRecvDevReadRequestAck(pBuff, nLength);
		break;
	}
	case  D_DV_RD_2:
	{
		//读取信息
		OnRecvReadIecData(pBuff, nLength);
		break;
	}
	case D_DV_WR_3:
	{
		//写入确认
		OnRecvIecWriteRequestAck(pBuff, nLength);
		break;
	}
    case F_DR_TA_1:
    {
        //召唤目录响应
        //OnRecvResponseCatalog(pBuff, nLength);
        break;
    }
    case F_FR_NA_1:
    {
        //选择文件
        //OnRecvChooseFileResponse(pBuff, nLength);
        break;
    }
    case F_SR_NA_1:
    {
        //请求文件准备就绪
       // OnRectRequestFileReady(pBuff, nLength);
        break;
    }
    case F_SG_NA_1:
    {
        //节传输
       // OnRecvNodeData(pBuff, nLength);
        break;
    }
    case F_LS_NA_1:
    {
        //节最后的段
       // OnRecvLastNodePart(pBuff, nLength);
        break;
    }
    case F_AF_NA_1:
    {
        //节传输确认
        //空
        break;
    }
    case C_GZ_SJ_1:
    {
        //故障事件
        OnRecvDefalutMsg(pBuff, nLength);
        break;
    }
    case P_ME_NC_1:
    {
        //死区响应
        OnRecvZoneMsg(pBuff,nLength);
        break;
    }
	case D_FIX_SWITCH:
	{
		//切换定值区号
		OnRecvSwitchFixArea(pBuff, nLength);
		break;
	}
	case D_FIX_SEARCH:
	{
		//获取当前定值区号
		OnrecvReadCurrentFixArea(pBuff, nLength);
		break;
	}
	case D_FILE_TRANSPORT:
	{
		//文件服务
		OnRecvFileAnalyseInfo(pBuff, nLength);
		break;
	}
	case D_FIX_READ:
	{
		//定值读取
		OnRecvDevReadRequestAck(pBuff, nLength);
		break;
	}
	case D_FIX_WRITE:
	{
		OnRecvDevWriteRequestAck(pBuff, nLength);
		break;
	}
	case D_UPDATE_PROCESS:
	{
		OnRecvUpdateActionAck(pBuff, nLength);
		break;
	}

    default:
        break;
    }
}

void CApduRecver::OnRecvZoneMsg(char *pBuff, int nLength)
{
    if (nLength == 0)
    {
        return;
    }
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);



}


void CApduRecver::OnRecvCatalogRepond(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND* pAsduCatlog = (FILE_CATALOG_RESPOND*)pBuff;


// 	m_pCatalogRespond->asduAddr = pAsduCatlog->asduAddr;
// 	m_pCatalogRespond->cot = pAsduCatlog->cot;
// 	m_pCatalogRespond->m_nCatalogID = pAsduCatlog->m_nCatalogID;
// 	m_pCatalogRespond->m_nOperatorType = pAsduCatlog->m_nOperatorType;
// 
// 	m_pCatalogRespond->m_nResultFlag = pAsduCatlog->m_nResultFlag;
// 	m_pCatalogRespond->m_uFileCount = pAsduCatlog->m_uFileCount;
// 
// 	m_pCatalogRespond->m_uNextFlag = pAsduCatlog->m_uNextFlag;
// 	m_pCatalogRespond->type = pAsduCatlog->type;
// 	m_pCatalogRespond->vsq = pAsduCatlog->vsq;

	char *pStartFileAttr = pBuff + sizeof(FILE_CATALOG_RESPOND);

	for (int i=0; i<pAsduCatlog->m_uFileCount; i++)
	{

		FILE_ATTR_INFO tArrInfo;
		tArrInfo.m_cFileNameLength = pStartFileAttr[0];
		//文件名称
		tArrInfo.m_strFileName = QString::fromLocal8Bit(pStartFileAttr + 1, tArrInfo.m_cFileNameLength);
	   //文件属性
		tArrInfo.m_cFileAttr = pStartFileAttr[1 + tArrInfo.m_cFileNameLength];
	   //文件大小
	   INFOADDR4* tSizeInfo = (INFOADDR4*)(pStartFileAttr + 1 + tArrInfo.m_cFileNameLength +1);

	   tArrInfo.m_FileSize = tSizeInfo->GetAddr();

	   //时间不用

	   pStartFileAttr = pStartFileAttr + 1 + tArrInfo.m_cFileNameLength + 1 + 4 + 7;

	   //m_arrCatalogRespond.append(tArrInfo);
	}

}

//读文件激活确认
void CApduRecver::OnRecvReadActionResond(char *pBuff, int nLenght)
{
	if (nLenght <= 0)
	{
		return;
	}

	ASDU_BASE *pbase = (ASDU_BASE*)pBuff;
	//基础信息

	//激活确认信息
	//操作标识
	char *pActionRespondData = pBuff + sizeof(ASDU_BASE);

	unsigned char bOperatorFlag = pActionRespondData[0];

	unsigned char cResultFlag = pActionRespondData[1];

	unsigned char cFileNameLegth = pActionRespondData[2];

	QString strFileName = QByteArray(pActionRespondData[3], cFileNameLegth);
    //文件标识
	INFOADDR4 *tFileFlag = (INFOADDR4*)(pActionRespondData+3+cFileNameLegth);
	//文件大小
	INFOADDR4 *TFileSize = (INFOADDR4*)(pActionRespondData+3+cFileNameLegth+sizeof(INFOADDR4));

	//

}

//读文件传输数据
void CApduRecver::OnRecvReadFileRespond(char *pBuff, int nLength)
{
}

void CApduRecver::OnRecvReadConform(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	READ_NODE_FILE_CONFORM *pbase = (READ_NODE_FILE_CONFORM*)pBuff;
	//基础信息

}

//文件 写激活确认
void CApduRecver::OnRecvWriteAction(char *pBuff, int nLength)
{

}

void CApduRecver::OnRecvWriteConform(char *pBuff, int nLength)
{

}

//升级激活确认
void CApduRecver::OnRecvUpdateActionAck(char * pBuff, int nLength)
{
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

//收到单点命令确认
void CApduRecver::OnRecvSetBinarySPAck(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);
}

//收到双点命令确认
void CApduRecver::OnRecvSetBinaryDPAck(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

//参数读取
/*********************************************************************************************************
** \brief OnRecvDevReadRequestAck(char *pBuff, int nLength)
** \details 参数读取
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvDevReadRequestAck(char* pBuff, int nLength)
{
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

//参数写入
void CApduRecver::OnRecvDevWriteRequestAck(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

//iec 写入反馈
void CApduRecver::OnRecvIecWriteRequestAck(char *pBuff, int nLength)
{
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

void CApduRecver::OnRecvIecReadRequestAck(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	//ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
	ASDU_IEC* pAsdudz = (ASDU_IEC*)pBuff;

	//int nDeviceAddr = pAsdudz->asduAddr.GetAddr();

	int nCount = pAsdudz->GetItemCount();

	QMap<int, float> mapPointValue;

	int nPointID = pAsdudz->infoaddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{

		short fValue = pAsdudz->GetValue(i);
		mapPointValue[nPointID++] = fValue;
	}

	if (!mapPointValue.isEmpty())
	{
		//emit Signal_DevReadBack(mapPointValue);
	}
}



/*********************************************************************************************************
** \brief OnRecvBinarySinglePoint(char *pBuff, int nLength)
** \details 解析单点遥信数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void   
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinarySinglePoint(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	/*

	ASDU_ENCODE tEncodeHeader;
	tEncodeHeader.m_nStart = 0xEB;
	tEncodeHeader.m_nStart2 = 0xEB;

	tEncodeHeader.m_nType0 = 0;

	tEncodeHeader.m_nType1.D6 = 1;
	*/


	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

void CApduRecver::OnRecvBinaryDoublePoint(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvBinaryGroupPoint(char *pBuff, int nLength)
** \details 成组单点遥信
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinaryGroupPoint(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvBinarySinglePointTime(char *pBuff, int nLength)
** \details 带短时标的单点信息  SEO
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinarySinglePointShortTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);


}

/*********************************************************************************************************
** \brief OnRecvBinarySinglePointTime(char *pBuff, int nLength)
** \details SOE
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinarySinglePointTime(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvAnalogNormalized(char *pBuff, int nLength)
** \details 带短时标的双点信息  SEO
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvAnalogNormalized(char *pBuff, int nLength)
** \details 带长时标的双点信息  SEO
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvBinaryDoublePointTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvAnalogNormalized(char *pBuff, int nLength)
** \details 归一化遥测数据解析
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogNormalized(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 带短时标的归一化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	//加密后转发     TODO
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLen);

}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 标度化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogScaled(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);
}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 带短时标的标度化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogScaledShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLen);

}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 累计量  遥脉
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvKwh(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvKwhShortTime(char *pBuff, int nLength)
** \details 带短时标的累计量  遥脉
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvKwhShortTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvKwhLongTime(char *pBuff, int nLength)
** \details 带长时标的累计量  遥脉
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvKwhLongTime(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

/*********************************************************************************************************
** \brief OnRecvAnalogShortFloat(char *pBuff, int nLength)
** \details 浮点型遥测数据解析
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogShortFloat(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);
}

//iec
void CApduRecver::OnRecvReadIecData(char* pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

void CApduRecver::OnDeCode(char * pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}

	qDebug() << QByteArray(pBuff, nLen).toHex();

	if (!m_DeviceEncodeCache.IsEmpty())
	{
		if (m_DeviceEncodeCache.GetCurByteLength() == 1)//只有一个启动字符
		{
			m_DeviceEncodeCache.CopyData(pBuff, 2);//复制长度域
			pBuff++;
			pBuff++;
			nLen--;
			nLen--;
		}

		int nNeedBytes = m_DeviceEncodeCache.GetNeedByteLength();
		if (nNeedBytes <= 0)
		{
			return;
		}

		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_DeviceEncodeCache.CopyData(pBuff, nLen);
			return;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_DeviceEncodeCache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;

			bool ISOK = OnAnalyseDecode(m_DeviceEncodeCache.GetDataBuff(), m_DeviceEncodeCache.GetDataLen());
			m_DeviceEncodeCache.Clear();
			if (!ISOK)
			{
				return ;
			}

			//解析剩余的部分
			return OnDeCode(pBuff, nLen);
		}
	}
	else//缓冲区为空
	{
		//在接收区寻找启动字符
		char cStartChar = DECODESTART;
		char* pNewBuff = std::find(pBuff, pBuff + nLen, cStartChar);

		if (pNewBuff == pBuff + nLen)
			return;//没有找到启动字符

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//复制启动字符至缓冲区
		m_DeviceEncodeCache.CopyData(pBuff, 1);
		pBuff += 1;
		nLen -= 1;

		return OnDeCode(pBuff, nLen);
	}

}

//
void CApduRecver::ReadCerFile(const QString &fileName, char *cel, int *len)
{
	QFile tCerFile(fileName);

	if (tCerFile.open(QFile::ReadOnly))
	{
		QByteArray tData = tCerFile.readAll();
		strncpy(cel, tData.data(), tData.length());

		int nLength = tData.length();

		memcpy(len, &nLength, 4);
	}
	else
	{
		QMessageBox::warning(0, tr("告警"),tr("读取证书文件失败"));
	}
		
}

bool CApduRecver::OnAnalyseDecode(char * pBuff, int nLength)
{
	ASDU_ENCODE* pEcodeHeader = (ASDU_ENCODE*)(pBuff);

	//计算应用数据区长度
	int nDataLength = pEcodeHeader->m_nLength.GetAddr();

	if (pEcodeHeader->m_nType1.D3 == 0)
	{
		//不加密
		unsigned char uApplyType = pBuff[6];

		switch (uApplyType)
		{
		case 0x0:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0 - 2;
			break;
		}
		case 0x01:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x41 - 2;

			break;
		}
		case 0x02:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x8 - 2;

			break;
		}
		case 0x03:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x49 - 2;

			break;
		}
		case 0x04:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x6 - 2;

			break;
		}
		case 0x05:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x47 - 2;

			break;
		}
		case 0x06:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0xE - 2;

			break;
		}
		case 0x07:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x4F - 2;

			break;
		}
		case 0x08:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x4F - 2;

			break;
		}
		case 0x1f:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x2 - 2;

			break;
		}
		case 0x51:
		{
			//终端返回的认证信息
			CLogCache::InstanceCache()->InsertLog(tr("终端认证确认并请求主站认证:%1").arg(QByteArray(pBuff, nLength).toHex().toStdString().c_str()));

			qDebug() << QByteArray(pBuff, nLength).toHex().toStdString().c_str();
			//终端随机数
			char rand[8];
			strncpy(rand,pBuff + sizeof(ASDU_ENCODE) + 1 + 2,8);
			//签名结果  64字节
			char signResult[64];
			strncpy(signResult,pBuff+ sizeof(ASDU_ENCODE) + 1 + 2 + 8,64);
			//签名秘钥标识
			char keyFlag;
			keyFlag = pBuff[sizeof(ASDU_ENCODE) + 1 + 2 + 8 + 64];
			//对签名结果进行验证
			int nCerLength = 0;
			char arrCer[1024*2];

		    ReadCerFile(qApp->applicationDirPath() + "/F30114301001201707280002.cer", arrCer, &nCerLength);
			int nResult = CXAHost::GetInstance()->VerSign(nCerLength,(unsigned char*)arrCer,8,(unsigned char*)rand,64,(unsigned char*)signResult);
			//end

			//主站对终端的认证请求进行相应
			//主站对终端的随机数进行签名  将签名结果发送给终端
			unsigned char sData[64];   //签名结果
			int sDataLen = 64;
			int nSingResult = CXAHost::GetInstance()->InternalSign(3,8,(unsigned char*)rand,sData,&sDataLen);

			if (nSingResult != 0)
			{
				QMessageBox::warning(0, tr("告警"), tr("调用主站加密接口失败,错误码:%1").arg(nSingResult));
			}
			qDebug() << QByteArray(pBuff, nLength).toHex();
			qDebug() << QByteArray(rand, 8).toHex();
			//组包发送签名结果

			//发送数据 发送随机数
			char pBuffEncode[1024];
			ASDU_ENCODE *pHeader = (ASDU_ENCODE *)pBuffEncode;
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

			qDebug() << QByteArray(pBuffEncode, sizeof(ASDU_ENCODE));

			//报文长度
			pHeader->m_nLength.SetAddr(2+1+2+64+1);

			pBuffEncode[sizeof(ASDU_ENCODE)] = 0x52;

			ASDUADDR2 *pAddr = (ASDUADDR2*)(pBuffEncode + sizeof(ASDU_ENCODE) + 1);
			//8字节
			pAddr->SetAddr(0x41);

			strncpy(pBuffEncode + sizeof(ASDU_ENCODE) + 1 + 2, (char*)sData, 64);

			pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 2 + 64] = 6;

			unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pBuffEncode +4), 68+2);

			pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 2 + 64 + 1] = pCheckData;
			pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 2 + 64 + 1 + 1] = 0xD7;

			//发送数据
			m_pSender->GetSendSocket()->WriteSocketData(pBuffEncode, sizeof(ASDU_ENCODE) + 1 + 2 + 64 + 1 + 1 + 1);

			CLogCache::InstanceCache()->InsertLog(tr("主站对终端随机数进行签名，将签名结果发送给终端:%1").arg(QByteArray(pBuffEncode, sizeof(ASDU_ENCODE) + 1 + 64 + 1 + 1 + 1 + 2).toHex().toStdString().c_str()));
			nDataLength = 0;
			break;
		}
		case 0x53:
		{
			nDataLength = 0;
			CLogCache::InstanceCache()->InsertLog(tr("认证结果:%1").arg(QByteArray(pBuff, nLength).toHex().toStdString().c_str()));


			//获取终端序列号
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

			pHeader->m_nLength.SetAddr(5);

			pBuff[sizeof(ASDU_ENCODE)] = 0x54;

			ASDUADDR2 *pAddr = (ASDUADDR2*)(pBuff + sizeof(ASDU_ENCODE) + 1);
			//8字节
			pAddr->SetAddr(0);


			unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pBuff+4), 5);

			pBuff[sizeof(ASDU_ENCODE) + 3] = pCheckData;
			pBuff[sizeof(ASDU_ENCODE) + 4] = 0xD7;

			//发送数据
			m_pSender->GetSendSocket()->WriteSocketData(pBuff, sizeof(ASDU_ENCODE) + 5);

			CLogCache::InstanceCache()->InsertLog(tr("主站获取终端芯片序列号:%1").arg(QByteArray(pBuff, sizeof(ASDU_ENCODE) + 5).toHex().toStdString().c_str()));
			nDataLength = 0;


			break;
		}
		case 0x55:
		{
			nDataLength = 0;
			CLogCache::InstanceCache()->InsertLog(tr("终端返回内置芯片的序列号:%1").arg(QByteArray(pBuff, nLength).toHex().toStdString().c_str()));
			//终端芯片序列号
			CXAHost::GetInstance()->SetCellIndex(pBuff + 9,8);
			qDebug() << QByteArray(pBuff + 9, 8).toHex();;
			//获取密钥版本

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

			pHeader->m_nLength.SetAddr(5);

			pBuff[sizeof(ASDU_ENCODE)] = 0x60;

			ASDUADDR2 *pAddr = (ASDUADDR2*)(pBuff + sizeof(ASDU_ENCODE) + 1);
			//8字节
			pAddr->SetAddr(0);


			unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pBuff+4), 5);

			pBuff[sizeof(ASDU_ENCODE) + 3] = pCheckData;
			pBuff[sizeof(ASDU_ENCODE) + 4] = 0xD7;

			//发送数据
			m_pSender->GetSendSocket()->WriteSocketData(pBuff, sizeof(ASDU_ENCODE) + 5);

			CLogCache::InstanceCache()->InsertLog(tr("主站获取终端密钥报文:%1").arg(QByteArray(pBuff, sizeof(ASDU_ENCODE) + 5).toHex().toStdString().c_str()));

			break;
		}
		case 0x61:
		{
			nDataLength = 0;
			CXAHost::GetInstance()->SetVersionIndex(pBuff[9]);
			//终端密钥版本
			CLogCache::InstanceCache()->InsertLog(tr("终端密钥版本报文:%1").arg(QByteArray(pBuff, nLength).toHex().toStdString().c_str()));

			break;
		}

		default:
			break;
		}
	}
	else
	{
		//解密
		unsigned char uApplyType = pBuff[6];

		switch (uApplyType)
		{
		case 0x0:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0 - 2;
			break;
		}
		case 0x01:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x41 - 2;

			break;
		}
		case 0x02:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x8 - 2;

			break;
		}
		case 0x03:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x49 - 2;

			break;
		}
		case 0x04:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x6 - 2;

			break;
		}
		case 0x05:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x47 - 2;

			break;
		}
		case 0x06:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0xE - 2;

			break;
		}
		case 0x07:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x4F - 2;

			break;
		}
		case 0x08:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x4F - 2;

			break;
		}
		case 0x1f:
		{
			nDataLength = nDataLength - sizeof(ASDU_ENCODE) - 1 - 0x2 - 2;

			break;
		}
		default:
			break;
		}
	}

	if (nDataLength != 0)
	{
		OnDeviceRecv(pBuff + sizeof(ASDU_ENCODE) + 1, nDataLength);
	}

	return true;
}

//带短时标的短浮点数测量值
/*********************************************************************************************************
** \brief OnRecvAnalogShortFloatShortTime(char *pBuff, int nLength)
** \details 带短时标的短浮点数测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLen);

}

//不带品质描述词的归一化测量值
/*********************************************************************************************************
** \brief OnRecvAnalogNormalizedNoQuality(char *pBuff, int nLength)
** \details 不带品质描述词的归一化测量值
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return;
	}
	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLen);


}

/*********************************************************************************************************
** \brief AnalyseU(char *pBuff, int nLength)
** \details 解析U格式类型数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CApduRecver::AnalyseU(char *pBuff, int nLength)
{
	//发送数据 
	char pBuffEncode[1024];
	ASDU_ENCODE *pHeader = (ASDU_ENCODE *)pBuffEncode;
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
	pHeader->m_nLength.SetAddr(2 + 1 + 1 + nLength + 2);

	pBuffEncode[sizeof(ASDU_ENCODE)] = 0x00;
	pBuffEncode[sizeof(ASDU_ENCODE) + 1] = nLength;

	strncpy(pBuffEncode + sizeof(ASDU_ENCODE) + 2, pBuff, nLength);


	ASDUADDR2 *pAddr = (ASDUADDR2*)(pBuffEncode + sizeof(ASDU_ENCODE) + 2+ nLength);
	//8字节
	pAddr->SetAddr(0);

	unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pBuffEncode + 4), 2+1+1+nLength+2);

	pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2] = pCheckData;
	pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2+1] = 0xD7;

	//发送数据
	m_pSender->GetSendSocket()->WriteSocketData(pBuffEncode, sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2 + 1+1);

	CLogCache::InstanceCache()->InsertLog(tr("向装置侧发送U数据:%1").arg(QByteArray(pBuffEncode, sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2 + 1 + 1).toHex().toStdString().c_str()));



	//m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);
    return true;
}

bool CApduRecver::AnalyseS(char* pBuff, int nLength)
{
	//加密后转发

	char pBuffEncode[1024];
	ASDU_ENCODE *pHeader = (ASDU_ENCODE *)pBuffEncode;
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
	pHeader->m_nLength.SetAddr(2 + 1 + 1 + nLength + 2);

	pBuffEncode[sizeof(ASDU_ENCODE)] = 0x00;
	pBuffEncode[sizeof(ASDU_ENCODE) + 1] = nLength;

	strncpy(pBuffEncode + sizeof(ASDU_ENCODE) + 2, pBuff, nLength);


	ASDUADDR2 *pAddr = (ASDUADDR2*)(pBuffEncode + sizeof(ASDU_ENCODE) + 2 + nLength);
	//8字节
	pAddr->SetAddr(0);

	unsigned char pCheckData = CXAHost::GetInstance()->checkAllData((unsigned char*)(pBuffEncode + 4), 2 + 1 + 1 + nLength + 2);

	pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2] = pCheckData;
	pBuffEncode[sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2 + 1] = 0xD7;

	//发送数据
	m_pSender->GetSendSocket()->WriteSocketData(pBuffEncode, sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2 + 1 + 1);

	CLogCache::InstanceCache()->InsertLog(tr("向装置侧发送S数据:%1").arg(QByteArray(pBuffEncode, sizeof(ASDU_ENCODE) + 1 + 1 + nLength + 2 + 1 + 1).toHex().toStdString().c_str()));


	//m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

	return true;
}


/*********************************************************************************************************
** \brief AnalyseU(char *pBuff, int nLength)
** \details 解析U格式类型数据
** \param argl   一单元十六进制数据包
** \param arg2   数据包的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CApduRecver::SendRequestAllMsg()
{
	//启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
	//召唤全数据
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
// 	bool IsOK = m_pComm104Pln->OnCommand(&telectrl);
// 	if (!IsOK)
// 	{
// 		return;
// 	}

	/*
	//召唤全电度
	telectrl.m_nCtrlType = TELECTRL_REQUEST_SYNCTIME;
	IsOK = m_pComm104Pln->OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}

	//对时请求
	telectrl.m_nCtrlType = TELECTRL_REQUEST_KWHGENERALCALL;
	IsOK = m_pComm104Pln->OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}
	*/
}

void CApduRecver::SetSender(CApduSender * pSender)
{
	m_pSender = pSender;
	pSender->GetSendSocket()->setApduRecver(this);
}

bool CApduRecver::OnDeviceRecv(char * pBuff, int nLen)
{
	//解密   TODO
	if (nLen <= 0)
	{
		return false;
	}

	qDebug() <<"解密前:"<< QByteArray(pBuff, nLen).toHex();

	char *pDecodeBuff = new char[2048];
	int nBuffLen = 0;
	CXAHost::GetInstance()->DeData(nLen,(unsigned char*)pBuff, (unsigned char*)pDecodeBuff, &nBuffLen);


	if (!m_104DeviceCache.IsEmpty())
	{
		if (m_104DeviceCache.GetCurByteLength() == 1)//只有一个启动字符
		{
			m_104DeviceCache.CopyData(pDecodeBuff, 1);//复制长度域
			pDecodeBuff++;
			nBuffLen--;
		}

		int nNeedBytes = m_104DeviceCache.GetNeedByteLength();
		if (nNeedBytes <= 0)
		{
			return false;
		}

		if (nNeedBytes > nBuffLen)//需要的长度大于接收到的长度
		{
			m_104DeviceCache.CopyData(pDecodeBuff, nBuffLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_104DeviceCache.CopyData(pDecodeBuff, nNeedBytes);
			pDecodeBuff += nNeedBytes;
			nBuffLen -= nNeedBytes;

			bool ISOK = AnalyseDeviceApdu(m_104DeviceCache.GetDataBuff(), m_104DeviceCache.GetDataLen());
			m_104DeviceCache.Clear();
			if (!ISOK)
			{
				return false;
			}

			//解析剩余的部分
			return OnDeviceRecv(pDecodeBuff, nBuffLen);
		}
	}
	else//缓冲区为空
	{
		//在接收区寻找启动字符
		char cStartChar = STARTCHAR;
		char* pNewBuff = std::find(pDecodeBuff, pDecodeBuff + nBuffLen, cStartChar);

		if (pNewBuff == pDecodeBuff + nBuffLen)
			return false;//没有找到启动字符

		nBuffLen = nBuffLen - (pNewBuff - pDecodeBuff);
		pDecodeBuff = pNewBuff;

		//复制启动字符至缓冲区
		m_104DeviceCache.CopyData(pDecodeBuff, 1);
		pDecodeBuff += 1;
		nBuffLen -= 1;

		return OnDeviceRecv(pDecodeBuff, nBuffLen);
	}

	return true;
}

QTcpSocket * CApduRecver::GetZuTcpSocket()
{
	return m_pzMidTcp;
}

//收到召唤响应报文
// void CApduRecver::OnRecvResponseCatalog(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
//     QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
//     m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Recv Response Catalog").toLocal8Bit().data(), 1);
// 
//     ASDUMLXY* pAsduZgml = (ASDUMLXY*)pBuff;
//     //文件数量
//     int nCount = pAsduZgml->GetItemCount();
// 
//     for (int i = 0; i < nCount; i++)
//     {
//         //当存在需要请求的文件时发送文件选择命令
//         if (pAsduZgml->GetValue(i) == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//         {
//             m_pComm104Pln->getSender()->OnSendChooseLbFile();
//         }
//     }
// 
// }

//选择文件
// void CApduRecver::OnRecvChooseFileResponse(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
//     QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
//     m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Recv Choose File Response").toLocal8Bit().data(), 1);
// 
// 
//     ASDUCHRQ* pAsduZgml = (ASDUCHRQ*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         if (pAsduZgml->m_frq == 0)
//         {
//             //0表示确认
//             //2 表示请求报文
//             m_pComm104Pln->getSender()->OnSendRequestFile(2);
//         }
//     }
// 
// }


//请求文件准备就绪
// void CApduRecver::OnRectRequestFileReady(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
// 
//     ASDUQQWJ* pAsduZgml = (ASDUQQWJ*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         if (pAsduZgml->m_frq == 0)
//         {
//             //0表示确认
//             //记录结长度
//             m_nNodeLength = pAsduZgml->m_FileLeg.GetAddr();
//             //请求节
//             if (m_nNodeLength > 0)
//             {
//                 //6表示请求节
//                 m_pComm104Pln->getSender()->OnSendRequestFile(6);
//                 m_byNodeData.clear();
//             }
//         }
//     }
// }

//节数据传输
// void CApduRecver::OnRecvNodeData(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
//     ASDUFILENODE* pAsduZgml = (ASDUFILENODE*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         //节数据还没发完
//         if (m_nNodeLength > 0)
//         {
//             //
//             m_byNodeData.append(pAsduZgml->m_data, pAsduZgml->m_NodeLegth);
//         }
// 
//         m_nNodeLength--;
//     }
// }

//一节最后段传输
// void CApduRecver::OnRecvLastNodePart(char *pBuff, int nLength)
// {
//     if (nLength == 0)
//     {
//         return;
//     }
// 
//     //发送数据合法性判断
//     if (!m_pComm104Pln->getSender()->GetLbData().m_bValidFlag)
//     {
//         return;
//     }
// 
//     ASDULASTNODE* pAsduZgml = (ASDULASTNODE*)pBuff;
// 
//     if (pAsduZgml->m_infoFileIndex.GetAddr() == m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex)
//     {
//         //最后的节
//         if (pAsduZgml->m_reserve == 1)
//         {
//             //整个文件的和校验
//             m_AbyFileData.append(m_byNodeData);
// 
//             if (pAsduZgml->m_frq == checkAllData((unsigned char *)m_AbyFileData.data(), m_AbyFileData.length()))
//             {
//                 //和校验通过  最后的文件
//                 //下发下一个文件
//                 //发送认可节
//                 m_pComm104Pln->getSender()->OnSendFileConformNode();
// 
//                 if (m_pComm104Pln->getSender()->GetLbAllData().count() > 0)
//                 {
//                     //保存文件
//                     m_pFileLoad->CreateNewFile(m_pComm104Pln->getSender()->GetLbData().m_strFileName, m_AbyFileData);
//                     m_AbyFileData.clear();
//                     //end
//                     QString strDeviceName = m_pComm104Pln->GetFtpModule()->GetDeviceName();
//                     QString strLog = tr("No") + m_pComm104Pln->getSender()->GetLbData().m_nFilenameIndex + tr(" File Recv Success!");
//                     m_pComm104Pln->GetFtpModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Recv Choose File Response").toLocal8Bit().data(), 1);
// 
//                     //当成功后
//                     m_pComm104Pln->getSender()->OnSendRecordCatalog(m_pComm104Pln->getSender()->GetLbAllData().at(0));
//                 }
//             }
//             else
//             {
//                 //和校验不通过
//             }
//         }
//         else
//         {
//             //不是最后的节
//             //和校验
//             if (pAsduZgml->m_frq == checkAllData((unsigned char *)m_byNodeData.data(), m_byNodeData.length()))
//             {
//                 m_AbyFileData.append(m_byNodeData);
//             }
//             else
//             {
//                 //和校验失败
//             }
//         }
// 
//     }
// }

unsigned char CApduRecver::checkAllData(const unsigned char *data, int length)
{
    unsigned char lValue = 0;
    for (int i = 0; i < length; i++)
    {
        lValue += data[i];
    }
    return lValue;
}

CApduRecver::~CApduRecver()
{
	m_pTcpServer->close();
}




void CApduRecver::OnRecvResetProcess(char *pBuff, int nLength)
{
    //记录

    if (nLength <= 0)
    {
        return;
    }

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

//切换定值区
void CApduRecver::OnRecvSwitchFixArea(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}


void CApduRecver::OnrecvReadCurrentFixArea(char *pBuff, int nLength)
{
	//获取当前定值区号
	if (nLength <= 0)
	{
		return;
	}

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

void CApduRecver::OnRecvFileAnalyseInfo(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

}

//目录召唤响应
void CApduRecver::OnRecvCatalogRespond(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND *pCatalogRespond = (FILE_CATALOG_RESPOND*)pBuff;

	if (pCatalogRespond->m_nResultFlag == 1)
	{
		return;
	}



	/*
	//报文长度
	int nBagLength = pCatalogRespond->GetLength();
	//现有文件长度
	int nFileLengths = 0;


	for (int i=0; i<pCatalogRespond->m_uFileCount; i++)
	{
		//文件名称长度
		int nFileNameLength = pBuff[nBagLength + nFileLengths];   //
		//文件名称
		QString strFilename = QString::fromLocal8Bit(pBuff + nBagLength + 1 + nFileLengths, nFileNameLength);
		//文件属性
		unsigned char uFileAttr = pBuff[nBagLength+1+nFileNameLength + nFileLengths];
		//文件大小
		INFOADDR4 *infoSize = (INFOADDR4*)(pBuff+nBagLength+2+nFileNameLength + nFileLengths);

		nFileLengths += 2 + nFileNameLength + sizeof(INFOADDR4) + 7;

		int nFileSize = infoSize->GetAddr();

		if (uFileAttr == 1)
		{
			//目录不做处理
			continue;
		}

		if (strFilename.contains(m_fileAttrInfo.m_strFileName))
		{
			FILE_ATTR_INFO tFileAttr;

			tFileAttr.m_cFileNameLength = nFileNameLength;
			tFileAttr.m_strFileName = strFilename;
			tFileAttr.m_FileSize = nFileSize;
			tFileAttr.m_cFileAttr = uFileAttr;
			tFileAttr.type = D_FILE_TRANSPORT;

			m_arrCatalogRespond.append(tFileAttr);

		}

	}

	if (pCatalogRespond->m_uNextFlag == 0)
	{
		//无后续
		//开始读取文件 
		if (m_arrCatalogRespond.count() != 0)
		{
			m_pComm104Pln->getSender()->OnSendReadFileAction(m_arrCatalogRespond.first());
			//m_arrCatalogRespond.removeFirst();

			if (m_arrCatalogRespond.isEmpty())
			{
				//文件下载结束
				m_pComm104Pln->getSender()->ClearFilepath();
			}
		}
	}

	*/
}

//读文件激活确认
void CApduRecver::OnRecvFileAction(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND *pActionRespond = (FILE_CATALOG_RESPOND*)pBuff;

	if (pActionRespond->m_nResultFlag == 1)
	{
		//失败
		return;
	}

}

//读取文件数据
void CApduRecver::OnRecvFileData(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}


}

//读文件传输确认
void CApduRecver::OnRecvFileConform(char *pBuff, int nLength)
{

}

//故障事件
void CApduRecver::OnRecvDefalutMsg(char *pBuff, int nLength)
{
    //
    if (nLength <= 0)
    {
        return;
    }

	m_pSender->GetSendSocket()->WriteSocketData(pBuff, nLength);

    
}
