/*! @file write_log_task.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  write_log_task.cpp
文件实现功能 :  记录到文件中
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author  LiJin
*  @version 1.0
*  @date    2016.12.28
*******************************************************************************/

#include "msglog.h"
#include "ui_log.h"
#include "stl_util-inl.h"
 
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile> 


CUiLog::CUiLog( )
{
}

CUiLog::~CUiLog()
{
	Clear();
}

void CUiLog::Clear()
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.clear();
}

void  CUiLog::AddLogMsg(std::shared_ptr<CMsgLog> pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;

	std::lock_guard<std::mutex> lock(m_LogLock);

	size_t nCount = m_arrLogs.size();

	while (nCount >= MAX_UI_LOG_COUNT)
	{
		m_arrLogs.pop_back();
		nCount = m_arrLogs.size();
	}
	
	m_arrLogs.push_back(pLog);
}

void  CUiLog::AddLogMsg(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;

	std::shared_ptr<CMsgLog> pMsgLog = std::make_shared<CMsgLog>(pMsg);

	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.push_back(pMsgLog);
}

size_t CUiLog::GetLogCount() const
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	return	m_arrLogs.size();
}

bool CUiLog::GetLogString(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	std::shared_ptr<CMsgLog> pTempLog = nullptr;

	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pTempLog = m_arrLogs.front();
	Q_ASSERT(pTempLog);
	if (pTempLog)
	{
		*pMsg = *pTempLog;
		m_arrLogs.pop_front(); 
	}
	return true;
}

bool  CUiLog::TakeMsgLog(std::shared_ptr<CMsgLog>&pLog)
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pLog = m_arrLogs.front();
	m_arrLogs.pop_front();
	return true;
}
/** @}*/
