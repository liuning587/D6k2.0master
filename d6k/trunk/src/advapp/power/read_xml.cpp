#pragma once

#include "read_xml.h"

CReadPowerXml::CReadPowerXml(const std::string& szPath)
{
}

CReadPowerXml::~CReadPowerXml()
{
}

std::vector<std::shared_ptr<CStationInfo>>& CReadPowerXml::LoadInfo()
{
	// todo:
	return m_vecStationInfo;
}
