#pragma once

#ifndef POWER_PREDICTION_H
#define POWER_PREDICTION_H

#include <memory> // shared_ptr
#include <string>

#include "ext_service.h"
#include "station_info.h"
#include "read_xml.h"
#include "history_svc_task.h"
#include "predict_svc_task.h"
#include "weather_file_svc_task.h"

class CPowerPrediction : public CExtService
{
public:
	CPowerPrediction(int32u nOccNo);

	~CPowerPrediction();

	// 初始化
	bool Init(const char * pszProjectName);

	// 运行（①、加载配置; ②、启动任务）
	bool Run();

	// 
	void Shutdown();

	// 获取多场站信息
	std::vector<std::shared_ptr<CStationInfo>>& GetStationInfo();

private:
	// 加载配置
	bool LoadConfig();	

	

private:
	// 历史数据处理(5分钟存储1次)
	bool HistoryDataHandle();
	
	// 预测处理（逆变器数据，气象站数据，总有功和天气预报数据.并将计算后的值回写到数据库中）
	bool PredictHandle();


	// ftp
	// 解析反向隔离装置传过来的天气预报文件（文件格式：2017072508.txt）
	// 定时1分钟解析指定目录下的文件
	// 根据组态工具设置的2个预测时间间隔来判断解析哪个文件保存到数据库
	// 假如这个文件是2017072508.txt
	bool WeatherFileHandle();

protected:
	// log
	void LogMsg(const char *szLogTxt, int nLevel);

private:

	// 配置文件路径
	std::string m_strPathName;

	// 加载配置
	std::shared_ptr<CReadPowerXml> m_pReadInfo;

	// 多场站信息存储
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

private:
	// 历史数据任务
	std::shared_ptr<CHistorySvcTask> m_pHisSvcTask;

	// 预测计算数据任务
	std::shared_ptr<CPredictSvcTask> m_pPredictCalcTask;

	// 天气预报数据处理任务
	std::shared_ptr<CWeatherFileSvcTask> m_pWeatherFileTask;
};

#endif
