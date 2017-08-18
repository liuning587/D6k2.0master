#pragma once

#ifndef READ_POWER_XML
#define READ_POWER_XML

#include <memory> // shared_ptr
#include <vector>
#include <string>
#include <QDomElement>

#include "station_info.h"

class CReadPowerXml
{
public:
	CReadPowerXml(const std::string& szPathName);
	~CReadPowerXml();

public:

	// 加载组态配置信息
	bool LoadInfo();

	// 获取所有场站信息
	std::vector<std::shared_ptr<CStationInfo>>& GetAllStationInfo();

private:
	// 加载场站信息
	bool LoadStationInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

	// 加载站内信息
	bool LoadPlantInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);
	// 加载全站静态info
	bool LoadPlantStaticInfo(const QDomElement& dom, std::shared_ptr<CPlantData> pPlantData);
	// 加载全站动态info
	bool LoadPlantDynamicInfo(const QDomElement& dom, std::shared_ptr<CPlantData> pPlantData, std::shared_ptr<CStationInfo> pStationInfo);

	// 加载气象站信息
	bool LoadWeatherInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

	// 加载预测信息
	bool LoadPredictInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

	// 加载逆变器信息
	bool LoadInvertersInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

private:

	// log打印
	void LogMsg(const char *szLogTxt, int nLevel = 0);

private:

	// 所有场站信息
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

	// 预测组态文件路径名
	const std::string m_strPathName;
};


#endif // READ_POWER_XML
