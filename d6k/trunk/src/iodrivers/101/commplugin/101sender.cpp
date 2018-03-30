
#include "101sender.h"
#include "commplugin.h"
#include "asdu_info.h"
#include "101recver.h"
#include "devicestudio/main_module.h"
#include "ftu_module.h"
std::string FormatBuffer(char* pBuff, int nMsgLen);
/*
{
	std::string strDebugData("");
	char szByte[10] = { '\0' };

	for (int i = 0; i < nMsgLen; i++)
	{
		sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
		strDebugData += szByte;
	}
	return strDebugData;
}*/

C101Sender::C101Sender(QObject* parent):QObject(parent)
{
	m_p101Impl = qobject_cast<CCommPlugin*>(parent);
	Q_ASSERT(m_p101Impl);
	if (!m_p101Impl)
	{
		return;
	}
}

C101Sender::~C101Sender()
{

}

bool C101Sender::OnSend()//�����߼�
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
		Q_ASSERT(pDeviceData);
		if (!pDeviceData)
		{
			return false;
		}
       
		bool  bRet = false;
		switch(pDeviceData->m_nDeviceState)
        {
        case DEVICE_STATE_UNKNOWN://װ�ô��ڳ�ʼ̬
			return  SendRequireLinkState();//������·״̬
			break;         
        case DEVICE_STATE_LINKSTATE://��·��λ
				return SendRequireLinkReset();
                break;
		case DEVICE_STATE_LINKRESET://��������·״̬����������
			    break;
		case CONTROL_STATE_LINKSTATE: //�ظ���·����
				return SendReplyLinkState();
				break;
		case CONTROL_STATE_LINKRESET://�ظ���·��λ
				return SendReplyLinkReset();
				break;    
        case DEVICE_STATE_GENERALCALL://
                return SendGeneralCall();
                break;
        case DEVICE_STATE_GENERALCALL_OVER://���ٽ���
                break;
        case DEVICE_STATE_SYNCTIME://��ʱ
                return SendSyncTime();
                break;
		case DEVICE_STATE_SYNCTIME_RDTIME:
				return SendSyncRdTime();
				break;
		case DEVICE_STATE_HEART:
				return SendHeartBeat();
				break;
		case DEVICE_STATE_NONE:
				//return SendConfirm();

				break;
        }
        return false;
}

bool C101Sender::LogE(int nType, char* pBuff, int nLength)
{
	//Tag
	std::string strTag("[E5]");

	std::string strBuff = FormatBuffer(pBuff, nLength);

	std::string strMsg = strTag + strBuff;

	LogMsg(pBuff, 1);

	return true;
}

bool C101Sender::LogF(int nType, char* pBuff, int nLength)
{
	AFCI* pAfci = (AFCI*)pBuff;

	char buf[256];
	if (pAfci->m_ctrl.m_PRM == DIRECTION_DOWN)//����
		sprintf(buf, "[SFIX:][DeviceAddr:%d RES:%d FCB:%d FCV:%d FUNCCODE:%d]", pAfci->m_addr.GetAddr(), pAfci->m_ctrl.m_Dir, pAfci->m_ctrl.m_DATA, pAfci->m_ctrl.m_FCV, pAfci->m_ctrl.m_FUNCCODE);
	else//����
		sprintf(buf, "[SFIX:][DeviceAddr:%d RES:%d ACD:%d DFC:%d FUNCCODE:%d]", pAfci->m_addr.GetAddr(), pAfci->m_ctrl.m_Dir, pAfci->m_ctrl.m_DATA, pAfci->m_ctrl.m_FCV, pAfci->m_ctrl.m_FUNCCODE);

	//����
	QString strData(buf);
	//�ڴ�
	std::string strBuff = FormatBuffer(pBuff, nLength);
	//����������
	QString strDesc;// = (pAfci->m_ctrl.m_PRM == DIRECTION_DOWN) ? GetDownwardFunCodeDesc(pAfci->m_ctrl.m_FUNCCODE) : GetDownwardFunCodeDesc(pAfci->m_ctrl.m_FUNCCODE);

	QString strMsg = strData + QString::fromStdString(strBuff) + strDesc;

	LogMsg(strMsg.toLocal8Bit(), 1);

	return true;
}

bool C101Sender::LogV(int nType, char* pBuff, int nLength)
{
	char* pAsduBuff = pBuff + sizeof(AVDU_HEAD);
	int nAsduLen = nLength - sizeof(AVDU_HEAD) - sizeof(AVDU_TAIL);

	ASDU_BASE* pAsduBase = (ASDU_BASE*)(pAsduBuff);

	char buf[128];
	sprintf(buf, "[SASDU%d]", pAsduBase->type);
	std::string strTag(buf);

	//����
	std::string strBuff = FormatBuffer(pBuff, nLength);

	std::string strMsg = strTag + strBuff;

	LogMsg(strMsg.c_str(), 1);

	return true;
}

void C101Sender::LogMsg(const char*pszText, int nLevel)
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

