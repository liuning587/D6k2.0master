#pragma once

#ifndef PREDICT_SVC_TASK
#define PREDICT_SVC_TASK

#include <vector>

#include "base_module.h"
#include "station_info.h"
#include "base_task.h"
#include "dbapi.h"

// 预测计算任务
class CPredictSvcTask : public CBaseTask
{
public:
	CPredictSvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo, const unsigned int uTime = 5);
	virtual ~CPredictSvcTask();

protected:
	virtual void DoWork();

private:
	// 定时15分钟预测一次,并将预测数据保存到数据库中
	int Handle_timeout(const void* act = 0);

private:
	// 获取计算需要的实时数据

	// 加载DB配置(加载数据库中的天气预报数据)用于计算
	bool LoadDBInfo();

	// 加载预测算法
	bool LoadPredictCalc();	

	// 保存短期数据到数据库中
	bool SaveShortDataToDB();

	// 保存短期数据中最后一天的数据到数据库中 （power_predict_72h_final_15m）
	bool SaveLastShortDataToDB();

	// 保存超短期数据到数据库中
	bool SaveUltraShortDataToDB();
	
	// 保存超短期数据中的最后一个15分钟数据到数据库中 （power_predict_4h_final_15m）
	bool SaveLastDayUltraShortDataToDB();

	// 多场站信息存储
	std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

private:
	// 短期预测数据
	// std::vector<>

	// 超短期预测数据
	// std::vector<>

	// DB接口
	std::shared_ptr<CDB_API> m_pDB_api;
};

#endif // PREDICT_SVC_TASK
