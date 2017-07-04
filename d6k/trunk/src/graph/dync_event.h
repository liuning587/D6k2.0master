/*! @file dync_event.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_event.h
�ļ�ʵ�ֹ��� :  �����¼�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����¼�����
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef DYNC_EVENT_H
#define DYNC_EVENT_H
 
#include <string>
#include <memory>
#include <vector>

#include "base_dync.h"

class QXmlStreamWriter;
class QXmlStreamReader;

struct TAGITEM;

class CVariant;

 
class CBaseWidget;
class CBaseDyncEventItem; 

class CEventIntent
{
public:
	CEventIntent();
	virtual ~CEventIntent();
public:
	//! ����ͼ��
	//std::function< bool(const std::string &)> m_fnOpenNewGraph;


};

/*! \class CDyncEventData dync_event.h  "dync_event.h"
*  \brief ��̬�����¼�
*  \details
*/
class CDyncEventData :public CBaseDyncData
{
public:
	CDyncEventData();

	const CDyncEventData& operator=(const CDyncEventData& src);
	bool operator==(const CDyncEventData& src) const;
	virtual ~CDyncEventData();

	// ��������
	enum ACTION_TYPE
	{
		ACTION_CLICK = 0, //!< ����
		ACTION_PRESSED,   //!< ����
		ACTION_RELEASE,   //!< �ͷ�
		ACTION_FOCUSED,   //!< ����
		ACTION_UNFOCUSED,
		MAX_ACTION_NUM
	};

public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
	// �Ƚ�������̬�����Ƿ���һ��������
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;
	// ����Ƿ�Ϸ�
	virtual bool CheckDyncData();
	// Ԥ����̬
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ���ƿ�¡  ����ռ䲢�����Լ�
	virtual CBaseDyncData *CloneDyncData();
	// ����̬�¼�
	virtual bool ProcessAction(CBaseWidget *pWidget, CEventIntent & intent);

	// ɾ���¼�
	void DeleteOpEvent(CBaseDyncEventItem *pEvent);

	void DeleteEvent(CBaseDyncEventItem *pEvent);
	// ����¼���ɾ���Ѿ���ȡ�������¼�
	void TrimData();

	//�����¼�
	CBaseDyncEventItem* CreateEventItem(int nEventType);
	//����ʱ��
	CBaseDyncEventItem* CreateEventItem(CBaseDyncEventItem* pBaseEventItem);

	// ���ö�������
	void SetActionType(ACTION_TYPE actionType);

	int GetActionType();

	const std::vector<CBaseDyncEventItem*> &getEvents()
	{
		return m_arrEvents;
	}


protected:
	//flag
	//bool m_bCheckFlag;
	//! ÿ��������ܴ��ڶ��ֶ�̬����
	std::vector<CBaseDyncEventItem*> m_arrEvents; 
	//! ��궯������
	ACTION_TYPE   m_ActionType;


};
#endif // BASE_DYNC_EVENT_H


/** @}*/
