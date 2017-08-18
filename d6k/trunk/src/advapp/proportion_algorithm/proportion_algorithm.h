#pragma once

#include <vector>
#include <memory>

#include <QList>
#include <QMap>
#include <QVector>
#include <QDateTime>

#include "proportion_algorithm_global.h"
#include "station_info.h"
#include "algorithm_define.h"
#include "dbapi.h"


class CProportionAlgorithm
{
public:
	CProportionAlgorithm();
	~CProportionAlgorithm();

	// 初始化
	bool Init(const std::shared_ptr<CStationInfo> pStationInfo);

	// 短期预测
	void PowerPredict72h(const QDateTime& currTime);

	// 超短期预测
	void PowerPredict4h(const QDateTime& currTime);

private:
	
	QString SetDateFormat(const QDateTime &dateTime)
	{
		return QString("%1").arg(dateTime.toString("yyyy-MM-dd"));
	}

	QString SetTimeFormat(const QDateTime &dateTime)
	{
		int hour = dateTime.time().hour();
		int minu = (dateTime.time().minute() / 15) * 15;

		return QString("%1 %2:%3:00").arg(dateTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));
	}

	QString SetTimeFormatActual(const QDateTime &dateTime)
	{
		int hour = dateTime.time().hour();
		int minu = dateTime.time().minute();
		int sec = dateTime.time().second();

		return QString("%1 %2:%3:%4").arg(dateTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
	}

private:

	// 加载测值信息
	bool GetAnalogusInfo();

	// 获取昨天超短期的数据总辐射和总有功
	bool GetYesterdayValue4h(const QDateTime& time);

	// 获取昨天短期的数据总辐射和总有功
	bool GetYesterdayValue72h(const QDateTime& time);

	// 获取超短期天气数据
	bool GetDataFromWeatherPredict4h(const QDateTime& time);

	// 获取短期天气数据
	bool GetDataFromWeatherPredict72h(const QDateTime& time);

	// 获取检修值
	QVector<float> GetMaintenanceValue(const QDateTime& startTime, const QDateTime& endTime);

	QString SetFieldsValue(const float& value, const int nFieldIndex);

	// 保存超短期预测数据到数据库中
	void SavePowerPredictData4h(const QDateTime& insertTime);
	void SavePowerPredictData4h(POWER_PREDICT_DATA_4H& data, const QDateTime& insertTime);

	// 保存超短期数据中的最后一个15分钟数据到数据库中 （power_predict_4h_final_15m）
	void SaveDataPowerPredict4h_Final_15m(POWER_PREDICT_DATA_4H& data, const QDateTime& insertTime);

	// 保存短期预测数据到数据库中
	void SavePowerPredictData72h(const QDateTime& insertTime);
	void SavePowerPredictData72h(POWER_PREDICT_DATA_72H& data, const QDateTime& insertTime);

	// 保存短期数据中最后一天的数据到数据库中 （power_predict_72h_final_15m）
	void SaveDataPowerPredict72h_Final_24h(POWER_PREDICT_DATA_72H& data, const QDateTime& insertTime);

private:

	// 场站信息
	std::shared_ptr<CStationInfo> m_pStationInfo;

	// 总辐射和总有功
	QMap<ANALOGUS_NAME, POINT_INFO> m_mapDataInfo;

	// 存放昨天超短期预测的数据
	QMap<ANALOGUS_NAME, QVector<float>> m_mapYesterday4h;

	// 存放昨天短期预测的数据
	QMap<ANALOGUS_NAME, QVector<float>> m_mapYesterday72h;	

	// 超短期预测天气预报数据
	QMap<Weather_Type, WEATHER_PREDICT_VALUES> m_mapPredictWeather4h;

	// 短期预测天气预报数据
	QMap<Weather_Type, WEATHER_PREDICT_VALUES> m_mapPredictWeather72h;

	// 超短期预测数据
	POWER_PREDICT_DATA_4H m_powerPredictData4h;

	// 短期预测数据
	POWER_PREDICT_DATA_72H m_powerPredictData72h;

private:

	// 装机容量
	float m_fCapacity;

	// 历史表中值的开始位置
	static const int m_nYesterday4h_Value_First_Field = 7;

	// 天气预报表中值的开始位置（ftp）
	static const int m_nPredict_Value_First_Field = 3;

	// 是否初始化
	std::atomic<bool> m_isInit;

	// DB接口
	std::shared_ptr<CDB_API> m_pDB_api;

	// 场站ID
	int m_nStation;
};
