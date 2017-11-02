/*! @file red_task.h
<PRE>
********************************************************************************
模块名       :
文件名       :  red_task.h
文件实现功能 :  主从同步任务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   主从同步任务
*  @author  LiJin
*  @version 1.0
*  @date    2017.10.08
*******************************************************************************/
#ifndef RED_TASK_MODULE_H
#define RED_TASK_MODULE_H
 
#include "datatypes.h"

#include "base_task.h"
 

class CDbSvc;
class CRedTask :public CBaseTask
{
public:
	CRedTask() = delete;
	~CRedTask() ;
	CRedTask(std::shared_ptr<CDbSvc>& pDBSvc);
public:
	void Start();
	void Stop();
protected:
	void Run();
	void PullAin();

private:
	std::shared_ptr<CDbSvc> & m_pDbSvc;

 
};
 
#endif // RED_TASK_MODULE_H


/** @}*/