int  C101Sender::OnControl(NBM_TELECTRL* pTelectrl)//ң��
{
        int nResult = HAS_NOTHING;

        switch(pTelectrl->m_nCtrlType)
        {
        case TELECTRL_REQUEST_KWHGENERALCALL:// ���������ٻ�����
                break;
        case TELECTRL_REQUEST_GENERALCALL:// ���ٻ�����
                {
                        //ֻ�ǰ�װ�õ�״̬��Ϊ��·��λ����,�·�������Sender��OnSend����
                        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
                        pDeviceData->m_nDeviceState = DEVICE_STATE_GENERALCALL;
                        return HAS_COMMAND;
                }
                break;
        case TELECTRL_REQUEST_SYNCTIME://ʱ��ͬ��
                {
                        //ֻ�ǰ�װ�õ�״̬��Ϊ���ٽ���,�·�������Sender��OnSend����
                        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
                        pDeviceData->m_nDeviceState = DEVICE_STATE_SYNCTIME;
                        return HAS_COMMAND;
                }
                break;
        case TELECTRL_REQUEST_SELECT:
        case TELECTRL_REQUEST_EXECUTE:
        case TELECTRL_REQUEST_UNSELECT:
                {
                        int nCommandType = pTelectrl->m_nCommandType;
                        if(nCommandType != C_SE_NA_1 && nCommandType != C_SE_NB_1 && nCommandType != C_SE_NC_1
                                && nCommandType != C_SC_NA_1 && nCommandType != C_DC_NA_1)
                        {
                          /*      if(command.m_nDataType == DATATYPE_ANALOG)
                                        nCommandType = m_pImpl->GetParam()->m_nAnalogTelectrlCommand;
                                else
                                        nCommandType = m_pImpl->GetParam()->m_nBinaryTelectrlCommand;
              */          }
                        bool IsSendOk = false;
                        switch(nCommandType)
                        {
                        case C_SE_NA_1://��һ�����48
                                IsSendOk = OnSendSetAnalogNormalizeRequest(*pTelectrl);
                                break;
                        case C_SE_NB_1://��Ȼ����49
                                IsSendOk = OnSendSetAnalogScaleRequest(*pTelectrl);
                                break;
                        case C_SE_NC_1://�̸��������50
                                IsSendOk = OnSendSetAnalogShortFloatRequest(*pTelectrl);
                                break;
                        case C_SC_NA_1://��������45
                                IsSendOk = OnSendSetBinarySPRequest(*pTelectrl);
                                break;
                        case C_DC_NA_1://˫������46
                                IsSendOk = OnSendSetBinaryDPRequest(*pTelectrl);
                                break;
                        default:
                                break;
                        }
                        if(IsSendOk)
                                return HAS_TELECTRL;
                }
                break;
        }       
        
        return nResult;
}

bool  C101Sender::OnTimer()//��ʱ��
{
	return true;
}

bool C101Sender::Send_F(char* pBuff,int nLen)
{
        AFCI* pAfci = (AFCI*)pBuff;

		pAfci->m_ctrl.m_Dir = 0;

        
        //����У����
		unsigned char chkSum = CheckSum(pBuff+1,2);

		pAfci->m_chksum = chkSum; 
       
		LogF(1,pBuff,nLen);
               
		return m_p101Impl->WriteApduData(pBuff, nLen);
}

bool C101Sender::Send_V(char* pBuff,int nLen)
{
        //1����֯AVDUͷ
        AVDU_HEAD* pHead = (AVDU_HEAD*)pBuff;
        //�����ַ�
        pHead->m_head1 = 0x68;
        pHead->m_head2 = 0x68;
        //����
        pHead->SetDataLength(nLen);

		pHead->m_ctrl.m_Dir = 0;
    
        //2����֯AVDUβ
        AVDU_TAIL* pTail = (AVDU_TAIL*)(pBuff + sizeof(AVDU_HEAD) + nLen);

		unsigned char chkSum = CheckSum(pBuff+4,nLen+sizeof LINKADDR +1);
        pTail->m_chksum = chkSum;
        pTail->m_tail = 0x16;
        
        nLen = sizeof(AVDU_HEAD) + nLen + sizeof(AVDU_TAIL);

		LogV(1,pBuff,nLen);

        return m_p101Impl->WriteApduData(pBuff,nLen);
}

char C101Sender::CheckSum(char *buf, short len)
{
	unsigned char checkSum = 0;
	int i;

	for (i = 0; i < len; i++)
	{
		checkSum += buf[i];
	}

	return checkSum % 256;
}

bool C101Sender::SendSyncRdTime()
{
	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return false;
	}
	char buf[BUFFER_SIZE_101];

	//��·������
	AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
	pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
	pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB;
	pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
	pHead->m_ctrl.m_FCV = 1;
	pHead->m_ctrl.m_FUNCCODE = 3;
	//��·��ַ
	pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	//��֯ASDU103
	ASDU103* pAsdu103 = (ASDU103*)((char*)buf + sizeof(AVDU_HEAD));

	pAsdu103->type = C_CS_NA_1;
	pAsdu103->vsq = 0x01;

	pAsdu103->cot.SetCot(COT_REQ);
	pAsdu103->asduAddr.SetAddr(pDeviceData->m_nDeviceAddr);
	pAsdu103->m_infoaddr.SetAddr(0x00);

	struct CP56Time2a* cp56time2a = (struct CP56Time2a*)(&(pAsdu103->m_time));

	QDateTime tvTime = QDateTime::currentDateTime();
	cp56time2a->Initialize(tvTime.date().year(), tvTime.date().month(), tvTime.date().day(), tvTime.time().hour(), tvTime.time().hour(), tvTime.time().second(), tvTime.time().msec());

	return Send_V((char*)(buf), sizeof(ASDU103));

}

