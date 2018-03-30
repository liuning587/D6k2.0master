#pragma once

#include<QStringList>

class CLogCache
{
public:
	~CLogCache();
	void InsertLog(const QString &strLog);

	static CLogCache* InstanceCache();

	QStringList &GetLogInfo()
	{
		return m_lstLogInfo;
	}

	void ClearLog();
private:
	CLogCache();

	static CLogCache *m_pLogCache;
	//logÊý¾Ý
	QStringList m_lstLogInfo;
};

