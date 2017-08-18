#include "proportion_algorithm.h"

#include "power_define.h"

CProportionAlgorithm::CProportionAlgorithm()
{
	m_isInit = false;

	m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
}

CProportionAlgorithm::~CProportionAlgorithm()
{
	m_isInit = false;
}

bool CProportionAlgorithm::Init(const std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pStationInfo);
	if (pStationInfo == nullptr)
	{
		return false;
	}

	if (m_isInit)
	{
		return true;
	}

	m_pStationInfo = pStationInfo;

	// 初始化装机容量
	Q_ASSERT(m_pStationInfo->GetPlant());
	if (m_pStationInfo->GetPlant() == nullptr)
	{
		// log
		return false;
	}

	m_fCapacity = m_pStationInfo->GetPlant()->m_fCapacity;

	// 从组态配置信息中获取总辐射和总有功的  场站ID、设备ID和点ID。(用来获取昨天数据库中的值)
	if (!GetAnalogusInfo())
	{
		return false;
	}

	Q_ASSERT(m_mapDataInfo.size() > 0);
	if (m_mapDataInfo.size() <= 0)
	{
		return false;
	}

	// 昨天数据库中短期和超短期数据初始化
	// 总辐射和总有功对应的16个点(超短期)和288点的数据（短期）
	QVector<float> vecPredict4h(POWER_PREDICT_4H_POINTS_COUNT);
	QVector<float> vecPredict72h(POWER_PREDICT_72H_POINTS_COUNT);

	m_mapYesterday4h.insert(PLANT_ACTIVE_POWER, vecPredict4h);
	m_mapYesterday72h.insert(TOTAL_RADIATION, vecPredict72h);
	
	// 天气预报数据初始化
	WEATHER_PREDICT_VALUES vecWeatherPredictValues4h(WEATHER_PREDICT_4H_POINTS_COUNT);
	WEATHER_PREDICT_VALUES vecWeatherPredictValues72h(WEATHER_PREDICT_72H_POINTS_COUNT);

	m_mapPredictWeather4h.insert(Weather_Total_Radiation, vecWeatherPredictValues4h);
	m_mapPredictWeather72h.insert(Weather_Total_Radiation, vecWeatherPredictValues72h);

	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		// log
		return false;
	}

	if (!m_pDB_api->open())
	{
		// log
		return false;
	}

	m_nStation = m_pStationInfo->GetStationID();
	Q_ASSERT(m_nStation > 0);
	if (m_nStation <= 0)
	{
		return false;
	}

	m_isInit = true;

	return true;
}

