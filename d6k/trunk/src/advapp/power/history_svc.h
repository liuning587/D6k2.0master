#pragma once

#ifndef HIS_SVC_H
#define HIS_SVC_H

#include "base_module.h"

// 处理历史数据
class CHistorySvc : public CBaseModule
{
public:
	CHistorySvc();
	virtual ~CHistorySvc();
public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
protected:
	virtual void MainLoop();

private:
	// 获取实时数据
	// todo:整个历史数据先从实时库获取数据然后入库
	//bool GetRealTimeData();
};

#endif
