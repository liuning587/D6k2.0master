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


struct DMSG;


/*! \struct  CMessageExecutor
*   \brief ��Ϣִ���� */
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