bool C101Sender::SendConfirm()
{
	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return false;
	}

	AFCI afci;

	//��·������
	afci.m_ctrl.m_Dir = 0;
	afci.m_ctrl.m_PRM = 0;
	afci.m_ctrl.m_DATA = 0;
	afci.m_ctrl.m_FCV = 0;
	afci.m_ctrl.m_FUNCCODE = 0;

	pDeviceData->m_nFCB = !pDeviceData->m_nFCB;

	//��·��ַ
	afci.m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	return Send_F((char*)(&afci), sizeof(afci));
}

bool C101Sender::SendHeartBeat()
{
	AFCI afci;

	//��·������
	afci.m_ctrl.m_Dir = DIRECTION_UP;
	afci.m_ctrl.m_DATA = 0;
	afci.m_ctrl.m_PRM = 0;
	afci.m_ctrl.m_FCV = 0;
	afci.m_ctrl.m_FUNCCODE = 2;

	//��·��ַ
	afci.m_addr.SetAddr(1);

	Send_F((char*)(&afci), sizeof(afci));

	return true;
}

bool C101Sender::SendResetProcess(NBM_TELECTRL* pTelectrl)
{
	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return false;
	}
	char buf[BUFFER_SIZE_101];

	//��·������
	AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
	pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
	pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB;
	pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
	pHead->m_ctrl.m_FCV = 1;
	pHead->m_ctrl.m_FUNCCODE = 3;
	//��·��ַ
	pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	//��֯ASDU103
	ASDU105* pAsdu105 = (ASDU105*)((char*)buf + sizeof(AVDU_HEAD));

	pAsdu105->type = C_RP_NA_1;
	pAsdu105->vsq = 0x01;

	pAsdu105->cot.SetCot(COT_ACT);
	pAsdu105->asduAddr.SetAddr(pDeviceData->m_nDeviceAddr);
	pAsdu105->m_infoaddr.SetAddr(0x00);
	pAsdu105->m_qcc = 0x01;

	return  Send_V((char*)(buf), sizeof(ASDU105));

}

bool C101Sender::SendReplyLinkReset()
{
	AFCI afci;

	//��·������
	afci.m_ctrl.m_Dir = DIRECTION_UP;
	afci.m_ctrl.m_DATA = 0;
	afci.m_ctrl.m_PRM = 0;
	afci.m_ctrl.m_FCV = 0;
	afci.m_ctrl.m_FUNCCODE = 0;

	//��·��ַ
	afci.m_addr.SetAddr(1);

	Send_F((char*)(&afci), sizeof(afci));

	return true;
}

bool C101Sender::SendReplyLinkState()
{
	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();

	AFCI afci;

	//��·������
	afci.m_ctrl.m_Dir = DIRECTION_UP;
	afci.m_ctrl.m_DATA = 0;//ACD
	afci.m_ctrl.m_PRM = 0;
	afci.m_ctrl.m_FUNCCODE = 11;

	//��·��ַ
	afci.m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

	Send_F((char*)(&afci), sizeof(afci));
	return true;
}

bool C101Sender::OnSendGetCatalogRequest(const FILE_CATALOG_REQUEST_1 &lbCatalog)
{
	char buf[255] = { 0 };

	if (m_nCurrentOperFlag == 1)
	{
		QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("�ļ�������,���Ժ�����").toLocal8Bit().data(), 1);

		return true;

	}

	if (!m_strFilePath.isEmpty())
	{
		QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("other file is loading, please wait!").toLocal8Bit().data(), 1);

		return true;

	}

	//
	m_p101Impl->GetRecver()->ClearFileInfos();
	//��֯
	FILE_CATALOG_REQUEST_1* pAsduCatalog = (FILE_CATALOG_REQUEST_1*)(buf + sizeof(AVDU_HEAD));

	pAsduCatalog->type = lbCatalog.type;
	pAsduCatalog->vsq = 0x01;

	pAsduCatalog->cot.SetCot(COT_REQ);

	pAsduCatalog->asduAddr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());

	pAsduCatalog->m_asduObjID.SetAddr(0);
	pAsduCatalog->m_addrType = 2;    //�ļ�����

	pAsduCatalog->m_nOperatorType = lbCatalog.m_nOperatorType;
	//Ŀ¼id
	pAsduCatalog->m_nCatalogID = lbCatalog.m_nCatalogID;
	//Ŀ¼����
	pAsduCatalog->m_uCatalogLength = lbCatalog.m_uCatalogLength;
	// Ŀ¼����
	strcpy(pAsduCatalog->m_cCatalogName, lbCatalog.m_cCatalogName);

	//��Ϣ��2
	FILE_CATALOG_REQUEST_2 *pCatalogFirst = (FILE_CATALOG_REQUEST_2 *)(buf + sizeof(AVDU_HEAD) + pAsduCatalog->GetAsduLength());

	pCatalogFirst->m_nCallFlag = pAsduCatalog->m_nCallFlag;
	pCatalogFirst->m_dateStart = pCatalogFirst->m_dateStart;
	pCatalogFirst->m_dataEnd = pCatalogFirst->m_dataEnd;

	//
	m_p101Impl->GetRecver()->SetFileInfo(lbCatalog.m_fileInfo);

	if (lbCatalog.m_fileInfo.m_cFileAttr == 9)
	{
		//�ļ����䲻ˢ��Ŀ¼
		OnSendReadFileAction(lbCatalog.m_fileInfo);
		ClearFilepath();
	}
	else
	{
		int nResult = Send_V(buf, pAsduCatalog->GetAsduLength() + sizeof(FILE_CATALOG_REQUEST_2));

		QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();
		if (nResult != SEND_OK)
		{
			m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Catalog Request Failed").toLocal8Bit().data(), 1);

			return false;
		}

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Search Catalog Request  Success").toLocal8Bit().data(), 1);
	}

	return true;
}

