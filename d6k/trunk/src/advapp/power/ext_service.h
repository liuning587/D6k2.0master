#pragma once

//  ��չ�������
#ifndef EXT_SERVICE_H
#define EXT_SERVICE_H

#include <vector>
#include <memory>

#include "datatypes.h"
#include "singleton.h"

class CExtService
{
public:
	CExtService(void);
	explicit CExtService(int32u nOccNo);

	~CExtService(void);
public:
	virtual bool Initalize(const char * pszProjectName);

	//virtual void Run();

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

#endif // EXT_SERVICE_H
