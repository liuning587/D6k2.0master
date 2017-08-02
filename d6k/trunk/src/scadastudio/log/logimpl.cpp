#include "logimpl.h"

#include "public.h"

#include "stl_util-inl.h"

#include "write_log_task.h"
//#include "send_log_task.h"

#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile> 
 

CLogImpl * CLogImpl::GetInstance()
{
	return Singleton<CLogImpl, DefaultSingletonTraits<CLogImpl>>::instance();
}

CLogImpl::CLogImpl()
{
	m_bTaskIsStarted = false;
	m_nMaxIndex = 0;
	m_pWriteFileTask = std::make_shared<CWriteLogTask>(m_LogConfigs);
//	m_pSendLogTask = std::make_shared<CSendLogTask>();
}

CLogImpl::~CLogImpl()
{

} 
/*! \fn bool  CLogImpl::CreateLog(const char* szLogName)
********************************************************************************************************* 
** \brief CLogImpl::CreateLog 
** \details  创建log服务器，
** \param szLogName 
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
bool  CLogImpl::CreateLog(const char* szLogName)
{
	Q_ASSERT(szLogName);
	if (szLogName == nullptr)
		return false;
	Q_ASSERT(strlen(szLogName) > 0);
	if (strlen(szLogName) == 0)
		return false;
	
	m_LogConfigs.LoadXml( );

	QString szLog = QString(QObject::tr("Start [%1] log ......")).arg(szLogName);

	LogMsg("LOG", szLog.toStdString().c_str(), 0);

	QThread::sleep(2);
	
	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->InitFilePath();
		m_pWriteFileTask->StartLogTask(szLogName);
	}

	return true;
}

bool CLogImpl::DestroyLog(const char* szLogName)
{
	Q_UNUSED(szLogName);

	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->StopModule();
	}
	m_pWriteFileTask.reset();

	return true;
}

void  CLogImpl::AddLogMsg(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;

	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->AddLogMsg(pMsg);
	}

	std::shared_ptr<CMsgLog> pLog = std::make_shared<CMsgLog>(pMsg);
	m_UiLog.AddLogMsg(pLog);

	delete pMsg;
	pMsg = nullptr;
}

bool CLogImpl::LogMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel, const QDateTime & tm)
{
	Q_ASSERT(szLogTxt);
	if (szLogTxt == nullptr)
		return false;

	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	Q_ASSERT(szLogTxt != nullptr);
	if (szLogTxt == nullptr)
		return false;
	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	std::string szModuleName;
	if (pszMoudleName == nullptr)
	{
		szModuleName = "Unknown";
	}
	else
	{
		if (strlen(pszMoudleName) == 0)
		{
			szModuleName = "Unknown";
		}
		else
		{
			szModuleName = pszMoudleName;
		}
	}
	m_nMaxIndex++;
	CMsgLog *pMsg = new CMsgLog(szModuleName, szLogTxt, nLevel, m_nMaxIndex,tm);

	AddLogMsg(pMsg);

	return true;
}

bool CLogImpl::LogMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel)
{
	QDateTime  tm = QDateTime::currentDateTime();
	return LogMsg(pszMoudleName, szLogTxt, nLevel, tm);
}

bool  CLogImpl::LogDebugMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, const QDateTime & tm)
{
	Q_ASSERT(szLogTxt);
	if (szLogTxt == nullptr)
		return false;

	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	Q_ASSERT(szLogTxt != nullptr);
	if (szLogTxt == nullptr)
		return false;
	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	std::string szModuleName;
	if (pszMoudleName == nullptr)
	{
		szModuleName = "Unknown";
	}
	else
	{
		if (strlen(pszMoudleName) == 0)
		{
			szModuleName = "Unknown";
		}
		else
		{
			szModuleName = pszMoudleName;
		}
	}
	m_nMaxIndex++;
	CMsgLog *pMsg = new CMsgLog(szModuleName, szLogTxt, nLevel, m_nMaxIndex, szFile, nLine, szFunc,tm);

	AddLogMsg(pMsg);

	return true;
}

bool  CLogImpl::LogDebugMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc)
{
	QDateTime  tm = QDateTime::currentDateTime();
	return LogDebugMsg(pszMoudleName, szLogTxt, nLevel, szFile, nLine, szFunc, tm);
}

/** @}*/
