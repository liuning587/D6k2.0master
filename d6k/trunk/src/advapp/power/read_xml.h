#pragma once

#ifndef READ_POWER_XML
#define READ_POWER_XML

#include <memory> // shared_ptr
#include <vector>

#include <string>

#include "station_info.h"

class CReadPowerXml
{
public:
	CReadPowerXml(const std::string& szPath);
	~CReadPowerXml();

public:
	std::vector<std::shared_ptr<CStationInfo>>& LoadInfo();

private:
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

};


#endif // READ_POWER_XML
