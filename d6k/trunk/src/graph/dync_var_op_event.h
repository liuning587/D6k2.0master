/*! @file base_dync.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_dync_event.h
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
#ifndef DYNC_VAR_OP_EVENT_H
#define DYNC_VAR_OP_EVENT_H
 
#include <string>
#include <memory>
#include <vector>
#include <QString>

#include "base_dync_event_item.h"

class QXmlStreamWriter;
class QXmlStreamReader;

/*! \class CDyncVarOpEventItem DynamicDef.h  "DynamicDef.h"
*  \brief ���������¼�
*  \details  ���������У�
*/
class  CDyncVarOpEventItem : public CBaseDyncEventItem
{
public:
	CDyncVarOpEventItem();
	virtual ~CDyncVarOpEventItem();
	CDyncVarOpEventItem& operator=(const CDyncVarOpEventItem& src);

	enum VAR_ACTION
	{
		VAR_SET,
		VAR_RESET,
		VAR_TOGGLE,
		VAR_STROBE,
		VAR_DECREASE,
		VAR_ALPHAN,
		VAR_NUMERIC,
		VAR_APPEND_VALUE,
		VAR_REMOVEVALUE,
		VAR_SWAP_PLUS,
		VAR_APPEND_DECIAL,
		VAR_MOVE_VALUE,
		VAR_RESET_STATIC,
		VAR_MOVE_MIN_VALUE,
		VAR_MOVE_MAX_VALUE,
		VAR_MOVE_AVERAGE_VALUE,
		VAR_SET_DA_ID,
		MAX_VAR_OP_NUM,
	};
public:
	virtual std::string GetEventDescript() const;

	static std::array<QString, MAX_VAR_OP_NUM> m_arrVarOpTypeTxt;

public:
	//��ֵ
	QString m_strBindValue;
	//action
	VAR_ACTION m_eAction;
	//���͵�����
	QString m_strTransformPara;
	//ֵ
	QString m_strValue;
	//ѡͨʱ��
	int m_nStrobeTime;
	//���ֵ
	float m_fMaxValue;
	//��Сֵ
	float m_fMinValue;
	//����ַ��� 
	int m_nMaxCharNums;

};
#endif // BASE_DYNC_EVENT_H


/** @}*/
