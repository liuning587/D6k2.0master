/*! @file base_dync_event_item.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  base_dync_event_item.cpp
文件实现功能 :  基本的动态定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的动态定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/


#include "base_widget.h"
#include "base_dync_event_item.h"

#include "fesapi/fesdatadef.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif

#include "../scdpub/variant_data.h"



CBaseDyncEventItem::CBaseDyncEventItem() :m_bEnable(false)
, m_OpEventType(DYNC_FILE_OP)
{

}

CBaseDyncEventItem::~CBaseDyncEventItem()
{
	m_bEnable = true;
}

CBaseDyncEventItem::CBaseDyncEventItem(const CBaseDyncEventItem& src)
{
	*this = src;
}

CBaseDyncEventItem& CBaseDyncEventItem::operator=(const CBaseDyncEventItem& src)
{
	if (this != &src)
	{
		this->m_bEnable = src.m_bEnable;
// 		this->m_ActionType = src.m_ActionType;
		this->m_OpEventType = src.m_OpEventType;
	}
	return *this;
}

bool  CBaseDyncEventItem::operator==(const CBaseDyncEventItem& src)const
{
	if (src.m_bEnable == this->m_bEnable &&
// 		this->m_ActionType == src.m_ActionType &&
		this->m_OpEventType == src.m_OpEventType)
	{
		return true;
	}

	return false;
}

// void CBaseDyncEventItem::SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const
// {
// 	ASSERT(!pDyncPtr->IsNull());
// 	if (pDyncPtr->IsNull())
// 		return;
// 	JWXml::CXmlNodePtr pChild = pDyncPtr->NewChild(_T("EVENT_TYPE"));
// 	ASSERT(!pChild->IsNull());
// 	if (!pChild->IsNull())
// 	{
// 		pChild->SetValue(static_cast<DWORD>(m_OpEventType));
// 	}
// 	pChild = pDyncPtr->NewChild(_T("ACTION_TYPE"));
// 	ASSERT(!pChild->IsNull());
// 	if (!pChild->IsNull())
// 	{
// 		pChild->SetValue(static_cast<DWORD>(m_ActionType));
// 	}
// }

// bool CBaseDyncEventItem::LoadXml(JWXml::CXmlNodePtr  pDyncPtr)
// {
// 
// 	return true;
// }



/*! \fn void CBaseDyncEventItem::SetActionType(const CString & szTxt)
*********************************************************************************************************
** \brief CBaseDyncEventItem::SetActionType
** \details 设置动作类型
** \param szTxt
** \return void
** \author LiJin
** \date 2012年1月18日
** \note
********************************************************************************************************/




void  CBaseDyncEventItem::DoAction(CEventIntent &intent)
{

}

std::string  CBaseDyncEventItem::GetEventDescript() const
{
	return "";
}


int CBaseDyncEventItem::GetEventType()
{
	return m_OpEventType;
}

void CBaseDyncEventItem::SetEventType(EVENT_TYPE eType)
{
	m_OpEventType = eType;
}



/** @}*/