bool C101Sender::OnSendReadFileAction(const FILE_ATTR_INFO& fileAttr)
{
	//Ŀ¼�ļ���������
	if (fileAttr.m_cFileAttr == 1)
	{
		return true;
	}

	if (m_nCurrentOperFlag == 1)
	{
		QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("�ļ�������,���Ժ�����").toLocal8Bit().data(), 1);

		return true;

	}


	//���ɲ���
	m_nCurrentOperFlag = 1;
	//m_pComm104Pln->GetRecver()->InsertToFileLists(fileAttr);

	char buf[255] = { 0 };

	//���ļ�����
	FILE_BASE* pAsduCatalog = (FILE_BASE*)(buf + sizeof(AVDU_HEAD));


	pAsduCatalog->type = fileAttr.type;
	pAsduCatalog->asduAddr = fileAttr.asduAddr;
	pAsduCatalog->vsq = 0x01;

	//file head
	pAsduCatalog->m_asduObjID.SetAddr(0);
	pAsduCatalog->m_addrType = 2;         //�ļ�����
										  //TODO

	pAsduCatalog->cot.SetCot(COT_ACT);

	pAsduCatalog->asduAddr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());


	//��Ϣ��
	char *pReadActionData = buf + sizeof(AVDU_HEAD) + sizeof(FILE_BASE);

	pReadActionData[0] = 3;          //���ļ�����

	pReadActionData[1] = fileAttr.m_cFileNameLength;    //�ļ�����

														//�ļ�����
	strncpy(pReadActionData + 2, fileAttr.m_strFileName.toLocal8Bit().data(), fileAttr.m_cFileNameLength);


	int nResult = Send_V(buf, sizeof(FILE_BASE) + 2 + fileAttr.m_cFileNameLength);

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Read Action Request Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Read Action Request  Success").toLocal8Bit().data(), 1);

	return true;
}

bool C101Sender::OnSendWriteFileAction(const FILE_ATTR_INFO& fileAttr)
{
	m_btWriteData = fileAttr.m_btArrayData;

	m_nStartNode = 0;

	if (m_nCurrentOperFlag == 1)
	{
		QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();

		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("�ļ�������,���Ժ�����").toLocal8Bit().data(), 1);

		return true;

	}
	m_nCurrentOperFlag = 1;


	char buf[255] = { 0 };

	//���ļ�����
	FILE_BASE* pFileAction = (FILE_BASE*)(buf + sizeof(AVDU_HEAD));

	//file head
	pFileAction->m_asduObjID.SetAddr(0);
	pFileAction->m_addrType = 2;         //�ļ�����

	pFileAction->type = fileAttr.type;    //�ļ�����code
	pFileAction->vsq = 0x01;

	pFileAction->cot.SetCot(COT_ACT);

	pFileAction->asduAddr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());


	//��Ϣ��
	char *pReadActionData = buf + sizeof(AVDU_HEAD) + sizeof(FILE_BASE);

	pReadActionData[0] = 7;
	pReadActionData[1] = fileAttr.m_cFileNameLength;

	strncpy(pReadActionData + 2, fileAttr.m_strFileName.toLocal8Bit().data(), fileAttr.m_cFileNameLength);

	//�ļ�id  TODO
	INFOADDR4 *infoFileID = (INFOADDR4*)(pReadActionData + 2 + fileAttr.m_cFileNameLength);

	//�ļ���С
	INFOADDR4 *infoFileSize = (INFOADDR4*)(pReadActionData + 2 + fileAttr.m_cFileNameLength + sizeof(INFOADDR4));
	infoFileSize->SetAddr(fileAttr.m_FileSize);


	int nResult = Send_V(buf, sizeof(FILE_BASE) + 2 + fileAttr.m_cFileNameLength + sizeof(INFOADDR4) * 2);

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request  Success").toLocal8Bit().data(), 1);

	return true;
}

