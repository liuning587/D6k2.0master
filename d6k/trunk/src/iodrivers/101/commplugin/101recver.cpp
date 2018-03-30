
#include "101recver.h"
#include "asdu_info.h"
#include "commplugin.h"
#include "101sender.h"
#include "devicestudio/main_module.h"
#include "ftu_module.h"
#include <algorithm>

namespace MYLIB
{
	std::string FormatBuffer(char* pBuff, int nMsgLen)
	{
		std::string strDebugData("");
		char szByte[10] = { '\0' };

		for (int i = 0; i < nMsgLen; i++)
		{
			sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
			strDebugData += szByte;
		}
		return strDebugData;
	}
}


C101Recver::C101Recver(QObject* parent) :QObject(parent)
{
	m_p101Impl = qobject_cast<CCommPlugin*>(parent);
	Q_ASSERT(m_p101Impl);
	if (!m_p101Impl)
	{
		return;
	}
}

C101Recver::~C101Recver()
{

}


bool C101Recver::OnReceive(char* pBuff, int nLen)
{
	if (nLen <= 0)
		return true;

	if (!m_101Cache.IsEmpty())
	{
		int nFrameType = m_101Cache.GetFrameType();
		switch (nFrameType)
		{
		case TYPE_E://单字符E5
			break;
		case TYPE_F://固定帧
			break;
		case TYPE_V:
		{
			if (m_101Cache.GetCurByteLength() == 1)//只有一个启动字符
			{
				m_101Cache.CopyData(pBuff, 1);//复制第一个长度域
				pBuff++;
				nLen--;
			}
		}
		break;
		}

		int nNeedBytes = m_101Cache.GetNeedByteLength();
		if (nNeedBytes <= 0)
			return true;
		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_101Cache.CopyData(pBuff, nLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_101Cache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;
			AnalyseAvdu(m_101Cache.GetDataBuff(), m_101Cache.GetDataLen());
			m_101Cache.Clear();

			//解析剩余的部分
			return OnReceive(pBuff, nLen);
		}
	}
	else//缓冲区为空
	{
		//在接收区寻找启动字符
		char* pNewBuff = m_101Cache.FindStartChar(pBuff, nLen);
		if (pNewBuff == NULL)
			return true;

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//复制启动字符至缓冲区
		m_101Cache.CopyData(pBuff, 1);
		pBuff += 1;
		nLen -= 1;

		return OnReceive(pBuff, nLen);
	}

	return true;
}

bool C101Recver::AnalyseAvdu(char* pBuff, int nLen)
{
	char StartChar = pBuff[0];
	
	switch (StartChar)
	{
	case 0xE5://单字符
		return AnalyseE5(pBuff, nLen);
	case 0x10://固定帧长
		return AnalyseF(pBuff, nLen);
	case 0x68://可变帧长
	{
		bool bRet= AnalyseV(pBuff, nLen);
		m_p101Impl->getSender()->SendConfirm();	
		return bRet;
	}
	default:
		if (nLen > 0)
		{
			std::string strMsg = MYLIB::FormatBuffer(pBuff, nLen);
			strMsg = "Unknown: " + strMsg;
			LogMsg(strMsg.c_str(), 1);
		}
		m_p101Impl->getSender()->SendConfirm();
		break;
	}
	return true;
}

bool C101Recver::AnalyseE5(char* pBuff, int nLen)
{
	LogE(0, pBuff, nLen);
	return true;
}

bool C101Recver::AnalyseF(char* pBuff, int nLen)
{
	LogF(1, pBuff, nLen);
	return OnRecvFfromMonitor(pBuff, nLen);
}

bool C101Recver::AnalyseV(char* pBuff, int nLen)
{
	//解析控制域
	AVDU_HEAD* pHead = (AVDU_HEAD*)pBuff;

	int nDeviceAddr = pHead->m_addr.GetAddr();

	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	if (pDeviceData == NULL)
		return false;
	pDeviceData->m_nACD = pHead->m_ctrl.m_DATA;

	return AnalyseAsdu(pBuff, nLen);
}

