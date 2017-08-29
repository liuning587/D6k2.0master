#include "breakrecver.h"
#include "socketconnect.h"
#include "breaker_data.h"
#include "breaker_module.h"

CBreakRecver::CBreakRecver(CSocketConnect *pSocket, QObject *parent)
	: QObject(parent)
{
	Q_ASSERT(pSocket);
	if (pSocket == nullptr)
	{
		return;
	}
	qRegisterMetaType<DBG_GET_SYS_INFO>("DBG_GET_SYS_INFO&");
	qRegisterMetaType<DBG_GET_MEAS>("DBG_GET_MEAS&");
	qRegisterMetaType<DEG_SOE_DETAIL>("DEG_SOE_DETAIL");
}

CBreakRecver::~CBreakRecver()
{
}

bool CBreakRecver::OnReceive(char *pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	qDebug() << QByteArray(pBuff,nLen).toHex();

	if (!m_dbgCache.IsEmpty())
	{
		if (m_dbgCache.GetCurByteLength() == 2)//������������
		{
			m_dbgCache.CopyData(pBuff, 2);//���Ƴ�����
			pBuff = pBuff + 2;
			nLen = nLen - 2;;
		}

		int nNeedBytes = m_dbgCache.GetNeedByteLength();
		if (nNeedBytes <= 0)
		{
			return false;
		}

		if (nNeedBytes > nLen)//��Ҫ�ĳ��ȴ��ڽ��յ��ĳ���
		{
			m_dbgCache.CopyData(pBuff, nLen);
			return true;
		}
		else//���յ��ĳ��ȴ�����Ҫ�ĳ���
		{
			//������Ҫ�Ĳ���
			m_dbgCache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;

			bool ISOK = AnalyseDbg(m_dbgCache.GetDataBuff(), m_dbgCache.GetDataLen());
			m_dbgCache.Clear();
			if (!ISOK)
			{
				return false;
			}

			//����ʣ��Ĳ���
			return OnReceive(pBuff, nLen);
		}
	}
	else//������Ϊ��
	{
		//�ڽ�����Ѱ�������ַ�
		char cStartChar = DBG_HEADER0;
		char cStartChar2 = DBG_HEADER1;
		char* pNewBuff = std::find(pBuff, pBuff + nLen, cStartChar);
		char *pNewBuff2 = std::find(pBuff, pBuff + nLen, cStartChar2);

		if (pNewBuff == pBuff + nLen)
			return false;//û���ҵ������ַ�

		if (pNewBuff2 == pBuff + nLen)
		{
			return false;
		}

		if (pNewBuff2 != pNewBuff +1)
		{
			return false;
		}

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//���������ַ���������  ���������ַ�
		m_dbgCache.CopyData(pBuff, 2);

		pBuff += 2;
		nLen -= 2;

		return OnReceive(pBuff, nLen);
	}

	return true;
}

