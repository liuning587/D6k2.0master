/*! @file red_task.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  red_task.h
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

