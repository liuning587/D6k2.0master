/*! @file ui_log.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  ui_log.cpp
�ļ�ʵ�ֹ��� :    
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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

	//  UI���ֵ���
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

