/*! @file base_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  base_widget.cpp
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
	//选通时间
	m_nStrobeTime = 0;
	//最大值
     m_fMaxValue = 100.0;
	//最小值
	m_fMinValue = 0.0;
	//最大字符数
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
