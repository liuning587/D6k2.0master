/*! @file base_dync.h
<PRE>
********************************************************************************
模块名       :
文件名       :  base_dync_event.h
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
*  \brief 变量操作事件
*  \details  变量操作有：
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
	//绑定值
	QString m_strBindValue;
	//action
	VAR_ACTION m_eAction;
	//传送到变量
	QString m_strTransformPara;
	//值
	QString m_strValue;
	//选通时间
	int m_nStrobeTime;
	//最大值
	float m_fMaxValue;
	//最小值
	float m_fMinValue;
	//最大字符数 
	int m_nMaxCharNums;

};
#endif // BASE_DYNC_EVENT_H


/** @}*/
