#pragma once

#ifndef HIS_SVC_H
#define HIS_SVC_H

#include "base_module.h"

// ������ʷ����
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
	// ��ȡʵʱ����
	// todo:������ʷ�����ȴ�ʵʱ���ȡ����Ȼ�����
	//bool GetRealTimeData();
};

#endif
