/*! @file message.h
<PRE>
********************************************************************************
模块名       :
文件名       :  message.h
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
#ifndef MESSAGE_H
#define MESSAGE_H


struct DMSG;


/*! \struct  CMessageExecutor
*   \brief 消息执行器 */
class CMessageExecutor
{
public:
	CMessageExecutor() = default;
	virtual ~CMessageExecutor() = default;

	CMessageExecutor(DMSG *pMsg);
public:

	bool Run();

private:


};



 
#endif // MESSAGE_H


/** @}*/