bool C101Recver::AnalyseAsdu(char* pBuff, int nLen)
{
	LogV(2, pBuff, nLen);
	//解析ASDU
	pBuff = pBuff + sizeof(AVDU_HEAD);
	nLen = nLen - sizeof(AVDU_HEAD) - sizeof(AVDU_TAIL);

	ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff);

	switch (pAsduBase->type)
	{
		//遥信
	case M_SP_NA_1: // 单点信息  ASDU1
		OnRecvBinarySinglePoint(pBuff, nLen);
		break;
	case M_DP_NA_1: // 双点信息   ASDU3
		OnRecvBinaryDoublePoint(pBuff, nLen);
		break;
	case M_PS_NA_1: // 成组单点信息   ASDU20
		OnRecvBinaryGroupPoint(pBuff, nLen);
		break;
	case M_SP_TA_1://带短时标的单点信息 ASDU2
		OnRecvBinarySinglePointShortTime(pBuff, nLen);
		break;
	case M_SP_TB_1://带长时标的单点信息 ASDU30
		OnRecvBinarySinglePointTime(pBuff, nLen);
		break;
	case M_DP_TA_1://带短时标的双点信息 ASDU4
		OnRecvBinaryDoublePointShortTime(pBuff, nLen);
		break;
	case M_DP_TB_1://带长时标的双点信息 ASDU31
		OnRecvBinaryDoublePointTime(pBuff, nLen);
		break;

		//遥测
	case M_ME_NA_1://归一化测量值 ASDU9
		OnRecvAnalogNormalized(pBuff, nLen);
		break;
	case M_ME_TA_1://带短时标的归一化测量值 ASDU10
		OnRecvAnalogNormalizedShortTime(pBuff, nLen);
		break;
	case M_ME_NB_1://标度化测量值 ASDU11
		OnRecvAnalogScaled(pBuff, nLen);
		break;
	case M_ME_TB_1://带短时标的标度化测量值 ASDU12
		OnRecvAnalogScaledShortTime(pBuff, nLen);
		break;
	case M_ME_NC_1://短浮点数测量值 ASDU13
		OnRecvAnalogShortFloat(pBuff, nLen);
		break;
	case M_ME_TC_1://带短时标的短浮点数测量值 ASDU14
		OnRecvAnalogShortFloatShortTime(pBuff, nLen);
		break;
	case M_ME_ND_1://不带品质描述词的归一化测量值 ASDU21
		OnRecvAnalogNormalizedNoQuality(pBuff, nLen);
		break;
	case M_ME_TD_1://带长时标的归一化测量值 ASDU34
		OnRecvAnalogNormalizedLongTime(pBuff, nLen);
		break;
	case M_ME_TE_1://带长时标的标度化测量值 ASDU35
		OnRecvAnalogScaledLongTime(pBuff, nLen);
		break;
	case M_ME_TF_1://带长时标的短浮点数测量值 ASDU36
		OnRecvAnalogShortFloatLongTime(pBuff, nLen);
		break;
	case M_FT_NA_1://故障信息
		OnRecvFaultMsg(pBuff, nLen);
		break;
		break;
		//累计量
	case M_IT_NA_1: //累计量 ASDU15
		OnRecvKwh(pBuff, nLen);
		break;
	case M_IT_TA_1: //带短时标的累计量 ASDU16
		OnRecvKwhShortTime(pBuff, nLen);
		break;
	case M_IT_TB_1: //带长时标的累计量 ASDU37
		OnRecvKwhLongTime(pBuff, nLen);
		break;
	case C_IC_NA_1:// 总召唤命令
	{
		OnRecvGeneralCallConfirm(pBuff, nLen);
		break;
	}
	case M_EI_NA_1://子站初始化结束指令
	{
		CDeviceData* pDev = m_p101Impl->GetFCIModule()->GetDeviceInfo();

		Q_ASSERT(pDev);
		if (!pDev)
		{
			return false;
		}

		LogMsg(tr("initalize over").toLocal8Bit(), 0);
		pDev->m_bIsInited = true;

		pDev->m_nDeviceState = DEVICE_STATE_GENERALCALL;
	}
	break;
	case F_DR_TA_1:
	{
		break;
	}
	case F_FR_NA_1:
	{
		break;
	}
	case F_SR_NA_1:
	{
		break;
	}
	case F_SG_NA_1:
	{
		break;
	}
	case F_LS_NA_1:
	{
		break;
	}
	case F_AF_NA_1:
	{
		break;
	}
	case D_FILE_TRANSPORT:
	{
		OnRecvFileAnalyseInfo(pBuff, nLen);
		break;
	}
	case D_FIX_SWITCH:
	{
		//切换定值区号
		OnRecvSwitchFixArea(pBuff, nLen);
		break;
	}
	case D_FIX_SEARCH:
	{
		//获取当前定值区号
		OnrecvReadCurrentFixArea(pBuff, nLen);
		break;
	}
	case D_FIX_READ:
	{
		//定值读取
		OnRecvDevReadRequestAck(pBuff, nLen);
		break;
	}
	case D_FIX_WRITE:
	{
		OnRecvDevWriteRequestAck(pBuff, nLen);
		break;
	}
	default:
		break;
	}

	return true;
}

