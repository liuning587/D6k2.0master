#pragma once

#ifndef ALGORITHM_DEFINE_H
#define ALGORITHM_DEFINE_H

namespace POWER_PREDICT
{
	const int POWER_PREDICT_4H_POINTS_COUNT = 16;

	// todo:
	// 是否要新增预测24小时的功能

	const int POWER_PREDICT_72H_POINTS_COUNT = 288;

	// 超短期预测天气数量
	const int WEATHER_PREDICT_4H_POINTS_COUNT = 16;

	const int WEATHER_PREDICT_24H_POINTS_COUNT = 96;

	// 短期预测天气数量
	const int WEATHER_PREDICT_72H_POINTS_COUNT = 288;

	// 早晨4点
	const int POWER_PREDICT_MORNING_HOUR = 4;

	// 下午16点
	const int POWER_PREDICT_AFTERNOON_HOUR = 16;

	// 晚上20点
	const int POWER_PREDICT_NIGHT_HOUR = 20;

	typedef struct power_predict_point
	{
		float val;
		int   flag;

	}POWER_PREDICT_POINT;

	typedef struct power_predict_4h
	{
		POWER_PREDICT_POINT data[POWER_PREDICT_4H_POINTS_COUNT];

	}POWER_PREDICT_DATA_4H;

	typedef struct power_predict_72h
	{
		POWER_PREDICT_POINT data[POWER_PREDICT_72H_POINTS_COUNT];

	}POWER_PREDICT_DATA_72H;

	// 模拟量名（station.xml组态配置中的Name信息）
	typedef QString ANALOGUS_NAME;

	typedef struct point_info
	{
		int nStationID;
		int nDeviceID;
		int nPointID;
		int nDataType;
	} POINT_INFO;

	enum Weather_Type
	{
		Weather_Total_Radiation = 0,  // 总辐射
		Weather_Scattered_Radiation,  // 散射辐射
		Weather_Direct_Radiation,	  // 垂直辐射
		Weather_Wind_Speed,			  // 平均风速
		Weather_Wind_Direction,		  // 平均风向
		Weather_Temperature,		  // 气温
		Weather_Humidity,			  // 相对湿度
		Weather_Air_Pressure,		  // 气压
		Weather_Type_Count
	};

	// 单个天气预报点数据
	typedef struct weather_predict_point_value
	{
		float val;
		quint8 flag;
	} WEATHER_PREDICT_POINT_VALUE;

	// 存放多个天气预报数据
	typedef QVector<WEATHER_PREDICT_POINT_VALUE> WEATHER_PREDICT_VALUES;

	const int WEATHER_POINT_LOST  =   0x01; // 丢失
	const int WEATHER_POINT_EXIST =   0x02;   // 存在

	const int SECONDS_IN_15_MINITES = 15 * 60; // 15分钟的秒数

}

using namespace POWER_PREDICT;

#endif
