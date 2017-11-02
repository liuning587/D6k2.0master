/*! @file user.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  user.cpp
�ļ�ʵ�ֹ��� :  �û���Ȩ�޹���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <DLL������Ϣ��SVC����SVC���п���>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �û���Ȩ�޹���
*  @author  LiJin
*  @version 1.0
*  @date    2017.10.29
*******************************************************************************/
#include "user.h"
#include "log/log.h"
#include "mail/mail.h"

#include <QObject> 
#include <QString>  
  


/*! \fn bool CUserMgr::UserLogin(int32u nAppOccNo, const std::string & szUserName, const std::string & szPassword, const std::chrono::seconds &dur)
********************************************************************************************************* 
** \brief CUserMgr::UserLogin 
** \details 
** \param nAppOccNo ��¼��APP
** \param szUserName 
** \param szPassword 
** \param dur 
** \return bool 
** \author LiJin 
** \date 2017��10��29�� 
** \note 
********************************************************************************************************/
bool CUserMgr::UserLogin(int32u nAppOccNo, const std::string & szUserName, const std::string & szPassword, const std::chrono::seconds &dur)
{
	if (szUserName.empty() == true)
		return false;

	if (dur > MAX_ONLINE_TIME)
	{// todo log
		return false;
	}

	auto it = m_arrUsers.begin();
	for ( ;it != m_arrUsers.end(); ++it)
	{
		if (  (*it)->GetUserName().compare(szUserName) == 0  )
		{
			if ( (*it)->GetPassword().compare(szPassword) == 0)
			{
				Q_ASSERT(m_pCurNode);
				if (m_pCurNode)
				{
					m_pCurNode->SetCurentLoginUser(*it);
				}
				// �澯  ĳ�û���¼ĳ�ڵ�


			
				return true;
			}			
		}
	}

	return false;
}
/*! \fn void CUserMgr::UserLogout(int32u nAppOccNo)
********************************************************************************************************* 
** \brief CUserMgr::UserLogout 
** \details 
** \param nAppOccNo �ǳ���APP
** \return void 
** \author LiJin 
** \date 2017��10��29�� 
** \note 
********************************************************************************************************/
void CUserMgr::UserLogout(int32u nAppOccNo)
{
	Q_ASSERT(m_pCurNode);
	if (m_pCurNode == nullptr)
		return;

	CScadaUser *pUser = m_pCurNode->GetCurentLoginUser();
	if (pUser == nullptr)
	{
		return;
	}
	m_pCurNode->SetCurentLoginUser(nullptr);
	// �澯 ��ĳĳ�û��˳���¼��

}


/** @}*/