void CProportionAlgorithm::PowerPredict72h(const QDateTime& currTime)
{
	if (!m_isInit)
	{
		// log
		return;
	}

	QDateTime predictStartTime;
	QDateTime predictEndTime;

	// 存放检修值
	QVector<float> vecMaintenanceValues;

	// 是否存在替代值
	bool isFind72h = false;

	QDateTime time = QDateTime::fromString(currTime.toString("yyyy-MM-dd 00:00:00"), "yyyy-MM-dd hh:mm:ss");

	predictStartTime = time.addDays(1);
	predictEndTime = time.addDays(4);

	// 获取前3天数据
	if (GetYesterdayValue72h(time))
	{
		if (GetDataFromWeatherPredict72h(currTime))
		{
			vecMaintenanceValues = GetMaintenanceValue(predictStartTime, predictEndTime);

			for (int i = 0; i < POWER_PREDICT_72H_POINTS_COUNT; i++)
			{
				if (m_mapYesterday72h[TOTAL_RADIATION][i] != 0)
				{
					m_powerPredictData72h.data[i].val = m_mapPredictWeather72h[Weather_Total_Radiation][i].val / m_mapYesterday72h[TOTAL_RADIATION][i] * m_mapYesterday72h[PLANT_ACTIVE_POWER][i];
					m_powerPredictData72h.data[i].val = m_powerPredictData72h.data[i].val - vecMaintenanceValues[i];

					if (m_powerPredictData72h.data[i].val < 0)
					{
						m_powerPredictData72h.data[i].val = 0;
					}

					if (m_powerPredictData72h.data[i].val > m_fCapacity)
					{
						m_powerPredictData72h.data[i].val = m_fCapacity;
					}
				}
				else
				{
					for (int j = 0; j < POWER_PREDICT_72H_POINTS_COUNT; j++)
					{
						if (m_mapYesterday72h[TOTAL_RADIATION][j] != 0)
						{
							m_powerPredictData72h.data[i].val = m_mapPredictWeather72h[Weather_Total_Radiation][i].val / m_mapYesterday72h[TOTAL_RADIATION][j] * m_mapYesterday72h[PLANT_ACTIVE_POWER][j];
							m_powerPredictData72h.data[i].val = m_powerPredictData72h.data[i].val - vecMaintenanceValues[i];

							if (m_powerPredictData72h.data[i].val < 0)
							{
								m_powerPredictData72h.data[i].val = 0;
							}

							if (m_powerPredictData72h.data[i].val > m_fCapacity)
							{
								m_powerPredictData72h.data[i].val = m_fCapacity;
							}

							isFind72h = true;
							break;
						}
					}
					if (!isFind72h)
					{
						m_powerPredictData72h.data[i].val = 0;
					}
				}

			}

			// 将预测值插入数据库
			SavePowerPredictData72h(m_powerPredictData72h, currTime);

			// 保存最后一天的值
			SaveDataPowerPredict72h_Final_24h(m_powerPredictData72h, currTime);
		}
		else
		{
			// log
			// 获取天气预报数据失败
		}
	}
	else
	{
		// log
		// 获取前3天数据失败
	}

	return;
}

