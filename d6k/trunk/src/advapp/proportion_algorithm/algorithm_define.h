#pragma once

#ifndef ALGORITHM_DEFINE_H
#define ALGORITHM_DEFINE_H

namespace POWER_PREDICT
{
	const int POWER_PREDICT_4H_POINTS_COUNT = 16;

	// todo:
	// �Ƿ�Ҫ����Ԥ��24Сʱ�Ĺ���

	const int POWER_PREDICT_72H_POINTS_COUNT = 288;

	// ������Ԥ����������
	const int WEATHER_PREDICT_4H_POINTS_COUNT = 16;

	const int WEATHER_PREDICT_24H_POINTS_COUNT = 96;

	// ����Ԥ����������
	const int WEATHER_PREDICT_72H_POINTS_COUNT = 288;

	// �糿4��
	const int POWER_PREDICT_MORNING_HOUR = 4;

	// ����16��
	const int POWER_PREDICT_AFTERNOON_HOUR = 16;

	// ����20��
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

	// ģ��������station.xml��̬�����е�Name��Ϣ��
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
		Weather_Total_Radiation = 0,  // �ܷ���
		Weather_Scattered_Radiation,  // ɢ�����
		Weather_Direct_Radiation,	  // ��ֱ����
		Weather_Wind_Speed,			  // ƽ������
		Weather_Wind_Direction,		  // ƽ������
		Weather_Temperature,		  // ����
		Weather_Humidity,			  // ���ʪ��
		Weather_Air_Pressure,		  // ��ѹ
		Weather_Type_Count
	};

	// ��������Ԥ��������
	typedef struct weather_predict_point_value
	{
		float val;
		quint8 flag;
	} WEATHER_PREDICT_POINT_VALUE;

	// ��Ŷ������Ԥ������
	typedef QVector<WEATHER_PREDICT_POINT_VALUE> WEATHER_PREDICT_VALUES;

	const int WEATHER_POINT_LOST  =   0x01; // ��ʧ
	const int WEATHER_POINT_EXIST =   0x02;   // ����

	const int SECONDS_IN_15_MINITES = 15 * 60; // 15���ӵ�����

}

using namespace POWER_PREDICT;

#endif
