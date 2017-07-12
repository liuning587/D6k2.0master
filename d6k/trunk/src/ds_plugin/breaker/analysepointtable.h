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
	//ʵʱӳ���ļ�
	void AnalyseRealFile(const QString &strFileName);
public:
	//��ȡ����ֵ
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &GetAnalogInfo()
	{
		return m_mapAnalogIDDes;
	}

	//��ȡ������Ϣ
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &GetDebugInfo()
	{
		return m_mapDebugIDDes;
	}
	//��ȡ������ֵ
	const std::map<int, std::shared_ptr<CDevInfo> > &GetProtectDevInfo()
	{
		return m_mapProtectDevIDDes;
	}
	//��ȡϵͳ������ֵ
	const std::map<int, std::shared_ptr<CDevInfo> > &GetSystemInfoDevInfo()
	{
		return m_mapSystemDevIDDes;
	}

	//��ȡ������
	const std::map<int, std::shared_ptr<CDIDOInfo> > &GetDIInfo()
	{
		return m_mapDIIDDes;
	}

	//��ȡ������
	const std::map<int, std::shared_ptr<CDIDOInfo> > &GetDOInfo()
	{
		return m_mapDOIDDes;
	}

	//��ȡ�����ź�
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetActionInfo()
	{
		return m_mapActionIDDes;
	}

	//��ȡ�쳣�ź�
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetAbnormalInfo()
	{
		return m_mapAbNormalIDDes;
	}

	//��ȡ��ѹ����Ϣ
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetSoftInfo()
	{
		return m_mapSoftIDDes;
	}

	//��ȡ�����¼�
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetActionEventInfo()
	{
		return m_mapActionEventIDDes;
	}
	//��ȡ�쳣�¼�
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetAbnromalEventInfo()
	{
		return m_mapAbnormalEventIDDes;
	}

	//�����¼�
	const std::map<int, std::shared_ptr<CSOFTSTRAP> > &GetRunEventInfo()
	{
		return m_mapRunEventIDDes;
	}

	int GetDiIndex()
	{
		return m_nDiIndex;
	}

	int GetDoIndex()
	{
		return m_nDoIndex;
	}

	int GetSoftIndex()
	{
		return m_nSoft;
	}

	int GetAction()
	{
		return m_nAction;
	}
	//�쳣�ź�
	int GetAbnormal()
	{
		return m_nAbnormal;
	}

public:

	//��������ֵ
	void AnalyseAnalogItems(const QString &strLineItem);
	//����������Ϣ
	void AnalyseDebugItems(const QString &strLineItem);
	//����������ֵ
	void AnalyseProtectDevItems(const QString &strLineItem);
	//����ϵͳ����
	void AnalyseSystemDevItems(const QString &strLineItem);

	//����������
	void AnalyseDIItems(const QString &strLineItem);
    //����������
	void AnalyseDOItems(const QString &strLineItem);
	//���������ź�
	void AnayseAction(const QString &strLineItem);
	//�����쳣�ź�
	void AnalyseAbnormal(const QString &strLineItem);
	//������ѹ���ź�
	void AnayseSoftDev(const QString &strLineItem);
	//���ö����¼�
	void AnalyseActionEvent(const QString &strLineItem);
	//�����쳣�¼�
	void AnalyseAbnormalEvent(const QString &strLineItem);
	//���������¼�
	void AnalyseRunEvent(const QString &strLineItem);

private:

	//����ֵ
	std::map<int, std::shared_ptr<CAnalogPointInfo> > m_mapAnalogIDDes;
	//������Ϣ
	std::map<int, std::shared_ptr<CAnalogPointInfo> > m_mapDebugIDDes;
	//������ֵ
	std::map<int, std::shared_ptr<CDevInfo> > m_mapProtectDevIDDes;
	//ϵͳ������ֵ
	std::map<int, std::shared_ptr<CDevInfo> > m_mapSystemDevIDDes;
	//������
	std::map<int, std::shared_ptr<CDIDOInfo> > m_mapDIIDDes;
	//������
	std::map<int, std::shared_ptr<CDIDOInfo> > m_mapDOIDDes;
	//�����ź�
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapActionIDDes;
	//�쳣�ź�
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapAbNormalIDDes;
	//��ѹ��
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapSoftIDDes;
	//�����¼�
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapActionEventIDDes;
	//�쳣�¼�
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapAbnormalEventIDDes;
	//�����¼�
	std::map<int, std::shared_ptr<CSOFTSTRAP> > m_mapRunEventIDDes;
	//di index
	int m_nDiIndex;
	//do index
	int m_nDoIndex;
	//action
	int m_nAction;
	//abnormal
	int m_nAbnormal;
	//soft
	int m_nSoft;
};

