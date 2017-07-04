/*! @file base_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_widget.cpp
�ļ�ʵ�ֹ��� :  �����Ķ�̬����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����Ķ�̬����
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/

#include "dync_var_op_event.h"

#include "fesapi/fesdatadef.h"
#include <QObject>
#include <QString>
#include<array>

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif

#include <memory>
#include <array>
#include <QObject>
#include <QActionGroup>
#include "../scdpub/variant_data.h"

std::array<QString, CDyncVarOpEventItem::MAX_VAR_OP_NUM> CDyncVarOpEventItem::m_arrVarOpTypeTxt =
{
	QObject::tr("set"),
	QObject::tr("reset"),
	QObject::tr("toggle"),
	QObject::tr("strobe"),
	QObject::tr("Print"),
	QObject::tr("increase"),
	QObject::tr("decrease"),
	QObject::tr("alphanumeric pad"),
	QObject::tr("numeric pad"),
	QObject::tr("Append Value"),
	QObject::tr("Remove Value"),
	QObject::tr("Swap Plus-Minus"),
	QObject::tr("Append Decimal Mode ON-OFF"),
	QObject::tr("Move Value"),
	QObject::tr("Reset Statistic"),
	QObject::tr("Move Min.Value"),
	QObject::tr("Move Max. Value"),
};
 
CDyncVarOpEventItem::CDyncVarOpEventItem()
{
	m_eAction = VAR_SET;
	//ѡͨʱ��
	m_nStrobeTime = 0;
	//���ֵ
     m_fMaxValue = 100.0;
	//��Сֵ
	m_fMinValue = 0.0;
	//����ַ���
	m_nMaxCharNums = 0;
}

CDyncVarOpEventItem::~CDyncVarOpEventItem()
{

}

CDyncVarOpEventItem& CDyncVarOpEventItem::operator=(const CDyncVarOpEventItem& src)
{
	if (this != &src)
	{
		Q_ASSERT(src.m_OpEventType == DYNC_VAR_OP);

		this->m_bEnable = src.m_bEnable;
		this->m_OpEventType = src.m_OpEventType;

		m_strBindValue = src.m_strBindValue;;

		m_eAction = src.m_eAction;
		m_strTransformPara = src.m_strTransformPara;;
		m_strValue = src.m_strValue;;
		m_nStrobeTime = src.m_nStrobeTime;;
		m_fMaxValue = src.m_fMaxValue;;
		m_fMinValue = src.m_fMinValue;;
		m_nMaxCharNums = src.m_nMaxCharNums;;

	}

	return *this;
}

std::string CDyncVarOpEventItem::GetEventDescript() const
{
	return ("");
}
/** @}*/