bool C101Sender::OnSendWriteFileData()
{
	//�ְ������ļ�
	if (m_btWriteData.isEmpty())
	{
		m_nStartNode = 0;
		//m_pFileTransfor->stop();
		return true;
	}
//	if (!m_pFileTransfor->isActive())
	{
//		m_pFileTransfor->start();
	}


	int nMaxDataSize = MAX_ASDU_SIZE - sizeof(FILE_BASE) - 11;

	int nBagCount = m_btWriteData.size() % nMaxDataSize == 0 ? m_btWriteData.size() / nMaxDataSize : m_btWriteData.size() / nMaxDataSize + 1;

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();


	for (int i = 0; i < nBagCount; i++)
	{
		char buf[255] = { 0 };

		//���ļ�����
		FILE_BASE* pFileAction = (FILE_BASE*)(buf + sizeof(AVDU_HEAD));

		//file head
		pFileAction->m_asduObjID.SetAddr(0);
		pFileAction->m_addrType = 2;         //�ļ�����

		pFileAction->type = D_FILE_TRANSPORT;    //�ļ�����code
		pFileAction->vsq = 0x01;

		pFileAction->cot.SetCot(COT_REQ);

		pFileAction->asduAddr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());


		//��Ϣ��
		char *pReadActionData = buf + sizeof(AVDU_HEAD) + sizeof(FILE_BASE);

		pReadActionData[0] = 9;     //д�ļ�����

									//�ļ�id	
		INFOADDR4 *infoFileID = (INFOADDR4*)(pReadActionData + 1);
		infoFileID->SetAddr(0);
		//���ݶκ�
		INFOADDR4 *infoNodeID = (INFOADDR4*)(pReadActionData + 1 + sizeof(INFOADDR4));
		infoNodeID->SetAddr(m_nStartNode);

		//������ʶ
		int nResult = 0;
		int nCurrentLength = 0;

		if (i == nBagCount - 1)
		{
			//���һ����
			pReadActionData[1 + sizeof(INFOADDR4) * 2] = 0;

			memcpy(pReadActionData + 2 + sizeof(INFOADDR4) * 2, m_btWriteData.mid(i*nMaxDataSize), m_btWriteData.size());

			nCurrentLength = m_btWriteData.size() - nMaxDataSize*i;

			m_btWriteData.clear();

		}
		else
		{
			qDebug() << m_btWriteData.mid(i*nMaxDataSize, nMaxDataSize).toHex();

			//�м��
			pReadActionData[1 + sizeof(INFOADDR4) * 2] = 1;

			memcpy(pReadActionData + 2 + sizeof(INFOADDR4) * 2, m_btWriteData.mid(i*nMaxDataSize, nMaxDataSize).data(), nMaxDataSize);

			//��У��
			nCurrentLength = nMaxDataSize;

			m_btWriteData.remove(0, i*nMaxDataSize + nMaxDataSize);
			m_nStartNode += nMaxDataSize;
		}
		qDebug() << QByteArray(pReadActionData, 100).toHex();
		qDebug() << QByteArray(buf, sizeof(FILE_BASE) + 2 + sizeof(INFOADDR4) * 2 + nCurrentLength + 1).toHex();

		nResult = Send_V(buf, sizeof(FILE_BASE) + 2 + sizeof(INFOADDR4) * 2 + nCurrentLength + 1);



		if (nResult != SEND_OK)
		{
			m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request Failed").toLocal8Bit().data(), 1);
		}

		break;

	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Write Action Request  Success").toLocal8Bit().data(), 1);

	return true;
}

unsigned char C101Sender::checkAllData(const char *data, int length)
{
	unsigned char lValue = 0;
	for (int i = 0; i < length; i++)
	{
		lValue += data[i];
	}
	return lValue;
}