void CProportionAlgorithm::PowerPredict4h(const QDateTime& currTime)
{
	if (!m_isInit)
	{
		// log
		return;
	}

	// 超短期预测开始时间
	QDateTime predictStartTime;
	// 超短期预测结束时间
	QDateTime predictEndTime;

	// 是否存在替代值
	bool isFind4h = false;
	// 存放检修值
	QVector<float> vecMaintenanceValues;

	// 从预测下一个15分钟开始
	predictStartTime = currTime.addSecs(15 * 60);
	int hour = predictStartTime.time().hour();
	int minu = (predictStartTime.time().minute() / 15) * 15; // 15分钟一个间隔

	// 日期
	QString strTime = QString("%1 %2:%3:00").arg(predictStartTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));

	// 超短期预测开始时间
	predictStartTime = QDateTime::fromString(strTime, "yyyy-MM-dd hh:mm:ss");

	// 超短期预测结束时间
	predictEndTime = predictStartTime.addSecs(60 * 60 * 4 - 15 * 60);

	// 不在预测时间范围数据为0
	if ((currTime.time().hour() <= POWER_PREDICT::POWER_PREDICT_MORNING_HOUR) || (currTime.time().hour() >= POWER_PREDICT::POWER_PREDICT_AFTERNOON_HOUR))
	{
		for (int i = 0; i < POWER_PREDICT_4H_POINTS_COUNT; i++)
		{
			m_powerPredictData4h.data[i].val = 0;
		}

		// 将预测值插入数据库
		SavePowerPredictData4h(m_powerPredictData4h, currTime);

		// 保存最后一个15分钟的值
		SaveDataPowerPredict4h_Final_15m(m_powerPredictData4h,currTime);

		// log
		return;
	}

	// 昨天这个时间下一个15分钟的时间
	QDateTime time = currTime.addDays(-1).addSecs(15 * 60);

	// 获取昨天的值
	if (GetYesterdayValue4h(time))
	{
		// 获取天气预报的值
		if (GetDataFromWeatherPredict4h(predictStartTime))
		{
			vecMaintenanceValues = GetMaintenanceValue(predictStartTime, predictEndTime);

			for (int i = 0; i < POWER_PREDICT_4H_POINTS_COUNT; i++)
			{
				if (m_mapYesterday4h[TOTAL_RADIATION][i] != 0)
				{ // 昨天有值的情况
					m_powerPredictData4h.data[i].val = m_mapPredictWeather4h[Weather_Total_Radiation][i].val / m_mapYesterday4h[TOTAL_RADIATION][i] * m_mapYesterday4h[PLANT_ACTIVE_POWER][i];
					m_powerPredictData4h.data[i].val = m_powerPredictData4h.data[i].val - vecMaintenanceValues[i];

					if (m_powerPredictData4h.data[i].val < 0)
					{
						m_powerPredictData4h.data[i].val = 0;
					}

					if (m_powerPredictData4h.data[i].val > m_fCapacity)
					{
						m_powerPredictData4h.data[i].val = m_fCapacity;
					}
				}
				else
				{ // 昨天没有值的情况,找一个有值的参与计算
					for (int j = 0; j < POWER_PREDICT_4H_POINTS_COUNT; j++)
					{
						if (m_mapYesterday4h[TOTAL_RADIATION][j] != 0)
						{
							m_powerPredictData4h.data[i].val = m_mapPredictWeather4h[Weather_Total_Radiation][i].val / m_mapYesterday4h[TOTAL_RADIATION][j] * m_mapYesterday4h[PLANT_ACTIVE_POWER][j];
							m_powerPredictData4h.data[i].val = m_powerPredictData4h.data[i].val - vecMaintenanceValues[i];

							if (m_powerPredictData4h.data[i].val < 0)
							{
								m_powerPredictData4h.data[i].val = 0;
							}

							if (m_powerPredictData4h.data[i].val > m_fCapacity)
							{
								m_powerPredictData4h.data[i].val = m_fCapacity;
							}

							isFind4h = true;
							break;
						}
					}
					if (!isFind4h)
					{
						m_powerPredictData4h.data[i].val = 0;
					}
				}
			}

			// 将预测值插入数据库
			SavePowerPredictData4h(m_powerPredictData4h, currTime);

			// 保存最后一个15分钟的值
			SaveDataPowerPredict4h_Final_15m(m_powerPredictData4h, currTime);
		}
		else
		{
			// log 获取天气数据失败
		}
	}
	else
	{
		// log 获取昨天数据失败
	}

	return;
}

bool CProportionAlgorithm::GetAnalogusInfo()
{
	// 获取多场站总有功和总辐射数据信息
	Q_ASSERT(m_pStationInfo);
	if (m_pStationInfo == nullptr)
	{
		return false;
	}

	const int nStation = m_pStationInfo->GetStationID();
	POINT_INFO pointInfo;
	pointInfo.nStationID = nStation;
	pointInfo.nDeviceID = 0;
	pointInfo.nDataType = DATA_TYPE_PLANT_ACTIVE_POWER; // 总有功

	m_mapDataInfo.insert(PLANT_ACTIVE_POWER, pointInfo);

	pointInfo.nStationID = nStation;
	pointInfo.nDeviceID = 0;
	pointInfo.nDataType = DATA_TYPE_TOTAL_RADIATION; // 总辐射

	m_mapDataInfo.insert(TOTAL_RADIATION, pointInfo);

	return true;
}

bool CProportionAlgorithm::GetYesterdayValue4h(const QDateTime& time)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			return false;
		}
	}

	m_mapYesterday4h.clear();

	bool isOk = true;
	QString sql = "";

	// 15分钟一个间隔的位置
	int pos = ((time.time().hour() * 60 + time.time().minute()) / 15) * 15;

	// 历史数据5分钟一个点，一个点数据库中2列。计算出实际偏移位置。
	int tick = (pos / 5) * 2;

	foreach(const QString &key, m_mapDataInfo.keys())
	{
		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND data_type = %4 AND time = '%5' ").
			arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
			arg(m_mapDataInfo[key].nStationID).
			arg(m_mapDataInfo[key].nDeviceID).
			arg(m_mapDataInfo[key].nDataType).
			arg(SetDateFormat(time));

		m_pDB_api->exec(sql);		

		if (m_pDB_api->next())
		{
			for (int i = 0; i < POWER_PREDICT_4H_POINTS_COUNT; i++)
			{
				m_mapYesterday4h[key][i] = m_pDB_api->getValue(m_nYesterday4h_Value_First_Field + tick + i * 6).toFloat();
			}
		}
		else
		{
			isOk = false;
		}
	}

	return isOk;
}

