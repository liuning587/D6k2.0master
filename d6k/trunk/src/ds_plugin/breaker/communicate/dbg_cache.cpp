#include "dbg_cache.h"
#include "breaker_data.h"

char* CDbgCache::GetDataBuff()
{
	return m_buf;
}

/*********************************************************************************************************
** \brief GetDataLen
** \details �ڶ����ַ�������ǳ���,�Ǵӵ������ַ�����ĳ��ȣ��ټ���ǰ����
�ַ��ĳ���  ������һ�������ܳ���
** \param argl
** \param arg2
** \return int  һ�������ܳ���
** \author ww
** \date 2016��5��27��
** \note
********************************************************************************************************/
int CDbgCache::GetDataLen()
{
	DBG_MSG2 *pLengbth  = (DBG_MSG2*)(m_buf+2);
	return pLengbth->GetAddr() + 4;
}

int CDbgCache::GetCurByteLength()
{
	return m_nCurPos;
}

/*********************************************************************************************************
** \brief GetNeedByteLength
** \details ��ȡ����Ҫ���ַ���
** \param argl
** \param arg2
** \return int  ����Ҫ���ַ�����
** \author ww
** \date 2016��5��27��
** \note
********************************************************************************************************/
int CDbgCache::GetNeedByteLength()
{
	//��ȡһ��104�ĳ���
	int nDataLen = GetDataLen();
	if (nDataLen <= 0)
	{
		return 0;
	}

	//��󳤶Ȳ��ܳ���
	if (nDataLen > MAX_DBG_LEN)
	{
		nDataLen = MAX_DBG_LEN;
	}

	//�ܳ��ȼ�ȥ�Ѿ����صĳ���
	return nDataLen - m_nCurPos;
}

/*********************************************************************************************************
** \brief CopyData(char* pBuff,int nLen)
** \details ����ָ�����ȵ��ַ�����������
** \param argl �ַ���
** \param arg2 ����
** \return void
** \author ww
** \date 2016��5��27��
** \note
********************************************************************************************************/
void CDbgCache::CopyData(char* pBuff, int nLen)
{
	char* pFrom = m_buf + m_nCurPos;

	memcpy(pFrom, pBuff, nLen);

	m_nCurPos = m_nCurPos + nLen;
}

bool CDbgCache::IsEmpty()
{
	if (m_nCurPos == 0)
	{
		return true;
	}
	return false;
}

void CDbgCache::Clear()
{
	m_nCurPos = 0;
}

