
#ifndef _LOG_IMPL_H
#define _LOG_IMPL_H

#include "singleton.h"

#include "msglog.h"
#include "log_config.h"

#include <string>
#include <QDateTime>
#include <deque>

#include <memory>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  
#include <atomic>

  

class CWriteLogTask;
//class CSendLogTask;
#include "ui_log.h"

class CLogImpl
{
public:
	static CLogImpl *GetInstance();
public:
	bool CreateLog(const char* szLogName);
	bool DestroyLog(const char* szLogName);

	bool LogMsg(const char* szMoudleName,const char* szLogTxt,int nLevel);
	bool LogMsg(const char* szMoudleName, const char* szLogTxt, int nLevel,const QDateTime & tm);

	bool LogDebugMsg(const char* szMoudleName, const char* szLogTxt, int nLevel,const char * szFile,int nLine,const char *szFunc);
	bool LogDebugMsg(const char* szMoudleName, const char* szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, const QDateTime & tm);

	CUiLog & GetUiLog()
	{
		return m_UiLog;
	}
protected:
	void AddLogMsg(CMsgLog *pLog);
 
private:
	friend DefaultSingletonTraits<CLogImpl>;

	CLogImpl();
	~CLogImpl();
	
	std::size_t  m_nMaxIndex;
	
	QString  m_szLogPath;       //! LOG´æ´¢Â·¾¶

	CLogConfigMgr m_LogConfigs;
	
	CUiLog m_UiLog;

	std::atomic<bool> m_bTaskIsStarted;
	std::shared_ptr<CWriteLogTask> m_pWriteFileTask;

//	std::shared_ptr<CSendLogTask> m_pSendLogTask;
};
 

#endif // _LOG_IMPL_H

