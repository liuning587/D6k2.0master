/*! @file red_task.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  red_task.cpp
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
#include "red_task.h"
#include "db_svc.h"
#include "fes.h"
#include "log/log.h"

#include <QObject> 
#include <QString> 

#include <vector>
#include <sstream>

CRedTask::CRedTask(std::shared_ptr<CDbSvc>& pDBSvc):m_pDbSvc(pDBSvc)
{
	Q_ASSERT(pDBSvc);
}

CRedTask::~CRedTask()
{

}

void CRedTask::Start()
{
	CBaseTask::Start();
}

void CRedTask::Stop()
{
	CBaseTask::Stop();
}

void CRedTask::Run()
{

}

/** @}*/
