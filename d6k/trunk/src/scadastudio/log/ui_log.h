/*! @file ui_log.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  ui_log.cpp
文件实现功能 :    
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
#ifndef UI_LOG_TASK_H
#define UI_LOG_TASK_H
 

#include <deque>
#include <string>
#include <mutex>

#include <QDateTime>
 
class CMsgLog;
 

class CUiLog 
{
public:
	CUiLog( );
	~CUiLog();
public:  
	void AddLogMsg(CMsgLog *pLog);
	void AddLogMsg(std::shared_ptr<CMsgLog> pLog);

	//  UI部分调用
	size_t GetLogCount()const;
	bool GetLogString(CMsgLog *pMsg);


	bool TakeMsgLog(std::shared_ptr<CMsgLog>&pLog);
	
protected:
	void Clear();
	  
private:  

	mutable std::mutex m_LogLock;

	std::deque <std::shared_ptr<CMsgLog>> m_arrLogs; 

	enum 
	{
		MAX_UI_LOG_COUNT = 10000,
	};
};


#endif // WRITE_LOG_TASK_H


/** @}*/

