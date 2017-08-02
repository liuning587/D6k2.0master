
#include "history_svc_task.h"

#include "power_define.h"

CHistorySvcTask::CHistorySvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo) :
	m_vecStationInfo(vecStationInfo)
{
	m_pTimer = nullptr;
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

	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Slot_Handle_Timeout()));
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

void CHistorySvcTask::Slot_Handle_Timeout(const void* act /*= 0*/)
{
	// 定时从实时库中获取实时数据，并入库
	// todo:
}
