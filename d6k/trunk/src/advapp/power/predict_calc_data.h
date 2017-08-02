#pragma once

#ifndef PREDICT_CALC_DATA_H
#define PREDICT_CALC_DATA_H

#include <atomic>

#include "tag.h"

// Ԥ���������
// ���û�������������ȡʵʱֵ���  ��   ����Ԥ���㷨����󣬽�ֵ��д��ʵʱ��
class CPredictCalcData
{
public:
	CPredictCalcData();
	~CPredictCalcData();

public:

	//! ���ó�����Ԥ��tagname
	void SetUltraShortPredict_4_Tagname(const std::string& strUltraShortPredict4Tagname);

	//! ���ö���Ԥ��tagname
	void SetShortPredict_72_Tagname(const std::string& strShortPredict72Tagname);

	// ��ʼ��
	bool Init();

	// ��ȡʵʱ����
	bool GetRealTimeData();

	// ��ȡ������Ԥ��ʵʱ����
	bool GetRealDataOfUltraShortPredict_4(fp64* value);

	// ��ȡ����Ԥ��ʵʱ����
	bool GetRealDataOfShortPredict_72(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:
	// �Ƿ��ʼ��
	std::atomic<bool> m_isInit;

	//! ������Ԥ��tagname
	std::string m_strUltraShortPredict4Tagname;

	//! ����Ԥ��tagname
	std::string m_strShortPredict72Tagname;

	//! ������Ԥ������д��
	std::shared_ptr<CAinData> m_pUltraShortPredict_4;

	//! ����Ԥ������д��
	std::shared_ptr<CAinData> m_pShortPredict_72;

};

#endif
