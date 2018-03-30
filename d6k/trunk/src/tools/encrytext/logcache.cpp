#include "logcache.h"

CLogCache * CLogCache::m_pLogCache = nullptr;

CLogCache::CLogCache()
{
}


CLogCache::~CLogCache()
{
}

void CLogCache::InsertLog(const QString & strLog)
{
	m_lstLogInfo.append(strLog);
}

CLogCache * CLogCache::InstanceCache()
{
	if (m_pLogCache == nullptr)
	{
		m_pLogCache = new CLogCache;
	}

	return m_pLogCache;
}

void CLogCache::ClearLog()
{
	m_lstLogInfo.clear();
}
