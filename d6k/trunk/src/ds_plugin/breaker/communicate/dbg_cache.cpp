#include "dbg_cache.h"
#include "breaker_data.h"

char* CDbgCache::GetDataBuff()
{
	return m_buf;
}

/*********************************************************************************************************
** \brief GetDataLen
** \details 第二个字符代表的是长度,是从第三个字符往后的长度，再加上前两个
字符的长度  代表了一个包的总长度
** \param argl
** \param arg2
** \return int  一条包的总长度
** \author ww
** \date 2016年5月27日
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
** \details 获取还需要的字符数
** \param argl
** \param arg2
** \return int  还需要的字符数量
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
int CDbgCache::GetNeedByteLength()
{
	//获取一包104的长度
	int nDataLen = GetDataLen();
	if (nDataLen <= 0)
	{
		return 0;
	}

	//最大长度不能超过
	if (nDataLen > MAX_DBG_LEN)
	{
		nDataLen = MAX_DBG_LEN;
	}

	//总长度减去已经加载的长度
	return nDataLen - m_nCurPos;
}

/*********************************************************************************************************
** \brief CopyData(char* pBuff,int nLen)
** \details 复制指定长度的字符串到缓存中
** \param argl 字符串
** \param arg2 长度
** \return void
** \author ww
** \date 2016年5月27日
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