void C101Recver::LogMsg(const char* pszText, int nLevel)
{
	Q_ASSERT(pszText && strlen(pszText) > 0);
	if (!pszText || strlen(pszText) <= 0)
	{
		return;
	}
	Q_ASSERT(m_p101Impl);
	if (!m_p101Impl)
	{
		return;
	}
	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), pszText, 1);
}

bool C101Recver::LogE(int nType, char* pBuff, int nLength)
{
	//Tag
	std::string strTag("[E5]");

	std::string strBuff = MYLIB::FormatBuffer(pBuff, nLength);

	std::string strMsg = strTag + strBuff;

	LogMsg(pBuff, 1);

	return true;
}

bool C101Recver::LogF(int nType, char* pBuff, int nLength)
{
	AFCI* pAfci = (AFCI*)pBuff;

	char buf[256];
	if (pAfci->m_ctrl.m_PRM == DIRECTION_DOWN)//下行
		sprintf(buf, "[RFIX:][DeviceAddr:%d RES:%d FCB:%d FCV:%d FUNCCODE:%d]", pAfci->m_addr.GetAddr(), pAfci->m_ctrl.m_Dir, pAfci->m_ctrl.m_DATA, pAfci->m_ctrl.m_FCV, pAfci->m_ctrl.m_FUNCCODE);
	else//上行
		sprintf(buf, "[RFIX:][DeviceAddr:%d RES:%d ACD:%d DFC:%d FUNCCODE:%d]", pAfci->m_addr.GetAddr(), pAfci->m_ctrl.m_Dir, pAfci->m_ctrl.m_DATA, pAfci->m_ctrl.m_FCV, pAfci->m_ctrl.m_FUNCCODE);

	//数据
	QString strData(buf);
	//内存
	std::string strBuff = MYLIB::FormatBuffer(pBuff, nLength);
	//功能码描述
	QString strDesc = (pAfci->m_ctrl.m_PRM == DIRECTION_DOWN) ? GetDownwardFunCodeDesc(pAfci->m_ctrl.m_FUNCCODE) : GetDownwardFunCodeDesc(pAfci->m_ctrl.m_FUNCCODE);

	QString strMsg = strData + QString::fromStdString(strBuff) + strDesc;

	LogMsg(strMsg.toLocal8Bit(), 1);

	return true;
}

bool C101Recver::LogV(int nType, char* pBuff, int nLength)
{
	char* pAsduBuff = pBuff + sizeof(AVDU_HEAD);
	int nAsduLen = nLength - sizeof(AVDU_HEAD) - sizeof(AVDU_TAIL);

	ASDU_BASE* pAsduBase = (ASDU_BASE*)(pAsduBuff);

	char buf[128];
	sprintf(buf, "[RASDU%d]", pAsduBase->type);
	std::string strTag(buf);

	//数据
	std::string strBuff = MYLIB::FormatBuffer(pBuff, nLength);

	std::string strMsg = strTag + strBuff;

	LogMsg(strMsg.c_str(), 1);

	return true;
}

QString C101Recver::GetDownwardFunCodeDesc(int nFuncode)
{
	QString strDesc("");
	switch (nFuncode)
	{
	case 0:
		strDesc = QString::fromStdString("复位远方链路/确认报文信息");
		break;
	case 2:
		strDesc = QString::fromStdString("链路测试功能");
		break;
	case 3:
		strDesc = QString::fromStdString("发送确认用户数据");
		break;
	case 4:
		strDesc = QString::fromStdString("发送无回答用户数据");
		break;
	case 9:
		strDesc = QString::fromStdString("请求响应链路状态");
		break;
	case 11:
		strDesc = QString::fromStdString("响应链路状态");
		break;
	default:
		strDesc = QString::fromStdString("未知功能码");
		break;
	}
	strDesc = "[" + strDesc + "]";

	return strDesc;
}

void C101Recver::OnRecvFaultMsg(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	ASDUGZ* pAsduGZ = (ASDUGZ*)pBuff;

	emit Signal_MalFuction(*pAsduGZ);
}

