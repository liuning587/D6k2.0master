/*! @file message.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  message.h
�ļ�ʵ�ֹ��� :  ��Ϣ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��Ϣ����
*  @author  LiJin
*  @version 1.0
*  @date    2017.09.13
*******************************************************************************/
#ifndef MESSAGE_H
#define MESSAGE_H

#include <functional>

struct DMSG;

typedef std::function<bool(DMSG *)> MsgFunctor;

class CDbSvc;


/*! \struct  CMessageExecutor
*   \brief ��Ϣִ���� */
class CMessageExecutor
{
public:
	CMessageExecutor();
	virtual ~CMessageExecutor() = default;

	explicit CMessageExecutor(DMSG *pMsg);
public:
	bool Run(const MsgFunctor  & handler);
	
	// 
	bool Run(CDbSvc *pDbSvc,DMSG *pDMsg);

private:
	DMSG * m_pDMsg;

};



 
#endif // MESSAGE_H


/** @}*/

