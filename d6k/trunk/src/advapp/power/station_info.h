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

	// 获取总有功和总无功
	std::shared_ptr<CPlantData> GetPlant();

	// 获取逆变器
	std::shared_ptr<CInverterData> GetInverter();

	// 获取气象站
	std::shared_ptr<CWeatherStationData> GetWeatherStation();

	// 获取预测
	std::shared_ptr<CPredictCalcData> GetPredictCalc();

private:
	// 所有遥测数据汇总，方便入库
	std::vector<DATA_INFO>  m_vecStationInfo;

	/********************  数据分开，预测算法  *********************************/
	// 总有功和总无功数据
	std::shared_ptr<CPlantData> m_pPlantData;

	// 逆变器数据
	std::shared_ptr<CInverterData> m_pInverterData;

	// 气象站数据
	std::shared_ptr<CWeatherStationData> m_pWeatherStationData;

	// 预测数据
	std::shared_ptr<CPredictCalcData> m_pPredictCalcData;
};



#endif // STATION_INFO