void C101Recver::OnRecvFileAnalyseInfo(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_BASE_HEADER *pFileHeader = (FILE_BASE_HEADER*)pBuff;

	switch (pFileHeader->m_nOperatorType)
	{
	case E_FILE_CATALOG_RESPOND:
	{
		//目录召唤确认
		OnRecvCatalogRespond(pBuff, nLength);
		break;
	}
	case FILE_READ_ACTION_ACT:
	{
		//读取激活确认
		OnRecvFileAction(pBuff, nLength);
		break;
	}
	case FILE_READ_TRANSPORT:
	{
		//读文件数据传输
		OnRecvReadFileRespond(pBuff, nLength);
		break;
	}
	case FILE_READ_TRANSPORT_ACT:
	{
		//读取传输确认
		OnRecvReadConform(pBuff, nLength);
		break;
	}
	case FILE_WRITE_ACTION_ACT:
	{
		//写入激活确认
		OnRecvWriteAction(pBuff, nLength);
		break;
	}
	case FILE_WRITE_TRANSPORT:
	{
		break;
	}
	case FILE_WRITE_TRANSPORT_ACT:
	{
		break;
	}
	default:
		break;
	}
}

void C101Recver::OnRecvCatalogRespond(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	FILE_CATALOG_RESPOND *pCatalogRespond = (FILE_CATALOG_RESPOND*)pBuff;

	if (pCatalogRespond->m_nResultFlag == 1)
	{
		//失败
		QByteArray byDestr = tr("Dir Respond Reject").toLocal8Bit();
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
		m_p101Impl->getSender()->ClearFilepath();
		m_p101Impl->getSender()->SetOperatorFlag(0);
		return;
	}

	if (m_fileAttrInfo.m_nOperatorType == 9)
	{
		//只招目录
		return;
	}
	else if (m_fileAttrInfo.m_nOperatorType == 8)
	{

		//报文长度
		int nBagLength = pCatalogRespond->GetLength();
		//现有文件长度
		int nFileLengths = 0;

		for (int i = 0; i < pCatalogRespond->m_uFileCount; i++)
		{
			//文件名称长度
			int nFileNameLength = pBuff[nBagLength + nFileLengths];   //
																	  //文件名称
			QString strFilename = QString::fromLocal8Bit(pBuff + nBagLength + 1 + nFileLengths, nFileNameLength);
			//文件属性
			unsigned char uFileAttr = pBuff[nBagLength + 1 + nFileNameLength + nFileLengths];
			//文件大小
			INFOADDR4 *infoSize = (INFOADDR4*)(pBuff + nBagLength + 2 + nFileNameLength + nFileLengths);
			//时间
			CP56Time2a *catalogTime = (CP56Time2a *)(pBuff + nBagLength + 2 + nFileNameLength + nFileLengths + sizeof(INFOADDR4));

			nFileLengths += 2 + nFileNameLength + sizeof(INFOADDR4) + 7;

			int nFileSize = infoSize->GetAddr();


			Catalog_Info tFileAttr;

			tFileAttr.m_cFileNameLength = nFileNameLength;
			tFileAttr.m_strFileName = strFilename;
			tFileAttr.m_FileSize = nFileSize;
			tFileAttr.m_cFileAttr = uFileAttr;
			tFileAttr.m_strTimeFile = catalogTime->Dump();

			m_lstFiles.append(tFileAttr);
		}

		if (pCatalogRespond->m_uNextFlag == 0)
		{		
			emit Signal_FIleCatalogINfo(m_lstFiles);
		}

		return;
	}

}

