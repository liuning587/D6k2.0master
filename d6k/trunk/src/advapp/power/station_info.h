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

	// 初始化
	bool Init();

	const std::vector<DATA_INFO>& GetStationInfo();

	void SetStationInfo(DATA_INFO& dataInfo);

	// 获取总有功和总无功
	std::shared_ptr<CPlantData> GetPlant();

	// 获取逆变器
	std::vector<std::shared_ptr<CInverterData>>& GetInverter();

	// 获取气象站
	std::shared_ptr<CWeatherStationData> GetWeatherStation();

	// 获取预测
	std::shared_ptr<CPredictCalcData> GetPredictCalc();

	// 场站ID
	void SetStationID(const int nStationID);
	const int GetStationID() const;

	// 场站名
	void SetStationName(const std::string strStationName);
	const std::string GetStationName() const;

	// 算法名
	void SetAlgorithmName(const std::string strAlgorithmName);
	const std::string GetAlgorithmName() const;

	// 反向隔离路径
	void SetReversePath(const std::string strReversePath);
	const std::string GetReversePath() const;

private:
	// 所有遥测数据汇总，方便入库
	std::vector<DATA_INFO>  m_vecStationInfo;

	/********************  数据分开，预测算法  *********************************/
	// 总有功和总无功数据
	std::shared_ptr<CPlantData> m_pPlantData;

	// 逆变器数据
	std::vector<std::shared_ptr<CInverterData>> m_vecInverterData;

	// 气象站数据
	std::shared_ptr<CWeatherStationData> m_pWeatherStationData;

	// 预测数据
	std::shared_ptr<CPredictCalcData> m_pPredictCalcData;

	// 场站ID
	int m_nStationID;
	// 场站名
	std::string m_strStationName;
	// 算法名
	std::string m_strAlgorithmName;
	// 反向隔离路径
	std::string m_strReversePath;
};



#endif // STATION_INFO
