/*! @file user.h
<PRE>
********************************************************************************
模块名       :
文件名       :  user.h
文件实现功能 :  用户与权限管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
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
#ifndef SCADA_USER_H
#define SCADA_USER_H

#include "datatypes.h"

#include <string>
#include <vector>
#include <chrono>


class CScadaUserGroup;

/*! \struct  CScadaUser
*   \brief 用户 */
class CScadaUser
{
public:
	CScadaUser();
	~CScadaUser();
public:

	std::string GetUserName() const
	{
		return m_szUserName;
	}

	std::string GetPassword() const
	{
		return m_szPassword;
	}
	int GetUserPrivilege()
	{
		return 0;
	}
private:
	std::string m_szUserName;

	std::string m_szPassword; //! 口令


	std::vector<CScadaUserGroup*> m_arrGroups; //! 属于哪个组，一个用户可以属于多个组中 
};


class CScadaUserGroup
{
public:
	CScadaUserGroup();
	~CScadaUserGroup();

private:
	std::vector<CScadaUser*> m_arrUsers;
};


class CScadaUserNode
{
public:
	CScadaUserNode();
	~CScadaUserNode();
public:
	void SetCurentLoginUser(CScadaUser *pUser)
	{
		m_pCurLoginUser = pUser;
		if (pUser)
		{
			m_tpLoginStart = std::chrono::system_clock::now();

		}
	}
	CScadaUser *GetCurentLoginUser()
	{
		return m_pCurLoginUser;
	}

private:

	CScadaUser *m_pCurLoginUser; //! 当前登录的用户

	std::chrono::time_point< std::chrono::system_clock> m_tpLoginStart; //! 开始登录的时间
};


class CUserMgr
{
public:
	CUserMgr();
	~CUserMgr();
public:
	// 加载配置
	void LoadXml();        

	// 用户登录
	bool UserLogin(int32u nAppOccNo, const std::string & szUserName, const std::string & szPassword, const std::chrono::seconds &dur);
	// 用户登出
	void UserLogout( int32u nAppOccNo );

public:

	
private:
	std::vector<CScadaUser *>    m_arrUsers;      //! 有哪些用户
	std::vector<CScadaUserGroup*> m_arrGroups;    //! 用户组
	std::vector<CScadaUserNode*>  m_arrNodes;     //! 节点

	CScadaUserNode *m_pCurNode;                   //! 当前节点

	const std::chrono::hours MAX_ONLINE_TIME = std::chrono::hours(8);



};


 
#endif // SCADA_USER_H


/** @}*/

