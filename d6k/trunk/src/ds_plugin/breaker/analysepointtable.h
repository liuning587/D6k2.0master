#pragma once

#include "breaker_data.h"
#include <memory>
class CAnalysePointTable
{
public:
	CAnalysePointTable();
	~CAnalysePointTable();
	enum 
	{
		//测量值
		ANALOG_VALUE = 1,
		//调试数据
		DEBUG_DATA,
		//开入量
		DI_DATA,
		//开出量
		DO_DATA,
		//动作信号
		ACTION_SIGNAL,
		//异常信号
		ABNORMAL_SIGNAL,
		//软压板信号
		SOFTPLATE_SIGNAL,
		//保护定值
		PROTECT_DEV,
		//系统参数
		SYSTEM_PARM,
		//动作事件
		ACTION_EVENT,
		//异常事件
		ABNORMAL_EVENT,
		//软压板变位事件
		SOFTLATE_CHANGE_EVENT,
		//运行事件
		RUN_EVENT,
	};
public:
	void AnalyseFile(const QString &strFileName);
public:
	//获取测量值
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &GetAnalogInfo()
	{
		return m_mapIDDes;
	}

	//解析测量值
	void AnalyseAnalogItems(const QString &strLineItem);

private:

	//测量值
	std::map<int, std::shared_ptr<CAnalogPointInfo> > m_mapIDDes;

};

