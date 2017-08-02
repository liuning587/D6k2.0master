#pragma once

#ifndef WEATHER_FILE_SVC_TASK_H
#define WEATHER_FILE_SVC_TASK_H

#include <vector>

#include "base_module.h"
#include "base_task.h"
#include "dbapi.h"

// ����Ԥ�����ݴ�������
class CWeatherFileSvcTask : public CBaseTask
{
public:
	CWeatherFileSvcTask(const unsigned int uTime = 5);
	virtual ~CWeatherFileSvcTask();

protected:
	// ��ʱ1���Ӵ�������봫�������ļ����������ݴ浽���ݿ��� weather_predict_4h �� weather_predict_72h
	virtual void DoWork();

private:
	// DB�ӿ�
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // WEATHER_FILE_SVC_TASK_H
