#pragma once

#ifndef PLANT_DATA
#define PLANT_DATA

#include <atomic>

#include "tag.h"

class CPlantData
{
public:
	CPlantData();
	~CPlantData();

public:	

	void SetActPowerTagname(const std::string& strActPowerTagame);

	void SetReactPowerTagname(const std::string& strReactPowerTagname);

	// ��ʼ��
	bool Init();

	// ��ȡʵʱ����
	bool GetRealTimeData();

	// ��ȡ���й�ʵʱ����
	bool GetRealDataOfActPower(fp64* value);

	// ��ȡ���޹�ʵʱ����
	bool GetRealDataOfReactPower(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:

	// �Ƿ��ʼ��
	std::atomic<bool> m_isInit;

	//!  ���й�
	std::string m_strActPowerTagname;
	std::shared_ptr<CAinData> m_pActPower;	
	
	//!  ���޹�
	std::string m_strReactPowerTagname;
	std::shared_ptr<CAinData> m_pReactPower;
};

#endif // PLANT_DATA
