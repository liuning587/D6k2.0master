#pragma once

#ifndef WEATHER_FILE_SVC_TASK_H
#define WEATHER_FILE_SVC_TASK_H

#include <vector>

#include "base_module.h"
#include "base_task.h"
#include "dbapi.h"

// 天气预报数据处理任务
class CWeatherFileSvcTask : public CBaseTask
{
public:
	CWeatherFileSvcTask(const unsigned int uTime = 5);
	virtual ~CWeatherFileSvcTask();

protected:
	// 定时1分钟处理反向隔离传过来的文件，并将内容存到数据库中 weather_predict_4h 和 weather_predict_72h
	virtual void DoWork();

private:
	// DB接口
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // WEATHER_FILE_SVC_TASK_H
