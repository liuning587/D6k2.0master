#pragma once

#include "breaker_data.h"
#include <memory>
class CAnalysePointTable
{
public:
	CAnalysePointTable();
	~CAnalysePointTable();
	enum 
	{
		//����ֵ
		ANALOG_VALUE = 1,
		//��������
		DEBUG_DATA,
		//������
		DI_DATA,
		//������
		DO_DATA,
		//�����ź�
		ACTION_SIGNAL,
		//�쳣�ź�
		ABNORMAL_SIGNAL,
		//��ѹ���ź�
		SOFTPLATE_SIGNAL,
		//������ֵ
		PROTECT_DEV,
		//ϵͳ����
		SYSTEM_PARM,
		//�����¼�
		ACTION_EVENT,
		//�쳣�¼�
		ABNORMAL_EVENT,
		//��ѹ���λ�¼�
		SOFTLATE_CHANGE_EVENT,
		//�����¼�
		RUN_EVENT,
	};
public:
	void AnalyseFile(const QString &strFileName);
public:
	//��ȡ����ֵ
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &GetAnalogInfo()
	{
		return m_mapIDDes;
	}

	//��������ֵ
	void AnalyseAnalogItems(const QString &strLineItem);

private:

	//����ֵ
	std::map<int, std::shared_ptr<CAnalogPointInfo> > m_mapIDDes;

};