bool CProportionAlgorithm::GetYesterdayValue72h(const QDateTime& time)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			return false;
		}
	}

	m_mapYesterday72h.clear();

	bool isOk = true;
	QString sql = "";

	int nIndex = 0;
	foreach(const QString &key, m_mapDataInfo.keys())
	{
		nIndex = 0;

		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND data_type = %4 AND time = '%5' ").
			arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
			arg(m_mapDataInfo[key].nStationID).
			arg(m_mapDataInfo[key].nDeviceID).
			arg(m_mapDataInfo[key].nDataType).
			arg(SetDateFormat(time.addDays(-1)));

		m_pDB_api->exec(sql);		

		if (m_pDB_api->next())
		{
			for (int i = 0; i < POWER_PREDICT_72H_POINTS_COUNT; i++)
			{
				m_mapYesterday72h[key][nIndex++] = m_pDB_api->getValue(m_nYesterday4h_Value_First_Field + i * 6).toFloat();
			}
		}
		else
		{
			isOk = false;
		}

		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND data_type = %4 AND time = '%5' ").
			arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
			arg(m_mapDataInfo[key].nStationID).
			arg(m_mapDataInfo[key].nDeviceID).
			arg(m_mapDataInfo[key].nDataType).
			arg(SetDateFormat(time.addDays(-2)));

		m_pDB_api->exec(sql);

		if (m_pDB_api->next())
		{
			for (int i = 0; i < POWER_PREDICT_72H_POINTS_COUNT; i++)
			{
				m_mapYesterday72h[key][nIndex++] = m_pDB_api->getValue(m_nYesterday4h_Value_First_Field + i * 6).toFloat();
			}
		}
		else
		{
			isOk = false;
		}

		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND data_type = %4 AND time = '%5' ").
			arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
			arg(m_mapDataInfo[key].nStationID).
			arg(m_mapDataInfo[key].nDeviceID).
			arg(m_mapDataInfo[key].nDataType).
			arg(SetDateFormat(time.addDays(-3)));

		m_pDB_api->exec(sql);

		if (m_pDB_api->next())
		{
			for (int i = 0; i < POWER_PREDICT_72H_POINTS_COUNT; i++)
			{
				m_mapYesterday72h[key][nIndex++] = m_pDB_api->getValue(m_nYesterday4h_Value_First_Field + i * 6).toFloat();
			}
		}
		else
		{
			isOk = false;
		}
	}

	return isOk;
}

bool CProportionAlgorithm::GetDataFromWeatherPredict4h(const QDateTime& time)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			return false;
		}
	}

	QString sql = "";
	QDateTime tmpTime;

	for (auto key : m_mapPredictWeather4h.keys())
	{
		for (int i = 0; i < WEATHER_PREDICT_4H_POINTS_COUNT; i++)
		{
			tmpTime = time.addSecs(SECONDS_IN_15_MINITES * i);
			sql = QString("SELECT * FROM %1 WHERE station_id='%2' AND weather_type = %3 AND predict_time = '%4' ").
				arg(TABLE_NAME_WEATHER_PREDICT_4H_Final_15M).
				arg(m_nStation).
				arg(key).
				arg(SetTimeFormat(tmpTime));

			m_pDB_api->exec(sql);
			if (m_pDB_api->next())
			{
				m_mapPredictWeather4h[key][i].val = m_pDB_api->getValue(m_nPredict_Value_First_Field).toFloat();
				m_mapPredictWeather4h[key][i].flag = WEATHER_POINT_EXIST;
			}
			else
			{
				m_mapPredictWeather4h[key][i].val = 0;
				m_mapPredictWeather4h[key][i].flag = WEATHER_POINT_LOST;
			}
		}
	}

	return true;
}

