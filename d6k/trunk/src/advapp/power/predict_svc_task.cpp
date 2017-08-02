
#include "predict_svc_task.h"

CPredictSvcTask::CPredictSvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo, const unsigned int uTime/* = 5*/) :
	m_vecStationInfo(vecStationInfo), CBaseTask(uTime)
{

}

CPredictSvcTask::~CPredictSvcTask()
{

}

void CPredictSvcTask::DoWork()
{

}
