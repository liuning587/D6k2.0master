#pragma once

#ifndef PREDICT_CALC_DATA_H
#define PREDICT_CALC_DATA_H

#include <atomic>

#include "tag.h"

// 预测计算数据
// 绑定用户变量：用来获取实时值入库  和   经过预测算法计算后，将值回写到实时库
class CPredictCalcData
{
public:
	CPredictCalcData();
	~CPredictCalcData();

public:

	//! 设置超短期预测tagname
	void SetUltraShortPredict_4_Tagname(const std::string& strUltraShortPredict4Tagname);

	//! 设置短期预测tagname
	void SetShortPredict_72_Tagname(const std::string& strShortPredict72Tagname);

	// 初始化
	bool Init();

	// 获取实时数据
	bool GetRealTimeData();

	// 获取超短期预测实时数据
	bool GetRealDataOfUltraShortPredict_4(fp64* value);

	// 获取短期预测实时数据
	bool GetRealDataOfShortPredict_72(fp64* value);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);

private:
	// 是否初始化
	std::atomic<bool> m_isInit;

	//! 超短期预测tagname
	std::string m_strUltraShortPredict4Tagname;

	//! 短期预测tagname
	std::string m_strShortPredict72Tagname;

	//! 超短期预测计算回写绑定
	std::shared_ptr<CAinData> m_pUltraShortPredict_4;

	//! 短期预测计算回写绑定
	std::shared_ptr<CAinData> m_pShortPredict_72;

};

#endif
