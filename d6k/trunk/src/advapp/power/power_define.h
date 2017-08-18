#pragma once

#ifndef POWER_DEFINE_H
#define POWER_DEFINE_H

#include <string>

#define LOG_FILENAME  "power"

#define HISTORY_DATA_SAVE_PERIOD		 5 * 1000

#define PREDICT_DATA_SAVE_PERIOD		 15 * 1000

#define WEATHER_FILE_DATA_SAVE_PERIOD    1 * 1000

#define APP_NAME	"POWER"

#define POWER_PREDICT_PATH_NAME "/powerpredict/station.xml"

#define PLANT_ACTIVE_POWER		"Plant Active Power"		// 总有功
#define PLANT_REACTIVE_POWER	"Plant Reactive Power"		// 总无功

#define TOTAL_RADIATION			"Total Radiation"			// 总辐射
#define DIRECT_RADIATION		"Direct Radiation"			// 垂直辐射
#define SCATTERING_RADIATION	"Scattering Radiation"		// 散射辐射
#define AVERAGE_WIND_DIRECTION	"Average Wind Direction"	// 平均风向
#define AVERAGE_WIND_SPEED		"Average Wind Speed"		// 平均风速
#define AIR_TEMPERATURE			"Air Temperature"			// 气温
#define RELATIVE_HUMIDITY		"Relative Humidity"			// 相对湿度
#define AIR_PRESSURE			"Air Pressure"				// 气压
#define COMPONENT_TEMPERATURE	"Component temperature"		// 组件温度

#define PLANT_4_HOUR_PREDICATION_VALUE		"Plant 4 Hour Predication Value"		// 超短期预测数据
#define PLANT_72_HOUR_PREDICATION_VALUE		"Plant 72 Hour Predication Value"		// 短期预测数据

#define INVERTER_ACTIVE_POWER		"Inverter Active Power"		// 逆变器有功
#define INVERTER_REACTIVE_POWER		"Inverter ReActive Power"	// 逆变器无功
#define INVERTER_VOLTAGE			"Inverter Voltage"			// 逆变器电压
#define INVERTER_STATUS				"Inverter State"			// 逆变器状态

#define ALL_HISTORY_DATA_CONNECT_NAME		"ALL_HISTORY_DATA"	// 所有测值数据入库连接名
#define PREDICT_DATA_CONNECT_NAME			"PREDICT_DATA"		// 预测数据入库连接名
#define WEATHER_DATA_CONNECT_NAME			"WEATHER_DATA"		// ftp气象数据入库连接名

/****************************************************** Begin:数据库表名 ******************************************************/
#define TABLE_NAME_HISTORY_ANALOGUES_DAY			   "tbHistory_Analogues_Day"				// 测值历史数据表名
#define TABLE_NAME_INVERTER_STATUS  				   "tbInverter_Status"						// 逆变器状态表名

#define TABLE_NAME_WEATHER_PREDICT_4H				   "tbWeather_Predict_4h"					// 超短期预测4小时天气数据
#define TABLE_NAME_WEATHER_PREDICT_4H_Final_15M		   "tbWeather_Predict_4h_Final_15m"			// 超短期预测最后一个15分钟天气数据
#define TABLE_NAME_WEATHER_PREDICT_72H				   "tbWeather_Predict_72h"					// 短期预测72小时天气数据
#define TABLE_NAME_WEATHER_PREDICT_72H_FINAL_24H	   "tbWeather_Predict_72h_Final_24h"		// 短期预测最后一天天气数据

#define TABLE_NAME_POWER_PREDICT_72H				   "tbPower_predict_72h"					// 短期预测表(存放所有短期预测数据)
#define TABLE_NAME_POWER_PREDICT_72H_FINAL_24H		   "tbPower_predict_72h_Final_24h"			// 短期预测(最后一天的值)
#define TABLE_NAME_POWER_PREDICT_72H_FINAL_MANUAL_24H  "tbPower_predict_72h_Final_Manual_24h"	// 短期预测(最后一天手动修改值)

#define TABLE_NAME_POWER_PREDICT_4H					   "tbPower_Predict_4h"						// 超短期预测表(存放所有超短期预测数据)
#define TABLE_NAME_POWER_PREDICT_4H_FINAL_15M		   "tbPower_Predict_4h_Final_15m"			// 超短期预测(最后一个15分钟值)
#define TABLE_NAME_POWER_PREDICT_4H_FINAL_MANUAL_15M   "tbPower_Predict_4h_Final_Manual_15m"	// 超短期预测(存放手动修改最后一个15分钟值)

#define TABLE_NAME_MAINTENANCE_CAPACITY				   "tbMaintenance_Capacity"					// 检修容量(逆变器检修)
#define TABLE_NAME_RUNNING_CAPACITY					   "tbRunning_Capacity"						// 运行容量(均方根用到)
#define TABLE_NAME_LIMIT_CAPACITY					   "tbLimit_Capacity"						// 有功限值
/****************************************************** End:数据库表名 ******************************************************/

struct DATA_INFO
{
	int nStationId;
	int nDeviceId;
	int nPointId;
	std::string strDesc;
	std::string strTagName;
};


enum POWER_DATA_TYPE
{
	DATA_TYPE_PLANT_ACTIVE_POWER		= 1,				// 总有功
	DATA_TYPE_PLANT_REACTIVE_POWER		= 2,				// 总无功								
	DATA_TYPE_TOTAL_RADIATION			= 3,				// 总辐射
	DATA_TYPE_DIRECT_RADIATION			= 4,				// 垂直辐射
	DATA_TYPE_SCATTERING_RADIATION		= 5,				// 散射辐射
	DATA_TYPE_AVERAGE_WIND_DIRECTION	= 6,				// 平均风向
	DATA_TYPE_AVERAGE_WIND_SPEED		= 7,				// 平均风速
	DATA_TYPE_AIR_TEMPERATURE			= 8,				// 气温
	DATA_TYPE_RELATIVE_HUMIDITY			= 9,				// 相对湿度
	DATA_TYPE_AIR_PRESSURE				= 10,				// 气压
	DATA_TYPE_COMPONENT_TEMPERATURE		= 11,				// 组件温度
	DATA_TYPE_PLANT_4_HOUR_PREDICATION_VALUE		= 12,	// 超短期预测数据
	DATA_TYPE_PLANT_72_HOUR_PREDICATION_VALUE		= 13	// 短期预测数据
};



#endif // POWER_DEFINE_H

