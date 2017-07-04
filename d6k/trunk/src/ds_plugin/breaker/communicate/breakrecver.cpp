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
		if (m_dbgCache.GetCurByteLength() == 2)//有两个启动符
		{
			m_dbgCache.CopyData(pBuff, 2);//复制长度域
			pBuff = pBuff + 2;
			nLen = nLen - 2;;
		}

		int nNeedBytes = m_dbgCache.GetNeedByteLength();
		if (nNeedBytes <= 0)
		{
			return false;
		}

		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_dbgCache.CopyData(pBuff, nLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_dbgCache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;

			bool ISOK = AnalyseDbg(m_dbgCache.GetDataBuff(), m_dbgCache.GetDataLen());
			m_dbgCache.Clear();
			if (!ISOK)
			{
				return false;
			}

			//解析剩余的部分
			return OnReceive(pBuff, nLen);
		}
	}
	else//缓冲区为空
	{
		//在接收区寻找启动字符
		char cStartChar = DBG_HEADER0;
		char cStartChar2 = DBG_HEADER1;
		char* pNewBuff = std::find(pBuff, pBuff + nLen, cStartChar);
		char *pNewBuff2 = std::find(pBuff, pBuff + nLen, cStartChar2);

		if (pNewBuff == pBuff + nLen)
			return false;//没有找到启动字符

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

		//复制启动字符至缓冲区  两个启动字符
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
		//获取系统信息
		OnRecvSysInfo(pBuff,nLen);
		break;
	}
	case DBG_CODE_GET_MEAS:
	{
		//获取测量值信息
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