bool CProportionAlgorithm::GetDataFromWeatherPredict72h(const QDateTime& time)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			return false;
		}
	}

	QString sql = "";
	QDateTime tmpTime;

	int nIndex = 0;

	for (auto key : m_mapPredictWeather72h.keys())
	{
		for (int i = 0; i < 3; i++)
		{
			tmpTime = time.addDays(i);
			sql = QString("SELECT * FROM %1 WHERE station_id='%2' AND weather_type = %3 AND predict_time = '%4' ORDER BY predict_time DESC ").
				arg(TABLE_NAME_WEATHER_PREDICT_72H_FINAL_24H).
				arg(m_nStation).
				arg(key).
				arg(SetTimeFormat(tmpTime));

			m_pDB_api->exec(sql);
			if (m_pDB_api->next())
			{
				for (int i = 0; i < WEATHER_PREDICT_24H_POINTS_COUNT; i++)
				{
					m_mapPredictWeather72h[key][nIndex].val = m_pDB_api->getValue(m_nPredict_Value_First_Field + i*2).toFloat();
					m_mapPredictWeather72h[key][nIndex].flag = WEATHER_POINT_EXIST;

					nIndex++;
				}
			}
			else
			{
				for (int i = 0; i < WEATHER_PREDICT_24H_POINTS_COUNT; i++)
				{
					m_mapPredictWeather72h[key][nIndex].val = 0;
					m_mapPredictWeather72h[key][nIndex].flag = WEATHER_POINT_LOST;

					nIndex++;
				}
			}
		}
	}

	return true;
}

QVector<float> CProportionAlgorithm::GetMaintenanceValue(const QDateTime& startTime, const QDateTime& endTime)
{
	QVector<float> vecMaintenanceValue;
	QString sql;

	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			return vecMaintenanceValue;
		}
	}

	QDateTime tmpTime = startTime;

	while (tmpTime <= endTime)
	{
		sql = QString("select * from %1 where station_id='%2' AND starttime <= '%3' and endtime >= '%4'").
			arg(TABLE_NAME_MAINTENANCE_CAPACITY).
			arg(m_nStation).
			arg(tmpTime.toString("yyyy-MM-dd hh:mm:ss")).
			arg(tmpTime.toString("yyyy-MM-dd hh:mm:ss"));

		m_pDB_api->exec(sql);
		if (m_pDB_api->next())
		{
			float value = m_pDB_api->getSqlQuery().value("value").toFloat();

			vecMaintenanceValue.append(value);
		}
		else
		{
			vecMaintenanceValue.append(0);
		}

		tmpTime = tmpTime.addSecs(60 * 15);
	}

	return vecMaintenanceValue;
}

QString CProportionAlgorithm::SetFieldsValue(const float& value, const int nFieldIndex)
{
	return QString(" v%1 = %2 ,").arg(nFieldIndex).arg(value);
}

void CProportionAlgorithm::SavePowerPredictData4h(const QDateTime& insertTime)
{
	SavePowerPredictData4h(m_powerPredictData4h, insertTime);
}