void C101Recver::OnRecvFileAction(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvFileData(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvFileConform(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvWriteConform(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvUpdateActionAck(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvReadIecData(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvCatalogRepond(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvReadActionResond(char *pBuff, int nLenght)
{

}

void C101Recver::OnRecvReadFileRespond(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvReadConform(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvWriteAction(char *pBuff, int nLength)
{

}

void C101Recver::OnRecvSwitchFixArea(char *pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	ASDU200* pAsdu200 = (ASDU200*)pBuff;

	if (pAsdu200->cot.GetCot() == 7)
	{
		QByteArray byDestr = tr("Receive Switch Fix Area Response Scuss").toLocal8Bit();

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
	}
}

void C101Recver::OnrecvReadCurrentFixArea(char *pBuff, int nLength)
{
	//获取当前定值区号
	if (nLength <= 0)
	{
		return;
	}

	ASDU201_M* pAsdu201 = (ASDU201_M*)pBuff;

	if (pAsdu201->cot.GetCot() == 7)
	{
		QByteArray byDestr = tr("Current Fix Area Code is: %1").arg(pAsdu201->m_infoCurrentAreaIndex.GetAddr()).toLocal8Bit();

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
	}
}

void C101Recver::OnRecvDevReadRequestAck(char* pBuff, int nLength)
{
	Q_UNUSED(pBuff);
	Q_UNUSED(nLength);

	if (nLength == 0)
	{
		return;
	}
	//定值
	DEV_BASE pDevData;
	//
	ASDU_BASE* pAsdudz = (ASDU_BASE*)(pBuff);

	//asdu公共地址
	pDevData.m_nAsduID = pAsdudz->asduAddr.GetAddr();
	//传送原因
	pDevData.m_nCto = pAsdudz->cot.GetCot();

	//当前数据在所节点
	int nSetIndex = 0;
	//数据个数

	//数据长度
	int nPagLength = sizeof(ASDU_BASE) + sizeof(ASDUADDR2) + sizeof(unsigned char);

	int nItemCount = pAsdudz->vsq & 0x7f;

	for (int i = 0; i < nItemCount; i++)
	{
		DEV_DATA tDevData;
		//信息体地址
		INFOADDR3 *pAddr = (INFOADDR3*)(pBuff + nPagLength + nSetIndex);
		tDevData.nAddress = pAddr->GetAddr();

		//tag类型
		tDevData.nTagType = pBuff[nPagLength + nSetIndex + sizeof(INFOADDR3)];

		//数据长度
		tDevData.nLength = pBuff[nPagLength + nSetIndex + sizeof(INFOADDR3) + 1];
		//数据值
		if (tDevData.nTagType == 2)
		{
			//int
			int *pp = (int *)(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
			tDevData.strValue = QString::number(*pp);
		}
		else if (tDevData.nTagType == 45)
		{
			//ushort
			unsigned short *puShort = (unsigned short*)(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
			tDevData.strValue = QString::number(*puShort);

			if (tDevData.nAddress == 32774)
			{
				QString tStrValue = QString::number(*puShort, 16);
				if (tStrValue.length() == 1)
				{
					tStrValue = "000" + tStrValue;
				}
				else if (tStrValue.length() == 2)
				{
					tStrValue = "00" + tStrValue;
				}
				else if (tStrValue.length() == 3)
				{
					tStrValue = "0" + tStrValue;
				}
				tDevData.strValue = "0x" + tStrValue;
			}
		}
		else if (tDevData.nTagType == 38)
		{
			//float
			char *pfStart = pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2;

			float tt = 0.0;
			*(uint32_t *)(&tt) = (unsigned char)pfStart[0] + (unsigned char)pfStart[1] * 0x100 + (unsigned char)pfStart[2] * 0x10000 + (unsigned char)pfStart[3] * 0x1000000;

			tDevData.strValue = QString::number(tt);
		}
		else if (tDevData.nTagType == 4)
		{
			//string
			tDevData.strValue = QString::fromLocal8Bit(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2, tDevData.nLength);		

		}
		else if (tDevData.nTagType == 1)
		{
			//bool
			bool *pp = (bool *)(pBuff + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
			tDevData.strValue = QString::number(*pp);

		}

		pDevData.m_lstData.append(tDevData);

		nSetIndex += sizeof(INFOADDR3) + 2 + tDevData.nLength;
	}

	//发送信号 
	emit Signal_ReadFixData(pDevData);

	QByteArray byDestr = tr("定值读取成功!").toLocal8Bit();
	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);

}

void C101Recver::OnRecvDevWriteRequestAck(char *pBuff, int nLength)
{
	if (nLength == 0)
	{
		return;
	}

	if (nLength == 9)
	{
		//固化报文
		ASDU_BASE* pAsdudz = (ASDU_BASE*)pBuff;

		emit Signal_devWriteBack(pAsdudz->type, pAsdudz->cot.GetCot(), 0);
	}
	else
	{
		ASDU_BASE* pAsdudz = (ASDU_BASE*)pBuff;
		if (pAsdudz->cot.GetCot() == 7)
		{
			m_p101Impl->getSender()->SetGhFlag(m_p101Impl->getSender()->GetGhFlag() - 1);

			if (m_p101Impl->getSender()->GetGhFlag() == 0)
			{	
				m_p101Impl->getSender()->OnSendDevWriteConform();
			}

		}
		else
		{
			m_p101Impl->getSender()->SetGhFlag(0);
			QByteArray byDestr = tr("定值设置失败,错误码%1!").arg(pAsdudz->cot.GetCot()).toLocal8Bit();
			m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
		}
	}
}

