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

	void SetStationInfo(DATA_INFO& dataInfo);

	// ��ȡ���й������޹�
	std::shared_ptr<CPlantData> GetPlant();

	// ��ȡ�����
	std::vector<std::shared_ptr<CInverterData>>& GetInverter();

	// ��ȡ����վ
	std::shared_ptr<CWeatherStationData> GetWeatherStation();

	// ��ȡԤ��
	std::shared_ptr<CPredictCalcData> GetPredictCalc();

	// ��վID
	void SetStationID(const int nStationID);
	const int GetStationID() const;

	// ��վ��
	void SetStationName(const std::string strStationName);
	const std::string GetStationName() const;

	// �㷨��
	void SetAlgorithmName(const std::string strAlgorithmName);
	const std::string GetAlgorithmName() const;

	// �������·��
	void SetReversePath(const std::string strReversePath);
	const std::string GetReversePath() const;

private:
	// ����ң�����ݻ��ܣ��������
	std::vector<DATA_INFO>  m_vecStationInfo;

	/********************  ���ݷֿ���Ԥ���㷨  *********************************/
	// ���й������޹�����
	std::shared_ptr<CPlantData> m_pPlantData;

	// ���������
	std::vector<std::shared_ptr<CInverterData>> m_vecInverterData;

	// ����վ����
	std::shared_ptr<CWeatherStationData> m_pWeatherStationData;

	// Ԥ������
	std::shared_ptr<CPredictCalcData> m_pPredictCalcData;

	// ��վID
	int m_nStationID;
	// ��վ��
	std::string m_strStationName;
	// �㷨��
	std::string m_strAlgorithmName;
	// �������·��
	std::string m_strReversePath;
};



#endif // STATION_INFO
