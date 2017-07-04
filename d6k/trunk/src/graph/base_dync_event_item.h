/*! @file base_dync_event_item.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_dync_event_item.h
�ļ�ʵ�ֹ��� :  �����������¼�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����������¼�����
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
*  \brief ��̬�����¼�
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
		DYNC_FILE_OP = 0, //!< �ļ�����
		DYNC_VAR_OP,      //!< ��������
		DYNC_SYS_OP,      //!< ϵͳ�����
		DYNC_SCRIPT_OP,   //!< �ű������
		DYNC_USER_OP,     //!< �û�����
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
	//! ����
	EVENT_TYPE  m_OpEventType;
};


#endif // BASE_DYNC_EVENT_ITEM_H


/** @}*/
