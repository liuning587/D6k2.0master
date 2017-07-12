#ifndef PREDICT_DEFINE_H
#define PREDICT_DEFINE_H

#include <QVector>

const int Weather_Predict_4h_Points = 16;
const int Weather_Predict_72h_Points = 288;
const int Power_Predict_4h_Points = 16;
const int Power_Predict_72h_Points = 288;

typedef struct power_predict_point
{
	float val;
	int   flag;
}Power_Predict_Point;

typedef struct power_predict_4h
{
	Power_Predict_Point data[Power_Predict_4h_Points];
}Power_Predict_Data4h;

typedef struct power_predict_72h
{
	Power_Predict_Point data[Power_Predict_72h_Points];
}Power_Predict_Data72h;

typedef struct analogues_information
{
	int station;
	int device;
	int point;
}Analogues_Info;

enum Weather_Type
{
	Weather_Total_Radiation = 0,
	Weather_Scattered_Radiation,
	Weather_Direct_Radiation,
	Weather_Wind_Speed,
	Weather_Wind_Direction,
	Weather_Temperature,
	Weather_Humidity,
	Weather_Air_Pressure,
	Weather_Type_Count

};

typedef struct weather_predict_point_value
{
	float val;
	unsigned char flag;
}Weather_Predict_Point_Value;


typedef  QVector<Weather_Predict_Point_Value>   Weather_Predict_Points;


#endif // PREDICT_DEFINE_H