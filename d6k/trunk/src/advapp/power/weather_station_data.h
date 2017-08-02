#pragma once

#ifndef WEATHER_STATION_DATA_H
#define WEATHER_STATION_DATA_H

#include <atomic>

#include "tag.h"

// 气象仪采集的信息
class CWeatherStationData
{
public:
	CWeatherStationData();
	~CWeatherStationData();

public:
	void SetTotalRadiationTagname(const std::string& strTotalRadiationTagname);

	void SetDirectRadiationTagname(const std::string& strDirectRadiationTagname);

	void SetScattRadiationTagname(const std::string& strScattRadiationTagname);

	void SetAvergWindDirectTagname(const std::string& strAvergWindDirectTagname);

	void SetAvergWindSpeedTagname(const std::string& strAvergWindSpeedTagname);

	void SetAirTemperatureTagname(const std::string& strAirTemperatureTagname);

	void SetRelativeHumdtyTagname(const std::string& strRelativeHumdtyTagname);

	void SetAirPressureTagname(const std::string& strAirPressureTagname);

	// 初始化
	bool Init();

	// 获取实时数据
	bool GetRealTimeData();

	// 获取总辐射实时数据
	bool GetRealDataOfTotalRadiation(fp64* value);

	// 获取垂直辐射实时数据
	bool GetRealDataOfDirectRadiation(fp64* value);

	// 获取散射辐射实时数据
	bool GetRealDataOfScattRadiation(fp64* value);

	// 获取平均风向实时数据
	bool GetRealDataOfAvergWindDirect(fp64* value);

	// 获取平均风速实时数据
	bool GetRealDataOfAvergWindSpeed(fp64* value);

	// 获取大气温度实时数据
	bool GetRealDataOfAirTemperature(fp64* value);

	// 获取相对湿度实时数据
	bool GetRealDataOfRelativeHumdty(fp64* value);

	// 获取大气压实时数据
	bool GetRealDataOfAirPressure(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:
	// 是否初始化
	std::atomic<bool> m_isInit;

	std::string m_strTotalRadiationTagname;
	std::string m_strDirectRadiationTagname;
	std::string m_strScattRadiationTagname;
	std::string m_strAvergWindDirectTagname;

	std::string m_strAvergWindSpeedTagname;
	std::string m_strAirTemperatureTagname;
	std::string m_strRelativeHumdtyTagname;
	std::string m_strAirPressureTagname;

	//!  总辐射
	std::shared_ptr<CAinData> m_pTotalRadiation;
	//!  垂直辐射
	std::shared_ptr<CAinData> m_pDirectRadiation;
	//!  散射辐射
	std::shared_ptr<CAinData> m_pScattRadiation;
	//!  平均风向
	std::shared_ptr<CAinData> m_pAvergWindDirect;
	//!  平均风速
	std::shared_ptr<CAinData> m_pAvergWindSpeed;
	//!  大气温度
	std::shared_ptr<CAinData> m_pAirTemperature;
	//!  相对湿度
	std::shared_ptr<CAinData> m_pRelativeHumdty;
	//!  大气压
	std::shared_ptr<CAinData> m_pAirPressure;
};

#endif
