#pragma once

#ifndef STATION_INFO
#define STATION_INFO

#include <vector>

#include "power_define.h"

#include "plant_data.h"
#include "inverter_data.h"
#include "weather_station_data.h"
#include "predict_calc_data.h"

class CStationInfo
{
public:
	CStationInfo();
	~CStationInfo();

	// ��ʼ��
	bool Init();

	const std::vector<DATA_INFO>& GetStationInfo();

	// ��ȡ���й������޹�
	std::shared_ptr<CPlantData> GetPlant();

	// ��ȡ�����
	std::shared_ptr<CInverterData> GetInverter();

	// ��ȡ����վ
	std::shared_ptr<CWeatherStationData> GetWeatherStation();

	// ��ȡԤ��
	std::shared_ptr<CPredictCalcData> GetPredictCalc();

private:
	// ����ң�����ݻ��ܣ��������
	std::vector<DATA_INFO>  m_vecStationInfo;

	/********************  ���ݷֿ���Ԥ���㷨  *********************************/
	// ���й������޹�����
	std::shared_ptr<CPlantData> m_pPlantData;

	// ���������
	std::shared_ptr<CInverterData> m_pInverterData;

	// ����վ����
	std::shared_ptr<CWeatherStationData> m_pWeatherStationData;

	// Ԥ������
	std::shared_ptr<CPredictCalcData> m_pPredictCalcData;
};



#endif // STATION_INFO
