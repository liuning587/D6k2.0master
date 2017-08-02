
#include "ext_service.h"

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

#include "ext_service.h" 

#include "power_define.h"
#include "scadaapi/scdsvcapi.h"
#include "scadaapi/scdapp_api.h"
#include "scadaapi/scdapp_def.h"
#include "log/log.h"
#include "stl_util-inl.h"

#include <QDebug>

CExtService::CExtService()
{

}

CExtService::~CExtService()
{

}

CExtService::CExtService(int32u nOccNo) :m_nOccNo(nOccNo)
{

}



bool CExtService::Initalize(const char * pszProjectName)
{
	Q_ASSERT(pszProjectName && strlen(pszProjectName) > 0);
	if (!pszProjectName || strlen(pszProjectName) == 0)
	{
		return false;
	}

	::ConnectScada(pszProjectName, APP_NAME, 1);

	return true;

}

void CExtService::LogMsg(const char* logMsg, int nLevel)
{
	::LogMsg("PPGF", logMsg, nLevel, nullptr);
}


void CExtService::Shutdown()
{
	::DisconnectScada("", APP_NAME);
}

CExtSvcContainer::CExtSvcContainer()
{

}

CExtSvcContainer::~CExtSvcContainer()
{
	STLDeleteElements(&m_arrExtSvcs);
}


bool CExtSvcContainer::RegisterService(CExtService *pSvc)
{
	Q_ASSERT(pSvc);
	if (pSvc == nullptr)
		return false;

	Q_ASSERT(pSvc->GetOccNo() == INVALID_OCCNO || pSvc->GetOccNo() <= MAX_OCCNO);
	if (pSvc->GetOccNo() == INVALID_OCCNO || pSvc->GetOccNo() > MAX_OCCNO)
		return false;

	// �Ȳ��ҿ�����û���ظ�
	for (auto it : m_arrExtSvcs)
	{
		if (it->GetOccNo() == pSvc->GetOccNo() || it == pSvc)
		{
			Q_ASSERT(false);
			return false;
		}
	}

	m_arrExtSvcs.push_back(pSvc);

	return true;
}

bool CExtSvcContainer::UnRegisterService(int32u nOccNo)
{
	for (auto it : m_arrExtSvcs)
	{
		if (it->GetOccNo() == nOccNo)
		{

			return true;
		}
	}

	return false;
}

CExtService * CExtSvcContainer::FindService(int32u nOccNo)
{
	Q_ASSERT(nOccNo == INVALID_OCCNO || nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	for (auto it : m_arrExtSvcs)
	{
		if (it->GetOccNo() == nOccNo)
		{
			return it;
		}
	}
	return nullptr;
}

/** @}*/

