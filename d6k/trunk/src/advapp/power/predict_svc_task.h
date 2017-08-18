#pragma once

#ifndef PREDICT_SVC_TASK
#define PREDICT_SVC_TASK

#include <vector>
#include <QLibrary>

#include "base_module.h"
#include "station_info.h"
#include "dbapi.h"

// 预测计算任务
class CPredictSvcTask : public CBaseModule
{
public:
	CPredictSvcTask(std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo);
	virtual ~CPredictSvcTask();

	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
	
protected:

	// 线程执行函数
	virtual void MainLoop();

private:

	// 加载预测算法
	bool LoadPredictCalc();	

	// 多场站信息存储
	const std::vector<std::shared_ptr<CStationInfo>>& m_vecStationInfo;

private:
	// 是否退出关键字
	volatile bool m_bQuit;

	std::vector<QLibrary*> m_vecLib;

	// library和数据关联
	QMap<QLibrary*, std::vector<std::shared_ptr<CStationInfo>>> m_mapLibAndData;
};

#endif // PREDICT_SVC_TASK
