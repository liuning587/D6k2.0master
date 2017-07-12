
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "predict_define.h"

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMap>

class BaseAlgorithm : public QObject
{
	Q_OBJECT

public:
	BaseAlgorithm(QObject *parent = 0);
	virtual void PowerPredict4h(const QDateTime& currTime) = 0;
	virtual void PowerPredict72h(const QDateTime& currTime) = 0;

public:
	Power_Predict_Data4h power_predict_data_4h;
	Power_Predict_Data72h power_predict_data_72h;
	QVector<float> maintenanceValues4h;
	QVector<float> maintenanceValues72h;

protected:
	QMap<QString, Analogues_Info> GetAnalogueInfo(QList<QString>& alias);

public:
	float Capacity;
};


class ProportionAlgorithm : public BaseAlgorithm
{
	Q_OBJECT

public:
	ProportionAlgorithm();
	void PowerPredict4h(const QDateTime& currTime);
	void PowerPredict72h(const QDateTime& currTime);

private:

	void InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Points>& map, Weather_Type type);
	void InsertWeatherType72h(QMap<Weather_Type, Weather_Predict_Points>& map, Weather_Type type);

	void insertWeatherType4h(QMap<Weather_Type, Weather_Predict_Points> & map, QList<Weather_Type> listType);
	void insertWeatherType72h(QMap<Weather_Type, Weather_Predict_Points> & map, QList<Weather_Type> listType);

private:

	QList<QString> a_Alias;
	QMap<QString, Analogues_Info> a_Info;
	QMap<QString, QVector<float> > map_yesterday_4h;
	QMap<QString, QVector<float> > map_yesterday_72h;
	QMap<Weather_Type, Weather_Predict_Points> map_predict_weather_4h;
	QMap<Weather_Type, Weather_Predict_Points> map_predict_weather_72h;

	QMap<Weather_Type, QString> map_weather_analogues_alias;
	Power_Predict_Data4h power_predict_data_4h;
	Power_Predict_Data72h power_predict_data_72h;
};

#endif // ALGORITHM_H