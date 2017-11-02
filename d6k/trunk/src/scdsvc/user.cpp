/*! @file user.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  user.cpp
文件实现功能 :  用户与权限管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <DLL发送消息给SVC，由SVC进行控制>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   用户与权限管理
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
** \param nAppOccNo 登录的APP
** \param szUserName 
** \param szPassword 
** \param dur 
** \return bool 
** \author LiJin 
** \date 2017年10月29日 
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
				// 告警  某用户登录某节点


			
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
** \param nAppOccNo 登出的APP
** \return void 
** \author LiJin 
** \date 2017年10月29日 
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
	// 告警 ，某某用户退出登录。

}


/** @}*/
