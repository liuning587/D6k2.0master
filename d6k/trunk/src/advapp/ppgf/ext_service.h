/*! @file ext_service.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  ext_service.h
�ļ�ʵ�ֹ��� :  ��չ�������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��չ�������
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef EXT_SVC_H
#define EXT_SVC_H

#include "datatypes.h"
#include "singleton.h"
#include <vector>
#include <memory>
 
class CExtService
{
public:
	CExtService(void);
	explicit CExtService(int32u nOccNo);

	~CExtService(void);
public:
	virtual bool Initalize(const char * pszProjectName);

	virtual void Run();

	virtual void Shutdown();

	void LogMsg(const char* logMsg, int nLevel);

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}
protected:
	//! APP�����к�
	int32u m_nOccNo;
	//! APP�ı�ǩ
	std::string m_szTagName;

private:
	 
};

/*! \struct  CExtSvcContainer
*   \brief   ��չ�����������һ�������ڿ��ܴ��ڶ������ */
class CExtSvcContainer
{
public:
	static CExtSvcContainer *GetInstance()
	{
		return Singleton<CExtSvcContainer, DefaultSingletonTraits<CExtSvcContainer>>::instance();
	}
public:
	bool RegisterService(CExtService *pSvc);

	bool UnRegisterService(int32u nOccNo);

	CExtService * FindService(int32u nOccNo);
	//! ���� 
	bool Start(int32u nOccNo);
	//! ֹͣ 
	bool Stop(int32u nOccNo);
private:
	CExtSvcContainer();
	~CExtSvcContainer();
private:
	friend DefaultSingletonTraits<CExtSvcContainer>;
	std::vector<CExtService*> m_arrExtSvcs;
};

#endif // EXT_SVC_H



/** @}*/