void CProportionAlgorithm::SavePowerPredictData4h(POWER_PREDICT_DATA_4H& data, const QDateTime& insertTime)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			// log
			return;
		}
	}

	QString dateTimeFormat = SetTimeFormat(insertTime);
	QString dateTimeFormatActual = SetTimeFormatActual(insertTime);

	QString sql = QString("SELECT * FROM %1 WHERE station_id='%2' AND predict_time = '%3' ").arg(TABLE_NAME_POWER_PREDICT_4H).arg(m_nStation).arg(dateTimeFormat);
	m_pDB_api->exec(sql);

	if (m_pDB_api->next())
	{
		m_pDB_api->getSqlQuery().seek(0);

		int id = m_pDB_api->getSqlQuery().value(0).toInt();

		sql = QString("UPDATE %1 SET predict_time = '%2', insert_time = '%3', ").arg(TABLE_NAME_POWER_PREDICT_4H).arg(dateTimeFormat).arg(dateTimeFormatActual);
		for (int i = 1; i <= POWER_PREDICT_4H_POINTS_COUNT; i++)
		{
			sql.append(QString("%1").arg(SetFieldsValue(data.data[i - 1].val, i)));
		}

		sql.chop(1);
		sql += QString("WHERE id = %1").arg(id);
	}
	else
	{
		sql = QString("INSERT INTO %1 VALUES (NULL,'%2','%3','%4',").arg(TABLE_NAME_POWER_PREDICT_4H).arg(m_nStation).arg(dateTimeFormat).arg(dateTimeFormatActual);
		for (int i = 1; i <= POWER_PREDICT_4H_POINTS_COUNT; i++)
		{
			sql.append(QString("%1,").arg(data.data[i - 1].val));
			sql.append(QString("%1,").arg(data.data[i - 1].flag));
		}

		sql.chop(1);
		sql.append(")");
	}

	// log
	// 打印sql数据库语句

	if (!m_pDB_api->exec(sql))
	{
		// log 输出error
		m_pDB_api->lastError();
	}
	else
	{
		// log 输出成功
	}

	m_pDB_api->commit();

	return;
}

void CProportionAlgorithm::SaveDataPowerPredict4h_Final_15m(POWER_PREDICT_DATA_4H& data, const QDateTime& insertTime)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			// log
			return;
		}
	}

	QDateTime tmpDateTime;
	QString dateTimeFormat;

	for (int j = 0; j < WEATHER_PREDICT_4H_POINTS_COUNT; j++)
	{
		tmpDateTime = insertTime.addSecs(15 * 60 * j);
		dateTimeFormat = SetTimeFormat(tmpDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE station_id = '%2' AND predict_time = '%3'").arg(TABLE_NAME_POWER_PREDICT_4H_FINAL_15M).arg(m_nStation).arg(dateTimeFormat);
		m_pDB_api->exec(sql);

		if (m_pDB_api->next())
		{
			m_pDB_api->getSqlQuery().seek(0);
			int id = m_pDB_api->getSqlQuery().value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', ").arg(TABLE_NAME_POWER_PREDICT_4H_FINAL_15M).arg(dateTimeFormat);

			sql.append(QString("%1").arg(SetFieldsValue(data.data[j].val, 1)));

			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		}
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2','%3',").arg(TABLE_NAME_POWER_PREDICT_4H_FINAL_15M).arg(m_nStation).arg(dateTimeFormat);
			sql.append(QString("%1,").arg(data.data[j].val));
			sql.append(QString("%1,").arg(data.data[j].flag));

			sql.chop(1);
			sql.append(")");
		}

		// log
		// 打印sql数据库语句

		if (!m_pDB_api->exec(sql))
		{
			// log 输出error
			m_pDB_api->lastError();
		}
		else
		{
			// log 输出成功
		}
	}

	m_pDB_api->commit();

	return;
}

void CProportionAlgorithm::SavePowerPredictData72h(const QDateTime& insertTime)
{
	SavePowerPredictData72h(m_powerPredictData72h, insertTime);
}

