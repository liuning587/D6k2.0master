#pragma once

#ifndef WEATHER_STATION_DATA_H
#define WEATHER_STATION_DATA_H

#include <atomic>

#include "tag.h"

// �����ǲɼ�����Ϣ
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

	// ��ʼ��
	bool Init();

	// ��ȡʵʱ����
	bool GetRealTimeData();

	// ��ȡ�ܷ���ʵʱ����
	bool GetRealDataOfTotalRadiation(fp64* value);

	// ��ȡ��ֱ����ʵʱ����
	bool GetRealDataOfDirectRadiation(fp64* value);

	// ��ȡɢ�����ʵʱ����
	bool GetRealDataOfScattRadiation(fp64* value);

	// ��ȡƽ������ʵʱ����
	bool GetRealDataOfAvergWindDirect(fp64* value);

	// ��ȡƽ������ʵʱ����
	bool GetRealDataOfAvergWindSpeed(fp64* value);

	// ��ȡ�����¶�ʵʱ����
	bool GetRealDataOfAirTemperature(fp64* value);

	// ��ȡ���ʪ��ʵʱ����
	bool GetRealDataOfRelativeHumdty(fp64* value);

	// ��ȡ����ѹʵʱ����
	bool GetRealDataOfAirPressure(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:
	// �Ƿ��ʼ��
	std::atomic<bool> m_isInit;

	std::string m_strTotalRadiationTagname;
	std::string m_strDirectRadiationTagname;
	std::string m_strScattRadiationTagname;
	std::string m_strAvergWindDirectTagname;

	std::string m_strAvergWindSpeedTagname;
	std::string m_strAirTemperatureTagname;
	std::string m_strRelativeHumdtyTagname;
	std::string m_strAirPressureTagname;

	//!  �ܷ���
	std::shared_ptr<CAinData> m_pTotalRadiation;
	//!  ��ֱ����
	std::shared_ptr<CAinData> m_pDirectRadiation;
	//!  ɢ�����
	std::shared_ptr<CAinData> m_pScattRadiation;
	//!  ƽ������
	std::shared_ptr<CAinData> m_pAvergWindDirect;
	//!  ƽ������
	std::shared_ptr<CAinData> m_pAvergWindSpeed;
	//!  �����¶�
	std::shared_ptr<CAinData> m_pAirTemperature;
	//!  ���ʪ��
	std::shared_ptr<CAinData> m_pRelativeHumdty;
	//!  ����ѹ
	std::shared_ptr<CAinData> m_pAirPressure;
};

#endif
