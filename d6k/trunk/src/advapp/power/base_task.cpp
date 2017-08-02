
#include "base_task.h"

CBaseTask::CBaseTask(const unsigned int uTime) : m_uTime(uTime)
{
	m_pTimer = nullptr;
}

CBaseTask::~CBaseTask()
{
	StopTimer();

	if (m_pTimer != nullptr)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
}

void CBaseTask::CreateTimer()
{
	m_pTimer = new QTimer(this);

	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Slot_Handle_Timeout()));
}

bool CBaseTask::StartTimer()
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
			m_pTimer->start(m_uTime);
		}
	}

	return true;
}

void CBaseTask::StopTimer()
{
	if (m_pTimer != nullptr)
	{
		if (m_pTimer->isActive())
		{
			m_pTimer->stop();
		}
	}
}

void CBaseTask::run()
{
	Q_ASSERT(m_pTimer);
	if (m_pTimer == nullptr)
	{
		CreateTimer();
	}

	StartTimer();

	exec();
}

void CBaseTask::Slot_Handle_Timeout(const void* act /*= 0*/)
{

}
