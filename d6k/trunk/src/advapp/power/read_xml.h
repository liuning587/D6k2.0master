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

	// ������̬������Ϣ
	bool LoadInfo();

	// ��ȡ���г�վ��Ϣ
	std::vector<std::shared_ptr<CStationInfo>>& GetAllStationInfo();

private:
	// ���س�վ��Ϣ
	bool LoadStationInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

	// ����վ����Ϣ
	bool LoadPlantInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);
	// ����ȫվ��̬info
	bool LoadPlantStaticInfo(const QDomElement& dom, std::shared_ptr<CPlantData> pPlantData);
	// ����ȫվ��̬info
	bool LoadPlantDynamicInfo(const QDomElement& dom, std::shared_ptr<CPlantData> pPlantData, std::shared_ptr<CStationInfo> pStationInfo);

	// ��������վ��Ϣ
	bool LoadWeatherInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

	// ����Ԥ����Ϣ
	bool LoadPredictInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

	// �����������Ϣ
	bool LoadInvertersInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo);

private:

	// log��ӡ
	void LogMsg(const char *szLogTxt, int nLevel = 0);

private:

	// ���г�վ��Ϣ
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

	// Ԥ����̬�ļ�·����
	const std::string m_strPathName;
};


#endif // READ_POWER_XML
