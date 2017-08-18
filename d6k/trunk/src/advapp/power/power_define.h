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

#define PLANT_ACTIVE_POWER		"Plant Active Power"		// ���й�
#define PLANT_REACTIVE_POWER	"Plant Reactive Power"		// ���޹�

#define TOTAL_RADIATION			"Total Radiation"			// �ܷ���
#define DIRECT_RADIATION		"Direct Radiation"			// ��ֱ����
#define SCATTERING_RADIATION	"Scattering Radiation"		// ɢ�����
#define AVERAGE_WIND_DIRECTION	"Average Wind Direction"	// ƽ������
#define AVERAGE_WIND_SPEED		"Average Wind Speed"		// ƽ������
#define AIR_TEMPERATURE			"Air Temperature"			// ����
#define RELATIVE_HUMIDITY		"Relative Humidity"			// ���ʪ��
#define AIR_PRESSURE			"Air Pressure"				// ��ѹ
#define COMPONENT_TEMPERATURE	"Component temperature"		// ����¶�

#define PLANT_4_HOUR_PREDICATION_VALUE		"Plant 4 Hour Predication Value"		// ������Ԥ������
#define PLANT_72_HOUR_PREDICATION_VALUE		"Plant 72 Hour Predication Value"		// ����Ԥ������

#define INVERTER_ACTIVE_POWER		"Inverter Active Power"		// ������й�
#define INVERTER_REACTIVE_POWER		"Inverter ReActive Power"	// ������޹�
#define INVERTER_VOLTAGE			"Inverter Voltage"			// �������ѹ
#define INVERTER_STATUS				"Inverter State"			// �����״̬

#define ALL_HISTORY_DATA_CONNECT_NAME		"ALL_HISTORY_DATA"	// ���в�ֵ�������������
#define PREDICT_DATA_CONNECT_NAME			"PREDICT_DATA"		// Ԥ���������������
#define WEATHER_DATA_CONNECT_NAME			"WEATHER_DATA"		// ftp�����������������

/****************************************************** Begin:���ݿ���� ******************************************************/
#define TABLE_NAME_HISTORY_ANALOGUES_DAY			   "tbHistory_Analogues_Day"				// ��ֵ��ʷ���ݱ���
#define TABLE_NAME_INVERTER_STATUS  				   "tbInverter_Status"						// �����״̬����

#define TABLE_NAME_WEATHER_PREDICT_4H				   "tbWeather_Predict_4h"					// ������Ԥ��4Сʱ��������
#define TABLE_NAME_WEATHER_PREDICT_4H_Final_15M		   "tbWeather_Predict_4h_Final_15m"			// ������Ԥ�����һ��15������������
#define TABLE_NAME_WEATHER_PREDICT_72H				   "tbWeather_Predict_72h"					// ����Ԥ��72Сʱ��������
#define TABLE_NAME_WEATHER_PREDICT_72H_FINAL_24H	   "tbWeather_Predict_72h_Final_24h"		// ����Ԥ�����һ����������

#define TABLE_NAME_POWER_PREDICT_72H				   "tbPower_predict_72h"					// ����Ԥ���(������ж���Ԥ������)
#define TABLE_NAME_POWER_PREDICT_72H_FINAL_24H		   "tbPower_predict_72h_Final_24h"			// ����Ԥ��(���һ���ֵ)
#define TABLE_NAME_POWER_PREDICT_72H_FINAL_MANUAL_24H  "tbPower_predict_72h_Final_Manual_24h"	// ����Ԥ��(���һ���ֶ��޸�ֵ)

#define TABLE_NAME_POWER_PREDICT_4H					   "tbPower_Predict_4h"						// ������Ԥ���(������г�����Ԥ������)
#define TABLE_NAME_POWER_PREDICT_4H_FINAL_15M		   "tbPower_Predict_4h_Final_15m"			// ������Ԥ��(���һ��15����ֵ)
#define TABLE_NAME_POWER_PREDICT_4H_FINAL_MANUAL_15M   "tbPower_Predict_4h_Final_Manual_15m"	// ������Ԥ��(����ֶ��޸����һ��15����ֵ)

#define TABLE_NAME_MAINTENANCE_CAPACITY				   "tbMaintenance_Capacity"					// ��������(���������)
#define TABLE_NAME_RUNNING_CAPACITY					   "tbRunning_Capacity"						// ��������(�������õ�)
#define TABLE_NAME_LIMIT_CAPACITY					   "tbLimit_Capacity"						// �й���ֵ
/****************************************************** End:���ݿ���� ******************************************************/

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
	DATA_TYPE_PLANT_ACTIVE_POWER		= 1,				// ���й�
	DATA_TYPE_PLANT_REACTIVE_POWER		= 2,				// ���޹�								
	DATA_TYPE_TOTAL_RADIATION			= 3,				// �ܷ���
	DATA_TYPE_DIRECT_RADIATION			= 4,				// ��ֱ����
	DATA_TYPE_SCATTERING_RADIATION		= 5,				// ɢ�����
	DATA_TYPE_AVERAGE_WIND_DIRECTION	= 6,				// ƽ������
	DATA_TYPE_AVERAGE_WIND_SPEED		= 7,				// ƽ������
	DATA_TYPE_AIR_TEMPERATURE			= 8,				// ����
	DATA_TYPE_RELATIVE_HUMIDITY			= 9,				// ���ʪ��
	DATA_TYPE_AIR_PRESSURE				= 10,				// ��ѹ
	DATA_TYPE_COMPONENT_TEMPERATURE		= 11,				// ����¶�
	DATA_TYPE_PLANT_4_HOUR_PREDICATION_VALUE		= 12,	// ������Ԥ������
	DATA_TYPE_PLANT_72_HOUR_PREDICATION_VALUE		= 13	// ����Ԥ������
};



#endif // POWER_DEFINE_H