bool C101Sender::OnSendDevDataRequest(DEV_BASE *pRequestDz)
{
	if (pRequestDz->m_nCommandType == D_FIX_READ)         //��ֵ��ȡ
	{
		char buf[255] = { 0 };

		//��֯ASDU46
		ASDUDZ* pAsdudz = (ASDUDZ*)(buf + sizeof(AVDU_HEAD));

		pAsdudz->type = pRequestDz->m_nCommandType;

		pAsdudz->vsq = 0x01;


		pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//����ԭ��
		pAsdudz->cot.SetCot(pRequestDz->m_nCto);

		//��ֵ����
		pAsdudz->infoFixCode.SetAddr(m_p101Impl->GetFCIModule()->GetFixCode());

		int nSetIndex = 0;

		for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
		{
			//��ֵ

			pAsdudz->m_data[nSetIndex].infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);


			if ((i + 1) % (pAsdudz->MAX_DATA_PER_ASDUDZ_RD) == 0)
			{
				//���ﵽ�����ʱ  �������ݰ�
				pAsdudz->SetItemCount(nSetIndex + 1);
				int nResult = Send_V(buf, sizeof(ASDUDZ));
				if (nResult != SEND_OK)
				{
					return false;
				}
				//�������
				memset(pAsdudz->m_data, 0, pAsdudz->MAX_DATA_PER_ASDUDZ_RD);
				nSetIndex = 0;
			}

			nSetIndex++;
		}

		if (nSetIndex != pAsdudz->MAX_DATA_PER_ASDUDZ_RD)
		{
			pAsdudz->SetItemCount(nSetIndex);

			int nResult = Send_V(buf, pAsdudz->GetAsduDzLength());

			if (nResult != SEND_OK)
			{
				return false;
			}
		}
	}
	else if (pRequestDz->m_nCommandType == D_FIX_WRITE)
	{

		m_nGhFlag = 0;

		m_nDevIndex = 0;
		m_WriteDevInfo.m_lstData.clear();
		m_WriteDevInfo = *pRequestDz;
		//��ֵд��
		char buf[255] = { 0 };

		//��ֵд��
		ASDU_BASE* pAsdudz = (ASDU_BASE*)(buf + sizeof(AVDU_HEAD));

		pAsdudz->type = pRequestDz->m_nCommandType;

		pAsdudz->vsq = pRequestDz->m_lstData.count();

		//asdu������ַ
		pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//����ԭ��
		pAsdudz->cot.SetCot(pRequestDz->m_nCto);

		//��ֵ����
		ASDUADDR2 *pAreaCode = (ASDUADDR2*)(buf + sizeof(AVDU_HEAD) + sizeof(ASDU_BASE));
		pAreaCode->SetAddr(m_p101Impl->GetFCIModule()->GetFixCode());

		//����������ʶ   TODO.....................
		DVFLAG *pFlag = (DVFLAG*)(buf + sizeof(AVDU_HEAD) + sizeof(ASDU_BASE) + sizeof(ASDUADDR2));
		pFlag->CONT = 0;
		pFlag->RES = 0;
		pFlag->CR = 0;
		pFlag->SE = 1;
		//��ǰ���������ڵ�
		int nSetIndex = 0;
		//���ݸ���
		int nNumbers = 0;
		//���ݳ���
		int nPagLength = sizeof(AVDU_HEAD) + sizeof(ASDU_BASE) + sizeof(ASDUADDR2) + sizeof(unsigned char);

		for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
		{
			if (nPagLength + nSetIndex + sizeof(INFOADDR3) + 2 + pRequestDz->m_lstData.at(i).nLength >= 255)
			{
				m_nDevIndex = nNumbers;
				m_nGhFlag++;
				Send_V(buf, nSetIndex + nPagLength - sizeof(AVDU_HEAD));
		
				nSetIndex = 0;
				return true;
			}

			nNumbers++;
			pAsdudz->vsq = nNumbers;

			//��Ϣ���ַ
			INFOADDR3 *pAddr = (INFOADDR3*)(buf + nPagLength + nSetIndex);

			pAddr->SetAddr(pRequestDz->m_lstData.at(i).nAddress);

			//tag����
			buf[nPagLength + nSetIndex + sizeof(INFOADDR3)] = pRequestDz->m_lstData.at(i).nTagType;

			//���ݳ���
			buf[nPagLength + nSetIndex + sizeof(INFOADDR3) + 1] = pRequestDz->m_lstData.at(i).nLength;
			//����ֵ

			if (pRequestDz->m_lstData.at(i).nTagType == 2)
			{
				//int
				int *pp = (int *)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
				*pp = pRequestDz->m_lstData.at(i).strValue.toInt();
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 45)
			{
				//u short
				unsigned short *puData = (unsigned short *)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
				*puData = pRequestDz->m_lstData.at(i).strValue.toUShort();
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 4)
			{
				//string
				strncpy(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2, pRequestDz->m_lstData.at(i).strValue.toLocal8Bit().data(), pRequestDz->m_lstData.at(i).nLength);
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 1)
			{
				//bool
				bool *pp = (bool *)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
				*pp = pRequestDz->m_lstData.at(i).strValue.toInt();
			}
			else if (pRequestDz->m_lstData.at(i).nTagType == 38)
			{
				//float
				unsigned char *pp = (unsigned char*)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);

				float tt = pRequestDz->m_lstData.at(i).strValue.toFloat();

				pp[0] = LSB(LSW(*(uint32_t *)(&tt)));
				pp[1] = MSB(LSW(*(uint32_t *)(&tt)));
				pp[2] = LSB(MSW(*(uint32_t *)(&tt)));
				pp[3] = MSB(MSW(*(uint32_t *)(&tt)));

			}
			else
			{
				char *ppp = (char*)(buf + nPagLength + nSetIndex + sizeof(INFOADDR3) + 2);
				memset(ppp, 0, pRequestDz->m_lstData.at(i).nLength);;
			}
			nSetIndex += sizeof(INFOADDR3) + 2 + pRequestDz->m_lstData.at(i).nLength;
		}

		m_nDevIndex = pRequestDz->m_lstData.count();
		int nResult = Send_V(buf, nSetIndex + nPagLength - sizeof(AVDU_HEAD));

		m_nGhFlag++;
		if (nResult != SEND_OK)
		{
			return false;
		}
		return true;
	}
	return true;
}