bool CBreakRecver::AnalyseDbg(char *pBuff, int nLen)
{
	DBG_HEADER *pDbg = (DBG_HEADER*)(pBuff);

	switch (pDbg->type)
	{
	case DBG_CODE_GET_SYS_INFO:
	{
		//��ȡϵͳ��Ϣ
		OnRecvSysInfo(pBuff,nLen);
		break;
	}
	case DBG_CODE_GET_MEAS:
	{
		//��ȡ����ֵ��Ϣ
		OnRecvAnalogInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_DEBUG_DATA:
	{
		//��ȡ������Ϣ
		OnRecvDebugInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SETTING:
	{
		//��ȡ������ֵ
		OnRecvProtectDevInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SYSCFG:
	{
		//��ȡϵͳ����
		OnRecvSystemDevInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_DICFG:
	{
		//������
		OnRecvDiInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_DOCFG:
	{
		//������
		OnRecvDOInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SOFTSTRAP:
	{
		//��ѹ�嶨ֵ
		OnRecvSoftDevInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_GET_SOE:
	{
		//soe����
		OnRecvSoeInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_TIME:
	{
		//ʱ������
		OnRecvTimeInfo(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_SETTING:
	{
		//������ֵ
		OnRecvProtectAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_SYSCFG:
	{
		//������ֵ
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_DICFG:
	{
		//���ÿ������
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_DOCFG:
	{
		//���ÿ�������
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_RM_CTRL:
	{
		//���ÿ�������
		OnRecvRemoteContrlAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SET_SOFTSTRAP:
	{
		//������ѹ�嶨ֵ
		OnRcvSysAck(pBuff, nLen);
		break;
	}
	case DBG_CODE_SIGNALRESET:
	{
		//�źŸ���
		OnRecvReset(pBuff, nLen);
		break;
	}
	case DBG_CODE_ClearSOE:
	{
		//���soe
		OnRecvClearSoeAck(pBuff, nLen);
		break;
	}
	default:
		break;
		
	}

	return true;
}

void CBreakRecver::OnRecvSysInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_SYS_INFO *pSysInfo = (DBG_GET_SYS_INFO*)(pBuff);
	emit Signal_SysInfo(*pSysInfo);
}

//����ֵ��Ϣ
void CBreakRecver::OnRecvAnalogInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_Analog(*pMeas);
}

//������Ϣ
void CBreakRecver::OnRecvDebugInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_Debug(*pMeas);

}

void CBreakRecver::OnRecvProtectDevInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_ProtectDev(*pMeas);

}

void CBreakRecver::OnRecvSystemDevInfo(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_SystemDev(*pMeas);
}

void CBreakRecver::OnRecvDiInfo(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DBG_GET_MEAS *pMeas = (DBG_GET_MEAS *)(pBuff);
	emit Signal_DiInfo(*pMeas);

}

void CBreakRecver::OnRecvDOInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);
	DEG_GET_MSG2 *pMeas = (DEG_GET_MSG2 *)(pBuff);
	emit Signal_DOInfo(*pMeas);

}

//��ѹ�嶨ֵ
void CBreakRecver::OnRecvSoftDevInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DEG_SOFT_INFO *pMeas = (DEG_SOFT_INFO *)(pBuff);
	emit Signal_SoftDev(*pMeas);
}

//soe����
void CBreakRecver::OnRecvSoeInfo(char * pBuff, int nLen)
{
	Q_UNUSED(nLen);

	DEG_SOE_DETAIL *pSoe = (DEG_SOE_DETAIL *)(pBuff);

	emit Signal_SoeDetailInfo(*pSoe);
}

//ʱ�����÷��ر���
void CBreakRecver::OnRecvTimeInfo(char * pBuff, int nLen)
{
	//д��־
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//�ɹ�
		GetBreakerModuleApi()->WriteRunLog("Breaker","ʱ�����óɹ�",1);
	}
	else
	{
		//ʧ��
		QString strValue = QStringLiteral("ʱ������ʧ��,������:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(),1 );
	}
	
}

void CBreakRecver::OnRecvProtectAck(char * pBuff, int nLen)
{
	//д��־
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//�ɹ�
		GetBreakerModuleApi()->WriteRunLog("Breaker", "��ֵ���óɹ�", 1);
	}
	else
	{
		//ʧ��
		QString strValue = QStringLiteral("��ֵ����ʧ��,������:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

void CBreakRecver::OnRcvSysAck(char *pBuff, int nLen)
{
	//д��־
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//�ɹ�
		GetBreakerModuleApi()->WriteRunLog("Breaker", "��ֵ���óɹ�", 1);
	}
	else
	{
		//ʧ��
		QString strValue = QStringLiteral("��ֵ����ʧ��,������:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

void CBreakRecver::OnRecvRemoteContrlAck(char * pBuff, int nLen)
{
	//
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//�ɹ�
		GetBreakerModuleApi()->WriteRunLog("Breaker", "�����ɹ�", 1);
		emit Signal_RemoteControlAck();;
	}
	else
	{
		//ʧ��
		QString strValue = QStringLiteral("����ʧ��,������:%1").arg(pTimeAck->m_Result);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

//�źŸ���
void CBreakRecver::OnRecvReset(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);
	if (pTimeAck->m_Result == 0)
	{
		//�ɹ�
		GetBreakerModuleApi()->WriteRunLog("Breaker", "�źŸ���ɹ�", 1);
	}
	else
	{
		//ʧ��
		QString strValue = QStringLiteral("�źŸ���ʧ��,������:%1").arg(pTimeAck->m_AddResult);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}

void CBreakRecver::OnRecvClearSoeAck(char *pBuff, int nLen)
{
	Q_UNUSED(nLen);
	TIME_SET_ACK_INFO *pTimeAck = (TIME_SET_ACK_INFO*)(pBuff);

	qDebug() << QByteArray(pBuff,nLen).toHex();
	if (pTimeAck->m_Result == 0)
	{
		//�ɹ�
		GetBreakerModuleApi()->WriteRunLog("Breaker", "�����ɹ�", 1);
		emit Signal_SeoClearSuccess();
	}
	else
	{
		//ʧ��
		QString strValue = QStringLiteral("����ʧ��,������:%1").arg(pTimeAck->m_AddResult);
		GetBreakerModuleApi()->WriteRunLog("Breaker", strValue.toLocal8Bit().data(), 1);
	}
}
