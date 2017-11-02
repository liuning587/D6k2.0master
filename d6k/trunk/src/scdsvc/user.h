/*! @file user.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  user.h
�ļ�ʵ�ֹ��� :  �û���Ȩ�޹���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
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
#ifndef SCADA_USER_H
#define SCADA_USER_H

#include "datatypes.h"

#include <string>
#include <vector>
#include <chrono>


class CScadaUserGroup;

/*! \struct  CScadaUser
*   \brief �û� */
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

	std::string m_szPassword; //! ����


	std::vector<CScadaUserGroup*> m_arrGroups; //! �����ĸ��飬һ���û��������ڶ������ 
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

	CScadaUser *m_pCurLoginUser; //! ��ǰ��¼���û�

	std::chrono::time_point< std::chrono::system_clock> m_tpLoginStart; //! ��ʼ��¼��ʱ��
};


class CUserMgr
{
public:
	CUserMgr();
	~CUserMgr();
public:
	// ��������
	void LoadXml();        

	// �û���¼
	bool UserLogin(int32u nAppOccNo, const std::string & szUserName, const std::string & szPassword, const std::chrono::seconds &dur);
	// �û��ǳ�
	void UserLogout( int32u nAppOccNo );

public:

	
private:
	std::vector<CScadaUser *>    m_arrUsers;      //! ����Щ�û�
	std::vector<CScadaUserGroup*> m_arrGroups;    //! �û���
	std::vector<CScadaUserNode*>  m_arrNodes;     //! �ڵ�

	CScadaUserNode *m_pCurNode;                   //! ��ǰ�ڵ�

	const std::chrono::hours MAX_ONLINE_TIME = std::chrono::hours(8);



};


 
#endif // SCADA_USER_H


/** @}*/

