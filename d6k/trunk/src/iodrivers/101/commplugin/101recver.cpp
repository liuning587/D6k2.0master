
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
		case TYPE_E://���ַ�E5
			break;
		case TYPE_F://�̶�֡
			break;
		case TYPE_V:
		{
			if (m_101Cache.GetCurByteLength() == 1)//ֻ��һ�������ַ�
			{
				m_101Cache.CopyData(pBuff, 1);//���Ƶ�һ��������
				pBuff++;
				nLen--;
			}
		}
		break;
		}

		int nNeedBytes = m_101Cache.GetNeedByteLength();
		if (nNeedBytes <= 0)
			return true;
		if (nNeedBytes > nLen)//��Ҫ�ĳ��ȴ��ڽ��յ��ĳ���
		{
			m_101Cache.CopyData(pBuff, nLen);
			return true;
		}
		else//���յ��ĳ��ȴ�����Ҫ�ĳ���
		{
			//������Ҫ�Ĳ���
			m_101Cache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;
			AnalyseAvdu(m_101Cache.GetDataBuff(), m_101Cache.GetDataLen());
			m_101Cache.Clear();

			//����ʣ��Ĳ���
			return OnReceive(pBuff, nLen);
		}
	}
	else//������Ϊ��
	{
		//�ڽ�����Ѱ�������ַ�
		char* pNewBuff = m_101Cache.FindStartChar(pBuff, nLen);
		if (pNewBuff == NULL)
			return true;

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//���������ַ���������
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
	case 0xE5://���ַ�
		return AnalyseE5(pBuff, nLen);
	case 0x10://�̶�֡��
		return AnalyseF(pBuff, nLen);
	case 0x68://�ɱ�֡��
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
	//����������
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
	//����ASDU
	pBuff = pBuff + sizeof(AVDU_HEAD);
	nLen = nLen - sizeof(AVDU_HEAD) - sizeof(AVDU_TAIL);

	ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff);

	switch (pAsduBase->type)
	{
		//ң��
	case M_SP_NA_1: // ������Ϣ  ASDU1
		OnRecvBinarySinglePoint(pBuff, nLen);
		break;
	case M_DP_NA_1: // ˫����Ϣ   ASDU3
		OnRecvBinaryDoublePoint(pBuff, nLen);
		break;
	case M_PS_NA_1: // ���鵥����Ϣ   ASDU20
		OnRecvBinaryGroupPoint(pBuff, nLen);
		break;
	case M_SP_TA_1://����ʱ��ĵ�����Ϣ ASDU2
		OnRecvBinarySinglePointShortTime(pBuff, nLen);
		break;
	case M_SP_TB_1://����ʱ��ĵ�����Ϣ ASDU30
		OnRecvBinarySinglePointTime(pBuff, nLen);
		break;
	case M_DP_TA_1://����ʱ���˫����Ϣ ASDU4
		OnRecvBinaryDoublePointShortTime(pBuff, nLen);
		break;
	case M_DP_TB_1://����ʱ���˫����Ϣ ASDU31
		OnRecvBinaryDoublePointTime(pBuff, nLen);
		break;

		//ң��
	case M_ME_NA_1://��һ������ֵ ASDU9
		OnRecvAnalogNormalized(pBuff, nLen);
		break;
	case M_ME_TA_1://����ʱ��Ĺ�һ������ֵ ASDU10
		OnRecvAnalogNormalizedShortTime(pBuff, nLen);
		break;
	case M_ME_NB_1://��Ȼ�����ֵ ASDU11
		OnRecvAnalogScaled(pBuff, nLen);
		break;
	case M_ME_TB_1://����ʱ��ı�Ȼ�����ֵ ASDU12
		OnRecvAnalogScaledShortTime(pBuff, nLen);
		break;
	case M_ME_NC_1://�̸���������ֵ ASDU13
		OnRecvAnalogShortFloat(pBuff, nLen);
		break;
	case M_ME_TC_1://����ʱ��Ķ̸���������ֵ ASDU14
		OnRecvAnalogShortFloatShortTime(pBuff, nLen);
		break;
	case M_ME_ND_1://����Ʒ�������ʵĹ�һ������ֵ ASDU21
		OnRecvAnalogNormalizedNoQuality(pBuff, nLen);
		break;
	case M_ME_TD_1://����ʱ��Ĺ�һ������ֵ ASDU34
		OnRecvAnalogNormalizedLongTime(pBuff, nLen);
		break;
	case M_ME_TE_1://����ʱ��ı�Ȼ�����ֵ ASDU35
		OnRecvAnalogScaledLongTime(pBuff, nLen);
		break;
	case M_ME_TF_1://����ʱ��Ķ̸���������ֵ ASDU36
		OnRecvAnalogShortFloatLongTime(pBuff, nLen);
		break;
	case M_FT_NA_1://������Ϣ
		OnRecvFaultMsg(pBuff, nLen);
		break;
		break;
		//�ۼ���
	case M_IT_NA_1: //�ۼ��� ASDU15
		OnRecvKwh(pBuff, nLen);
		break;
	case M_IT_TA_1: //����ʱ����ۼ��� ASDU16
		OnRecvKwhShortTime(pBuff, nLen);
		break;
	case M_IT_TB_1: //����ʱ����ۼ��� ASDU37
		OnRecvKwhLongTime(pBuff, nLen);
		break;
	case C_IC_NA_1:// ���ٻ�����
	{
		OnRecvGeneralCallConfirm(pBuff, nLen);
		break;
	}
	case M_EI_NA_1://��վ��ʼ������ָ��
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
		//�л���ֵ����
		OnRecvSwitchFixArea(pBuff, nLen);
		break;
	}
	case D_FIX_SEARCH:
	{
		//��ȡ��ǰ��ֵ����
		OnrecvReadCurrentFixArea(pBuff, nLen);
		break;
	}
	case D_FIX_READ:
	{
		//��ֵ��ȡ
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
	if (pAfci->m_ctrl.m_PRM == DIRECTION_DOWN)//����
		sprintf(buf, "[RFIX:][DeviceAddr:%d RES:%d FCB:%d FCV:%d FUNCCODE:%d]", pAfci->m_addr.GetAddr(), pAfci->m_ctrl.m_Dir, pAfci->m_ctrl.m_DATA, pAfci->m_ctrl.m_FCV, pAfci->m_ctrl.m_FUNCCODE);
	else//����
		sprintf(buf, "[RFIX:][DeviceAddr:%d RES:%d ACD:%d DFC:%d FUNCCODE:%d]", pAfci->m_addr.GetAddr(), pAfci->m_ctrl.m_Dir, pAfci->m_ctrl.m_DATA, pAfci->m_ctrl.m_FCV, pAfci->m_ctrl.m_FUNCCODE);

	//����
	QString strData(buf);
	//�ڴ�
	std::string strBuff = MYLIB::FormatBuffer(pBuff, nLength);
	//����������
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

	//����
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
		strDesc = QString::fromStdString("��λԶ����·/ȷ�ϱ�����Ϣ");
		break;
	case 2:
		strDesc = QString::fromStdString("��·���Թ���");
		break;
	case 3:
		strDesc = QString::fromStdString("����ȷ���û�����");
		break;
	case 4:
		strDesc = QString::fromStdString("�����޻ش��û�����");
		break;
	case 9:
		strDesc = QString::fromStdString("������Ӧ��·״̬");
		break;
	case 11:
		strDesc = QString::fromStdString("��Ӧ��·״̬");
		break;
	default:
		strDesc = QString::fromStdString("δ֪������");
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
		//Ŀ¼�ٻ�ȷ��
		OnRecvCatalogRespond(pBuff, nLength);
		break;
	}
	case FILE_READ_ACTION_ACT:
	{
		//��ȡ����ȷ��
		OnRecvFileAction(pBuff, nLength);
		break;
	}
	case FILE_READ_TRANSPORT:
	{
		//���ļ����ݴ���
		OnRecvReadFileRespond(pBuff, nLength);
		break;
	}
	case FILE_READ_TRANSPORT_ACT:
	{
		//��ȡ����ȷ��
		OnRecvReadConform(pBuff, nLength);
		break;
	}
	case FILE_WRITE_ACTION_ACT:
	{
		//д�뼤��ȷ��
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
		//ʧ��
		QByteArray byDestr = tr("Dir Respond Reject").toLocal8Bit();
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
		m_p101Impl->getSender()->ClearFilepath();
		m_p101Impl->getSender()->SetOperatorFlag(0);
		return;
	}

	if (m_fileAttrInfo.m_nOperatorType == 9)
	{
		//ֻ��Ŀ¼
		return;
	}
	else if (m_fileAttrInfo.m_nOperatorType == 8)
	{

		//���ĳ���
		int nBagLength = pCatalogRespond->GetLength();
		//�����ļ�����
		int nFileLengths = 0;

		for (int i = 0; i < pCatalogRespond->m_uFileCount; i++)
		{
			//�ļ����Ƴ���
			int nFileNameLength = pBuff[nBagLength + nFileLengths];   //
																	  //�ļ�����
			QString strFilename = QString::fromLocal8Bit(pBuff + nBagLength + 1 + nFileLengths, nFileNameLength);
			//�ļ�����
			unsigned char uFileAttr = pBuff[nBagLength + 1 + nFileNameLength + nFileLengths];
			//�ļ���С
			INFOADDR4 *infoSize = (INFOADDR4*)(pBuff + nBagLength + 2 + nFileNameLength + nFileLengths);
			//ʱ��
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
	//��ȡ��ǰ��ֵ����
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
	//��ֵ
	DEV_BASE pDevData;
	//
	ASDU_BASE* pAsdudz = (ASDU_BASE*)(pBuff);

	//asdu������ַ
	pDevData.m_nAsduID = pAsdudz->asduAddr.GetAddr();
	//����ԭ��
	pDevData.m_nCto = pAsdudz->cot.GetCot();

	//��ǰ���������ڵ�
	int nSetIndex = 0;
	//���ݸ���

	//���ݳ���
	int nPagLength = sizeof(ASDU_BASE) + sizeof(ASDUADDR2) + sizeof(unsigned char);

	int nItemCount = pAsdudz->vsq & 0x7f;

	for (int i = 0; i < nItemCount; i++)
	{
		DEV_DATA tDevData;
		//��Ϣ���ַ
		INFOADDR3 *pAddr = (INFOADDR3*)(pBuff + nPagLength + nSetIndex);
		tDevData.nAddress = pAddr->GetAddr();

		//tag����
		tDevData.nTagType = pBuff[nPagLength + nSetIndex + sizeof(INFOADDR3)];

		//���ݳ���
		tDevData.nLength = pBuff[nPagLength + nSetIndex + sizeof(INFOADDR3) + 1];
		//����ֵ
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

	//�����ź� 
	emit Signal_ReadFixData(pDevData);

	QByteArray byDestr = tr("��ֵ��ȡ�ɹ�!").toLocal8Bit();
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
		//�̻�����
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
			QByteArray byDestr = tr("��ֵ����ʧ��,������%1!").arg(pAsdudz->cot.GetCot()).toLocal8Bit();
			m_p101Impl->GetFCIModule()->GetMainModule()->LogString(m_p101Impl->GetFCIModule()->GetDeviceName().toStdString().c_str(), byDestr.data(), 1);
		}
	}
}

