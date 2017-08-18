
#include "history_svc_task.h"

#include "power_define.h"

CHistorySvcTask::CHistorySvcTask(const std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo) :
	m_vecStationInfo(vecStationInfo)
{
	m_pTimer = nullptr;

	m_pDB_api = std::make_shared<CDB_API>(ALL_HISTORY_DATA_CONNECT_NAME);
}

CHistorySvcTask::~CHistorySvcTask()
{
	StopTimer();

	if (m_pTimer != nullptr)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
}

void CHistorySvcTask::CreateTimer()
{
	m_pTimer = new QTimer(this);

	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Slot_Handle_Timeout()), Qt::DirectConnection);
}

bool CHistorySvcTask::StartTimer()
{
	Q_ASSERT(m_pTimer);
	if (m_pTimer == nullptr)
	{
		return false;
	}

	if (m_pTimer != nullptr)
	{
		if (!m_pTimer->isActive())
		{
			m_pTimer->start(HISTORY_DATA_SAVE_PERIOD);
		}
	}

	return true;
}

void CHistorySvcTask::StopTimer()
{
	if (m_pTimer != nullptr)
	{
		if (m_pTimer->isActive())
		{
			m_pTimer->stop();
		}
	}
}

void CHistorySvcTask::run()
{
	Q_ASSERT(m_pTimer);
	if (m_pTimer == nullptr)
	{
		CreateTimer();
	}

	StartTimer();

	exec();
}

void CHistorySvcTask::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}

void CHistorySvcTask::Slot_Handle_Timeout(const void* act /*= 0*/)
{
	// 定时从实时库中获取实时数据，并入库

	QDateTime currTime = QDateTime::currentDateTime();

	Insert_All_History_Data(currTime);
}

void CHistorySvcTask::Insert_All_History_Data(const QDateTime& insertTime)
{
	Q_ASSERT(m_pDB_api && m_vecStationInfo.size() > 0);
	if (m_pDB_api == nullptr || m_vecStationInfo.size() <= 0)
	{
		// log
		return;
	}

	if (!m_pDB_api->open())
	{
		// log
		return;
	}

	// yyyy-MM-dd
	QString dateTimeFormat = Set_Date_Format(insertTime);

	QTime current_time = QTime::currentTime();
	int hour = current_time.hour();
	int minute = current_time.minute();

	int nIndex = (hour * 60 + minute) / 5;

	for (auto varStationInfo : m_vecStationInfo)
	{
		Q_ASSERT(varStationInfo);
		if (varStationInfo == nullptr)
		{
			m_pDB_api->close();

			// log
			return;
		}

		for (auto i : varStationInfo->GetStationInfo())
		{
			QString sql = QString("SELECT * FROM %1 WHERE time = '%2' AND station = %3 AND device = %4 AND point = %5").
				arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
				arg(dateTimeFormat).
				arg(i.nStationId).
				arg(i.nDeviceId).
				arg(i.nPointId);

			m_pDB_api->exec(sql);

			// 获取实时数据
			fp64 value;
			CAinData ainData(i.strTagName);
			if (!ainData.GetData(&value))
			{
				continue;
			}

			if (m_pDB_api->next())
			{ // 存在则更新

				int nID = m_pDB_api->getValue(0).toInt();

				sql = QString("UPDATE %1 SET v%2 = '%3', v%4_flag = '%5' WHERE id = %6").
					arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
					arg(nIndex).
					arg(value).
					arg(nIndex).
					arg(1).
					arg(nID);
			}
			else
			{ // 不存在则插入

				sql = QString("INSERT INTO %1 (id, name, time, station, device, point, base, v%2, v%3_flag) VALUES (NULL,'%4','%5','%6','%7','%8','%9','%10')").
					arg(TABLE_NAME_HISTORY_ANALOGUES_DAY).
					arg(nIndex).
					arg(nIndex).
					arg(i.strDesc.c_str()).
					arg(dateTimeFormat).
					arg(i.nStationId).
					arg(i.nDeviceId).
					arg(i.nPointId).
					arg(value).
					arg(1);
			}

			// log
			// 记录sql语句

			// 执行sql语句
			if (m_pDB_api->exec(sql))
			{
				// 执行成功
			}
			else
			{
				// 执行失败
				// 记录错误：m_pDB_api->lastError();
			}
		}

		// 一个场站的数据一提交
		m_pDB_api->commit();
	}
	
	m_pDB_api->close();

	return;
}

QString CHistorySvcTask::Set_Date_Format(const QDateTime &dateTime)
{
	return QString("%1").arg(dateTime.toString("yyyy-MM-dd"));
}