bool C101Sender::OnSendIecDataRequest(IEC_BASE *pRequestDz)
{
	if (pRequestDz->m_nCommandType == 138)
	{
		//��������
		char buf[255] = { 0 };

		//��֯ASDU46
		ASDU_IEC* pAsdudz = (ASDU_IEC*)(buf + sizeof(AVDU_HEAD));

		pAsdudz->type = pRequestDz->m_nCommandType;

		pAsdudz->vsq = 0x01;


		pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//����ԭ��
		pAsdudz->cot.SetCot(pRequestDz->m_nCto);

		int nSetIndex = 0;

		for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
		{
			//��ֵ
			if (0 == nSetIndex)
			{
				pAsdudz->infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);
			}
			//��ֵ
			pAsdudz->m_data[nSetIndex].m_nValue = pRequestDz->m_lstData.at(i).nValue;

			if ((i + 1) % (pAsdudz->MAX_DATA_PER_ASDUDZ_RD) == 0)
			{
				//���ﵽ�����ʱ  �������ݰ�
				pAsdudz->SetItemCount(nSetIndex + 1);

				buf[sizeof(AVDU_HEAD) + sizeof(ASDU_IEC)] = 0;
				int nResult = Send_V(buf, sizeof(ASDU_IEC) + 1);
				if (nResult != SEND_OK)
				{
					return false;
				}
				//�������
				memset(pAsdudz->m_data, 0, pAsdudz->MAX_DATA_PER_ASDUDZ_RD);
				nSetIndex = 0;
			}
			else
			{
				nSetIndex++;
			}


		}

		if (nSetIndex != pAsdudz->MAX_DATA_PER_ASDUDZ_RD && nSetIndex != 0)
		{
			pAsdudz->SetItemCount(nSetIndex);
			buf[sizeof(AVDU_HEAD) + pAsdudz->GetAsduDzLength()] = 0;
			int nResult = Send_V(buf, pAsdudz->GetAsduDzLength() + 1);

			if (nResult != SEND_OK)
			{
				return false;
			}
		}
	}
	else if (pRequestDz->m_nCommandType == 132)
	{
		//��ȡ����
		char buf[255] = { 0 };

		//��֯ASDU46
		ASDUDZ2* pAsdudz = (ASDUDZ2*)(buf + sizeof(AVDU_HEAD));

		pAsdudz->type = pRequestDz->m_nCommandType;

		pAsdudz->vsq = 0x01;


		pAsdudz->asduAddr.SetAddr(pRequestDz->m_nAsduID);
		//����ԭ��
		pAsdudz->cot.SetCot(pRequestDz->m_nCto);

		int nSetIndex = 0;

		for (int i = 0; i < pRequestDz->m_lstData.count(); i++)
		{
			//��ֵ
			if (i == 0)
			{
				pAsdudz->m_data[nSetIndex].infoaddr.SetAddr(pRequestDz->m_lstData.at(i).nAddress);
			}


			if ((i + 1) % (pAsdudz->MAX_DATA_PER_ASDUDZ2_RD) == 0)
			{
				//���ﵽ�����ʱ  �������ݰ�
				pAsdudz->SetItemCount(nSetIndex + 1);
				int nResult = Send_V(buf, sizeof(ASDUDZ2));
				if (nResult != SEND_OK)
				{
					return false;
				}
				//�������
				memset(pAsdudz->m_data, 0, pAsdudz->MAX_DATA_PER_ASDUDZ2_RD);
				nSetIndex = 0;
			}

			nSetIndex++;
		}

		if (nSetIndex != pAsdudz->MAX_DATA_PER_ASDUDZ2_RD)
		{
			pAsdudz->SetItemCount(nSetIndex);

			int nResult = Send_V(buf, pAsdudz->GetAsduDzLength());

			if (nResult != SEND_OK)
			{
				return false;
			}
		}
	}
	return true;
}

bool C101Sender::OnSendDevWriteConform()
{
	//����ȷ����Ϣ
	char bufg[255];

	//��֯ASDU100
	ASDU203_GH* pAsduGh = (ASDU203_GH*)(bufg + sizeof(AVDU_HEAD));


	pAsduGh->type = D_FIX_WRITE;

	pAsduGh->vsq = 0x01;


	pAsduGh->asduAddr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());
	//����ԭ��
	pAsduGh->cot.SetCot(6);

	//��ֵ����
	pAsduGh->m_infoFixCode.SetAddr(m_p101Impl->GetFCIModule()->GetFixCode());
	//TODO

	pAsduGh->m_featureCode.CONT = 0;
	pAsduGh->m_featureCode.RES = 0;
	pAsduGh->m_featureCode.CR = 0;
	pAsduGh->m_featureCode.SE = 0;

	int nResult = 0;
	nResult = Send_V(bufg, sizeof(ASDU203_GH));

	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}

bool C101Sender::OnSendZoomDataRequest(ZOOM_BASE *pZoomData)
{
	char buf[255] = { 0 };

	//��֯
	ASDU112_ZOOM* pAsduZoom = (ASDU112_ZOOM*)(buf + sizeof(AVDU_HEAD));

	pAsduZoom->type = 0x70;
	pAsduZoom->vsq = 0x01;

	pAsduZoom->asduAddr.SetAddr(pZoomData->nBaseAddr);
	//����ԭ��
	pAsduZoom->cot.SetCot(pZoomData->nReason);
	//���ø���
	pAsduZoom->SetItemCount(pZoomData->nNumber);
	//����ֵ
	pAsduZoom->m_value = pZoomData->fValue;
	//
	pAsduZoom->uType = pZoomData->qpm;
	//
	pAsduZoom->infoaddr.SetAddr(pZoomData->nAddr);

	int nResult = Send_V(buf, sizeof(ASDU112_ZOOM));
	if (nResult != SEND_OK)
	{
		return false;
	}

	return true;
}

bool C101Sender::OnSendRecordCatalog(LB_DATA dLbData)
{
	//���һ��
	m_lstLbData.removeFirst();
	//����¼������
	m_lbData = dLbData;
	//�ٻ�Ŀ¼
	char buf[255];

	//��֯ASDUZGML
	ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(AVDU_HEAD));

	pAsduzgml->type = F_SC_NA_1;
	pAsduzgml->vsq = 0x01;

	pAsduzgml->cot.SetCot(COT_REQ);

	int nDeviceAddr = 1;
	pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

	pAsduzgml->m_infoaddr.SetAddr(dLbData.m_nCatalogIndex);

	pAsduzgml->m_infoFileIndex.SetAddr(0);
	pAsduzgml->m_reserve3 = 0x0;
	pAsduzgml->m_reserve4 = 0x2;

	int nResult = Send_V(buf, sizeof(ASDUZGML));

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();


	if (nResult != SEND_OK)
	{
		m_lbData.m_bValidFlag = false;
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Request Catalog failed").toLocal8Bit().data(), 1);
		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Request Catalog Success").toLocal8Bit().data(), 1);
	return true;
}