void CProportionAlgorithm::SavePowerPredictData72h(POWER_PREDICT_DATA_72H& data, const QDateTime& insertTime)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			// log
			return;
		}
	}

	QString dateTimeFormat = SetTimeFormat(insertTime);
	QString dateTimeFormatActual = SetTimeFormatActual(insertTime);

	QString sql = QString("SELECT * FROM %1 WHERE station_id='%2' AND predict_time = '%3' ").arg(TABLE_NAME_POWER_PREDICT_72H).arg(m_nStation).arg(dateTimeFormat);
	m_pDB_api->exec(sql);

	if (m_pDB_api->next())
	{
		m_pDB_api->getSqlQuery().seek(0);
		int id = m_pDB_api->getSqlQuery().value(0).toInt();

		sql = QString("UPDATE %1 SET predict_time = '%2', insert_time = '%3', ").arg(TABLE_NAME_POWER_PREDICT_72H).arg(dateTimeFormat).arg(dateTimeFormatActual);
		for (int i = 1; i <= POWER_PREDICT_72H_POINTS_COUNT; i++)
		{
			sql.append(QString("%1").arg(SetFieldsValue(data.data[i - 1].val, i)));
		}

		sql.chop(1);
		sql += QString("WHERE id = %1").arg(id);
	}
	else
	{
		sql = QString("INSERT INTO %1 VALUES (NULL,'%2','%3','%4',").arg(TABLE_NAME_POWER_PREDICT_72H).arg(m_nStation).arg(dateTimeFormat).arg(dateTimeFormatActual);
		for (int i = 1; i <= POWER_PREDICT_72H_POINTS_COUNT; i++)
		{
			sql.append(QString("%1,").arg(data.data[i - 1].val));
			sql.append(QString("%1,").arg(data.data[i - 1].flag));
		}

		sql.chop(1);
		sql.append(")");
	}

	// log
	// 打印sql数据库语句

	if (!m_pDB_api->exec(sql))
	{
		// log 输出error
		m_pDB_api->lastError();
	}
	else
	{
		// log 输出成功
	}

	m_pDB_api->commit();

	return;
}

void CProportionAlgorithm::SaveDataPowerPredict72h_Final_24h(POWER_PREDICT_DATA_72H& data, const QDateTime& insertTime)
{
	Q_ASSERT(m_pDB_api);
	if (m_pDB_api == nullptr)
	{
		m_pDB_api = std::make_shared<CDB_API>(PREDICT_DATA_CONNECT_NAME);
	}

	if (!m_pDB_api->IsOpen())
	{
		if (!m_pDB_api->open())
		{
			// log
			return;
		}
	}

	QDateTime tmpDateTime;
	QString dateTimeFormat;

	for (int day = 1; day <= 3; day++)
	{
		tmpDateTime = insertTime.addDays(day);
		dateTimeFormat = SetDateFormat(tmpDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE station_id AND predict_time = '%2'").arg(TABLE_NAME_POWER_PREDICT_72H_FINAL_24H).arg(m_nStation).arg(dateTimeFormat);
		m_pDB_api->exec(sql);

		if (m_pDB_api->next())
		{
			m_pDB_api->getSqlQuery().seek(0);
			int id = m_pDB_api->getSqlQuery().value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', ").arg(TABLE_NAME_POWER_PREDICT_72H_FINAL_24H).arg(dateTimeFormat);
			for (int index = 1; index <= POWER_PREDICT_72H_POINTS_COUNT; index++)
			{
				sql.append(QString("%1").arg(SetFieldsValue(data.data[(day - 1)*POWER_PREDICT_72H_POINTS_COUNT + index - 1].val, index)));
			}

			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		}
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2','%3',").arg(TABLE_NAME_POWER_PREDICT_72H_FINAL_24H).arg(m_nStation).arg(dateTimeFormat);
			for (int index = 1; index <= POWER_PREDICT_72H_POINTS_COUNT; index++)
			{
				sql.append(QString("%1,").arg(data.data[(day - 1)*POWER_PREDICT_72H_POINTS_COUNT + index - 1].val));
				sql.append(QString("%1,").arg(data.data[(day - 1)*POWER_PREDICT_72H_POINTS_COUNT + index - 1].flag));
			}

			sql.chop(1);
			sql.append(")");
		}

		// log
		// 打印sql数据库语句

		if (!m_pDB_api->exec(sql))
		{
			// log 输出error
			m_pDB_api->lastError();
		}
		else
		{
			// log 输出成功
		}		
	}

	m_pDB_api->commit();

	return;
}
