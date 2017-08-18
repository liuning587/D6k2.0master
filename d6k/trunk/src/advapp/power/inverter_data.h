#pragma once

#ifndef INVERTER_DATA_H
#define INVERTER_DATA_H

#include <atomic>

#include "tag.h"

// 逆变器的状态、有功、无功数据
class CInverterData
{
public:
	CInverterData();
	~CInverterData();

public:
	void SetInverterStatusTagname(const std::string& strInverterTagname);

	void SetInverterActivePowerTagname(const std::string& strInverterActiveTagname);

	void SetInverterReactivePowerTagname(const std::string& strInverterReactiveTagname);
	
	void SetInverterVoltageTagname(const std::string& strInverterVoltageTagname);

	// 初始化
	bool Init();

	// 获取实时数据
	bool GetRealTimeData();

	// 获取逆变器分合状态实时数据
	bool GetRealDataOfStatus(int8u* value);

	// 获取逆变器有功实时数据
	bool GetRealDataOfActivePower(fp64* value);	

	// 获取逆变器电压实时数据
	bool GetRealDataOfVoltage(fp64* value);

	// 获取逆变器无功实时数据
	bool GetRealDataOfReactivePower(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:

	// 是否初始化
	std::atomic<bool> m_isInit;

	//! 逆变器状态tagname
	std::string m_strInverterTagname;
	//! 逆变器有功tagname
	std::string m_strInverterActiveTagname;
	//! 逆变器无功tagname
	std::string m_strInverterReactiveTagname;
	//! 逆变器电压tagname
	std::string m_strInverterVoltageTagname;

	//! 逆变器分合状态
	std::shared_ptr<CDinData> m_pInverterStatus;
	//!  逆变器有功
	std::shared_ptr<CAinData> m_pInverterActivePower;
	//! 逆变器无功
	std::shared_ptr<CAinData> m_pInverterReactivePower;
	//! 逆变器电压
	std::shared_ptr<CAinData> m_pInverterVoltage;

	//! 设备ID
	int m_nDeviceID;
};

#endif // INVERTER_DATA_H
