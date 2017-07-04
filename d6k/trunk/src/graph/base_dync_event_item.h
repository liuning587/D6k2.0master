/*! @file base_dync_event_item.h
<PRE>
********************************************************************************
模块名       :
文件名       :  base_dync_event_item.h
文件实现功能 :  基本的命令事件定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的命令事件定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef BASE_DYNC_EVENT_ITEM_H
#define BASE_DYNC_EVENT_ITEM_H
 
#include <string>
#include <memory>
#include "dync_event.h"

class QXmlStreamWriter;
class QXmlStreamReader;

struct TAGITEM;

class CVariant;

class CBaseWidget;

/*! \class CBaseDyncEventItem base_dync_event.h  "base_dync_event.h"
*  \brief 动态操作事件
*  \details
*/
class CBaseDyncEventItem
{
public:
	CBaseDyncEventItem();
	virtual ~CBaseDyncEventItem();
	CBaseDyncEventItem(const CBaseDyncEventItem&);
	CBaseDyncEventItem& operator=(const CBaseDyncEventItem& src);
	bool operator==(const CBaseDyncEventItem& src) const;
public:
	enum EVENT_TYPE
	{
		DYNC_FILE_OP = 0, //!< 文件操作
		DYNC_VAR_OP,      //!< 变量操作
		DYNC_SYS_OP,      //!< 系统类操作
		DYNC_SCRIPT_OP,   //!< 脚本类操作
		DYNC_USER_OP,     //!< 用户操作
	};

public:
// 	virtual void SaveXml(JWXml::CXmlNodePtr  pDyncPtr)const;
// 	virtual bool LoadXml(JWXml::CXmlNodePtr  pDyncPtr);

	virtual void DoAction(CEventIntent &intent);
	virtual std::string GetEventDescript()const;

	int GetEventType();
	void SetEventType(EVENT_TYPE eType);

public:
	bool m_bEnable;
public:
	//! 命令
	EVENT_TYPE  m_OpEventType;
};


#endif // BASE_DYNC_EVENT_ITEM_H


/** @}*/
