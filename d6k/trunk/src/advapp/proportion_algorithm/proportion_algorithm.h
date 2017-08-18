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

	// ��ʼ��
	bool Init(const std::shared_ptr<CStationInfo> pStationInfo);

	// ����Ԥ��
	void PowerPredict72h(const QDateTime& currTime);

	// ������Ԥ��
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

	// ���ز�ֵ��Ϣ
	bool GetAnalogusInfo();

	// ��ȡ���쳬���ڵ������ܷ�������й�
	bool GetYesterdayValue4h(const QDateTime& time);

	// ��ȡ������ڵ������ܷ�������й�
	bool GetYesterdayValue72h(const QDateTime& time);

	// ��ȡ��������������
	bool GetDataFromWeatherPredict4h(const QDateTime& time);

	// ��ȡ������������
	bool GetDataFromWeatherPredict72h(const QDateTime& time);

	// ��ȡ����ֵ
	QVector<float> GetMaintenanceValue(const QDateTime& startTime, const QDateTime& endTime);

	QString SetFieldsValue(const float& value, const int nFieldIndex);

	// ���泬����Ԥ�����ݵ����ݿ���
	void SavePowerPredictData4h(const QDateTime& insertTime);
	void SavePowerPredictData4h(POWER_PREDICT_DATA_4H& data, const QDateTime& insertTime);

	// ���泬���������е����һ��15�������ݵ����ݿ��� ��power_predict_4h_final_15m��
	void SaveDataPowerPredict4h_Final_15m(POWER_PREDICT_DATA_4H& data, const QDateTime& insertTime);

	// �������Ԥ�����ݵ����ݿ���
	void SavePowerPredictData72h(const QDateTime& insertTime);
	void SavePowerPredictData72h(POWER_PREDICT_DATA_72H& data, const QDateTime& insertTime);

	// ����������������һ������ݵ����ݿ��� ��power_predict_72h_final_15m��
	void SaveDataPowerPredict72h_Final_24h(POWER_PREDICT_DATA_72H& data, const QDateTime& insertTime);

private:

	// ��վ��Ϣ
	std::shared_ptr<CStationInfo> m_pStationInfo;

	// �ܷ�������й�
	QMap<ANALOGUS_NAME, POINT_INFO> m_mapDataInfo;

	// ������쳬����Ԥ�������
	QMap<ANALOGUS_NAME, QVector<float>> m_mapYesterday4h;

	// ����������Ԥ�������
	QMap<ANALOGUS_NAME, QVector<float>> m_mapYesterday72h;	

	// ������Ԥ������Ԥ������
	QMap<Weather_Type, WEATHER_PREDICT_VALUES> m_mapPredictWeather4h;

	// ����Ԥ������Ԥ������
	QMap<Weather_Type, WEATHER_PREDICT_VALUES> m_mapPredictWeather72h;

	// ������Ԥ������
	POWER_PREDICT_DATA_4H m_powerPredictData4h;

	// ����Ԥ������
	POWER_PREDICT_DATA_72H m_powerPredictData72h;

private:

	// װ������
	float m_fCapacity;

	// ��ʷ����ֵ�Ŀ�ʼλ��
	static const int m_nYesterday4h_Value_First_Field = 7;

	// ����Ԥ������ֵ�Ŀ�ʼλ�ã�ftp��
	static const int m_nPredict_Value_First_Field = 3;

	// �Ƿ��ʼ��
	std::atomic<bool> m_isInit;

	// DB�ӿ�
	std::shared_ptr<CDB_API> m_pDB_api;

	// ��վID
	int m_nStation;
};
