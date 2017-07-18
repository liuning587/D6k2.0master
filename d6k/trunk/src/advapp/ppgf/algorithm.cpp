#include "algorithm.h"
#include "predict_define.h"

BaseAlgorithm::BaseAlgorithm(QObject *parent /* = 0 */) :
	QObject(parent)
{
	Capacity = 20; //临时设定

	//日志
}


QMap<QString, Analogues_Info> BaseAlgorithm::GetAnalogueInfo(QList<QString>& alias)
{
	//从数据库获取模拟量信息

	QMap<QString, Analogues_Info> map;

	Q_UNUSED(alias);

	return map;
}


ProportionAlgorithm::ProportionAlgorithm()
{

}


void ProportionAlgorithm::InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Points>& map, Weather_Type type)
{
	Weather_Predict_Points weatherPredictData4h(Weather_Predict_4h_Points);
	map.insert(type, weatherPredictData4h);
}


void ProportionAlgorithm::insertWeatherType4h(QMap<Weather_Type, Weather_Predict_Points> & map, QList<Weather_Type> listType)
{
	Weather_Predict_Points weatherPredictData4h(Weather_Predict_4h_Points);

	foreach(Weather_Type type, listType)
	{
		map.insert(type, weatherPredictData4h);
	}
}


void ProportionAlgorithm::InsertWeatherType72h(QMap<Weather_Type, Weather_Predict_Points>& map, Weather_Type type)
{
	Weather_Predict_Points weatherPredictData72h(Weather_Predict_72h_Points);
	map.insert(type, weatherPredictData72h);
}


void ProportionAlgorithm::insertWeatherType72h(QMap<Weather_Type, Weather_Predict_Points> & map, QList<Weather_Type> listType)
{
	Weather_Predict_Points weatherPredictData72h(Weather_Predict_72h_Points);

	foreach(Weather_Type type, listType)
	{
		map.insert(type, weatherPredictData72h);
	}
}


void ProportionAlgorithm::PowerPredict4h(const QDateTime& currTime)
{
	QDateTime predictStartTime, predictEndTime;
	bool isFind4h = false;
	QVector<float> maintenanceValues;
	QString str;
	bool tmptest = false;

	predictStartTime = currTime.addSecs(15 * 60);
	int hour = predictStartTime.time().hour();
	int minu = (predictStartTime.time().minute() / 15) * 15;

	str = QString("%1 %2:%3:00").arg(predictStartTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));

	predictStartTime = QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss");

	predictEndTime = predictStartTime.addSecs(60 * 60 * 4 - 15 * 60);

	
}

void ProportionAlgorithm::PowerPredict72h(const QDateTime& currTime)
{
	Q_UNUSED(currTime);
}


