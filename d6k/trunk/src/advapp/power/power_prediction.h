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

	// ��ʼ��
	bool Init(const char * pszProjectName);

	// ���У��١���������; �ڡ���������
	bool Run();

	// 
	void Shutdown();

	// ��ȡ�ೡվ��Ϣ
	std::vector<std::shared_ptr<CStationInfo>>& GetStationInfo();

private:
	// ��������
	bool LoadConfig();	

	

private:
	// ��ʷ���ݴ���(5���Ӵ洢1��)
	bool HistoryDataHandle();
	
	// Ԥ�⴦����������ݣ�����վ���ݣ����й�������Ԥ������.����������ֵ��д�����ݿ��У�
	bool PredictHandle();


	// ftp
	// �����������װ�ô�����������Ԥ���ļ����ļ���ʽ��2017072508.txt��
	// ��ʱ1���ӽ���ָ��Ŀ¼�µ��ļ�
	// ������̬�������õ�2��Ԥ��ʱ�������жϽ����ĸ��ļ����浽���ݿ�
	// ��������ļ���2017072508.txt
	bool WeatherFileHandle();

protected:
	// log
	void LogMsg(const char *szLogTxt, int nLevel);

private:

	// �����ļ�·��
	std::string m_strPathName;

	// ��������
	std::shared_ptr<CReadPowerXml> m_pReadInfo;

	// �ೡվ��Ϣ�洢
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

private:
	// ��ʷ��������
	std::shared_ptr<CHistorySvcTask> m_pHisSvcTask;

	// Ԥ�������������
	std::shared_ptr<CPredictSvcTask> m_pPredictCalcTask;

	// ����Ԥ�����ݴ�������
	std::shared_ptr<CWeatherFileSvcTask> m_pWeatherFileTask;
};

#endif
