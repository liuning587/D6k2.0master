/*! @file red_task.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  red_task.cpp
�ļ�ʵ�ֹ��� :  ����ͬ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����ͬ������
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
