#include "breakrecver.h"
#include "socketconnect.h"
#include "breaker_data.h"

CBreakRecver::CBreakRecver(CSocketConnect *pSocket, QObject *parent)
	: QObject(parent)
{
	Q_ASSERT(pSocket);
	if (pSocket == nullptr)
	{
		return;
	}
	qRegisterMetaType<DBG_GET_SYS_INFO>("DBG_GET_SYS_INFO&");

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

void CBreakRecver::OnRecvAnalogInfo(char * pBuff, int nLen)
{

}
