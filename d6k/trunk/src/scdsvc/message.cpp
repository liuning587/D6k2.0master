/*! @file message.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  message.cpp
文件实现功能 :  消息定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   消息定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.09.13
*******************************************************************************/
#include "message.h"
#include "log/log.h"
#include "mail/mail.h"

#include <QObject> 
#include <QString>  
 
 

CMessageExecutor::CMessageExecutor(DMSG *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;


}

bool CMessageExecutor::Run()
{

	return true;
}

/** @}*/
