#pragma once

#ifndef PLANT_DATA
#define PLANT_DATA

#include <atomic>

#include "tag.h"

class CPlantData
{
public:
	CPlantData();
	~CPlantData();

public:	

	void SetActPowerTagname(const std::string& strActPowerTagame);

	void SetReactPowerTagname(const std::string& strReactPowerTagname);

	// 初始化
	bool Init();

	// 获取实时数据
	bool GetRealTimeData();

	// 获取总有功实时数据
	bool GetRealDataOfActPower(fp64* value);

	// 获取总无功实时数据
	bool GetRealDataOfReactPower(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:

	// 是否初始化
	std::atomic<bool> m_isInit;

	//!  总有功
	std::string m_strActPowerTagname;
	std::shared_ptr<CAinData> m_pActPower;	
	
	//!  总无功
	std::string m_strReactPowerTagname;
	std::shared_ptr<CAinData> m_pReactPower;
};

#endif // PLANT_DATA
