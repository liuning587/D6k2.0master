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
	//实时映射文件
	void AnalyseRealFile(const QString &strFileName);
public:
	//获取测量值
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &GetAnalogInfo()
	{
		return m_mapAnalogIDDes;
	}

	//获取调试信息
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &GetDebugInfo()
	{
		return m_mapDebugIDDes;
	}
	//获取保护定值
	const std::map<int, std::shared_ptr<CDevInfo> > &GetProtectDevInfo()
	{
		return m_mapProtectDevIDDes;
	}
	//获取系统参数定值
	const std::map<int, std::shared_ptr<CDevInfo> > &GetSystemInfoDevInfo()
	{
		return m_mapSystemDevIDDes;
	}

	//获取开入量
	const std::map<int, std::shared_ptr<CDIDOInfo> > &GetDIInfo()
	{
		return m_mapDIIDDes;
	}

	//获取开出量
	const std::map<int, std::shared_ptr<CDIDOInfo> > &GetDOInfo()
	{
		return m_mapDOIDDes;
	}

	//获取动作信号
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetActionInfo()
	{
		return m_mapActionIDDes;
	}

	//获取异常信号
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetAbnormalInfo()
	{
		return m_mapAbNormalIDDes;
	}

	//获取软压板信息
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetSoftInfo()
	{
		return m_mapSoftIDDes;
	}

	//获取动作事件
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetActionEventInfo()
	{
		return m_mapActionEventIDDes;
	}
	//获取异常事件
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetAbnromalEventInfo()
	{
		return m_mapAbnormalEventIDDes;
	}

	//运行事件
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetRunEventInfo()
	{
		return m_mapRunEventIDDes;
	}

	int GetDiIndex()
	{
		return m_nDiIndex;
	}

	int GetDoIndex()
	{
		return m_nDoIndex;
	}

	int GetSoftIndex()
	{
		return m_nSoft;
	}

	int GetAction()
	{
		return m_nAction;
	}
	//异常信号
	int GetAbnormal()
	{
		return m_nAbnormal;
	}

public:

	//解析测量值
	void AnalyseAnalogItems(const QString &strLineItem);
	//解析调试信息
	void AnalyseDebugItems(const QString &strLineItem);
	//解析保护定值
	void AnalyseProtectDevItems(const QString &strLineItem);
	//解析系统参数
	void AnalyseSystemDevItems(const QString &strLineItem);

	//解析开入量
	void AnalyseDIItems(const QString &strLineItem);
    //解析开出量
	void AnalyseDOItems(const QString &strLineItem);
	//解析动作信号
	void AnayseAction(const QString &strLineItem);
	//解析异常信号
	void AnalyseAbnormal(const QString &strLineItem);
	//设置软压板信号
	void AnayseSoftDev(const QString &strLineItem);
	//设置动作事件
	void AnalyseActionEvent(const QString &strLineItem);
	//设置异常事件
	void AnalyseAbnormalEvent(const QString &strLineItem);
	//设置运行事件
	void AnalyseRunEvent(const QString &strLineItem);

private:

	//测量值
	std::map<int, std::shared_ptr<CAnalogPointInfo> > m_mapAnalogIDDes;
	//调试信息
	std::map<int, std::shared_ptr<CAnalogPointInfo> > m_mapDebugIDDes;
	//保护定值
	std::map<int, std::shared_ptr<CDevInfo> > m_mapProtectDevIDDes;
	//系统参数定值
	std::map<int, std::shared_ptr<CDevInfo> > m_mapSystemDevIDDes;
	//开入量
	std::map<int, std::shared_ptr<CDIDOInfo> > m_mapDIIDDes;
	//开出量
	std::map<int, std::shared_ptr<CDIDOInfo> > m_mapDOIDDes;
	//动作信号
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapActionIDDes;
	//异常信号
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapAbNormalIDDes;
	//软压板
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapSoftIDDes;
	//动作事件
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapActionEventIDDes;
	//异常事件
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapAbnormalEventIDDes;
	//运行事件
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapRunEventIDDes;
	//di index
	int m_nDiIndex;
	//do index
	int m_nDoIndex;
	//action
	int m_nAction;
	//abnormal
	int m_nAbnormal;
	//soft
	int m_nSoft;
};