bool C101Sender::OnSendFixAreaCode(ASDU200 *pRequestArea)
{
	char buf[255] = { 0 };

	//��֯
	ASDU200* pAsduFix = (ASDU200*)(buf + sizeof(AVDU_HEAD));

	pAsduFix->type = pRequestArea->type;
	pAsduFix->vsq = pRequestArea->vsq;
	pAsduFix->cot.SetCot(pRequestArea->cot.GetCot());
	pAsduFix->asduAddr.SetAddr(pRequestArea->asduAddr.GetAddr());
	pAsduFix->m_infoaddr.SetAddr(pRequestArea->m_infoaddr.GetAddr());
	pAsduFix->m_infAreaIndex.SetAddr(pRequestArea->m_infAreaIndex.GetAddr());


	int nResult = Send_V(buf, sizeof(ASDU200));
	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Switch Fix Area Code Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Switch Fix Area Code Success").toLocal8Bit().data(), 1);

	return true;
}

LB_DATA C101Sender::GetLbData() const
{
	return m_lbData;
}

bool C101Sender::OnSendChooseLbFile()
{
	//�ٻ�Ŀ¼
	char buf[255];

	//��֯ASDUZGML
	ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(AVDU_HEAD));

	pAsduzgml->type = F_SC_NA_1;
	pAsduzgml->vsq = 0x01;

	pAsduzgml->cot.SetCot(COT_FILE);

	int nDeviceAddr = 1;
	pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

	pAsduzgml->m_infoaddr.SetAddr(m_lbData.m_nCatalogIndex);

	pAsduzgml->m_infoFileIndex.SetAddr(m_lbData.m_nFilenameIndex);

	pAsduzgml->m_reserve3 = 0x0;
	//01��ʾѡ���ļ�
	pAsduzgml->m_reserve4 = 0x1;

	int nResult = Send_V(buf, sizeof(ASDUZGML));

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();


	if (nResult != SEND_OK)
	{
		m_lbData.m_bValidFlag = false;
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Choose File failed").toLocal8Bit().data(), 1);
		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Choose File Success").toLocal8Bit().data(), 1);
	
	return true;
}

bool C101Sender::OnSendRequestFile(int nIndex)
{
	//�ٻ�Ŀ¼
	char buf[255];

	//��֯ASDUZGML
	ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(AVDU_HEAD));

	pAsduzgml->type = F_SC_NA_1;
	pAsduzgml->vsq = 0x01;

	pAsduzgml->cot.SetCot(COT_FILE);

	int nDeviceAddr = 1;
	pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

	pAsduzgml->m_infoaddr.SetAddr(m_lbData.m_nCatalogIndex);

	pAsduzgml->m_infoFileIndex.SetAddr(m_lbData.m_nFilenameIndex);

	pAsduzgml->m_reserve3 = 0x0;
	//01��ʾѡ���ļ�
	pAsduzgml->m_reserve4 = nIndex;

	int nResult = Send_V(buf, sizeof(ASDUZGML));

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();

	if (nResult != SEND_OK)
	{
		m_lbData.m_bValidFlag = false;
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Requese File Failed").toLocal8Bit().data(), 1);
		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Requese File Success").toLocal8Bit().data(), 1);
	return true;
}

bool C101Sender::OnSendFixSearch()
{
	char buf[255] = { 0 };

	//��֯
	ASDU201_C* pAsduFix = (ASDU201_C*)(buf + sizeof(AVDU_HEAD));

	pAsduFix->type = D_FIX_SEARCH;
	pAsduFix->vsq = 0x01;

	pAsduFix->cot.SetCot(COT_ACT);

	pAsduFix->asduAddr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());

	pAsduFix->m_infoaddr.SetAddr(0);


	int nResult = Send_V(buf, sizeof(ASDU201_C));

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();
	if (nResult != SEND_OK)
	{
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Search Area Code Failed").toLocal8Bit().data(), 1);

		return false;
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Search Fix Area Code Success").toLocal8Bit().data(), 1);

	return true;
}

void C101Sender::SetLbAllInfo(const QList<LB_DATA> &lstLbData)
{
	m_lstLbData = lstLbData;
}

void C101Sender::OnSendFileConformNode()
{
	//�ٻ�Ŀ¼
	char buf[255];

	//��֯ASDUZGML
	ASDUZGML* pAsduzgml = (ASDUZGML*)(buf + sizeof(AVDU_HEAD));

	pAsduzgml->type = F_AF_NA_1;
	pAsduzgml->vsq = 0x01;

	pAsduzgml->cot.SetCot(COT_FILE);

	int nDeviceAddr = 1;
	pAsduzgml->asduAddr.SetAddr(nDeviceAddr);

	pAsduzgml->m_infoaddr.SetAddr(m_lbData.m_nCatalogIndex);

	pAsduzgml->m_infoFileIndex.SetAddr(m_lbData.m_nFilenameIndex);

	pAsduzgml->m_reserve3 = 0x0;
	//01��ʾѡ���ļ�
	pAsduzgml->m_reserve4 = 1;

	int nResult = Send_V(buf, sizeof(ASDUZGML));

	QString strDeviceName = m_p101Impl->GetFCIModule()->GetDeviceName();

	if (nResult != SEND_OK)
	{
		m_lbData.m_bValidFlag = false;
		m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Last Conform File Failed").toLocal8Bit().data(), 1);
	}

	m_p101Impl->GetFCIModule()->GetMainModule()->LogString(strDeviceName.toLocal8Bit().data(), tr("Send Last Conform File Success").toLocal8Bit().data(), 1);

}

QList<LB_DATA> C101Sender::GetLbAllData()
{
	return m_lstLbData;
}

