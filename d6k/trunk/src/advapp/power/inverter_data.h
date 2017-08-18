#pragma once

#ifndef INVERTER_DATA_H
#define INVERTER_DATA_H

#include <atomic>

#include "tag.h"

// �������״̬���й����޹�����
class CInverterData
{
public:
	CInverterData();
	~CInverterData();

public:
	void SetInverterStatusTagname(const std::string& strInverterTagname);

	void SetInverterActivePowerTagname(const std::string& strInverterActiveTagname);

	void SetInverterReactivePowerTagname(const std::string& strInverterReactiveTagname);
	
	void SetInverterVoltageTagname(const std::string& strInverterVoltageTagname);

	// ��ʼ��
	bool Init();

	// ��ȡʵʱ����
	bool GetRealTimeData();

	// ��ȡ������ֺ�״̬ʵʱ����
	bool GetRealDataOfStatus(int8u* value);

	// ��ȡ������й�ʵʱ����
	bool GetRealDataOfActivePower(fp64* value);	

	// ��ȡ�������ѹʵʱ����
	bool GetRealDataOfVoltage(fp64* value);

	// ��ȡ������޹�ʵʱ����
	bool GetRealDataOfReactivePower(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:

	// �Ƿ��ʼ��
	std::atomic<bool> m_isInit;

	//! �����״̬tagname
	std::string m_strInverterTagname;
	//! ������й�tagname
	std::string m_strInverterActiveTagname;
	//! ������޹�tagname
	std::string m_strInverterReactiveTagname;
	//! �������ѹtagname
	std::string m_strInverterVoltageTagname;

	//! ������ֺ�״̬
	std::shared_ptr<CDinData> m_pInverterStatus;
	//!  ������й�
	std::shared_ptr<CAinData> m_pInverterActivePower;
	//! ������޹�
	std::shared_ptr<CAinData> m_pInverterReactivePower;
	//! �������ѹ
	std::shared_ptr<CAinData> m_pInverterVoltage;

	//! �豸ID
	int m_nDeviceID;
};

#endif // INVERTER_DATA_H
