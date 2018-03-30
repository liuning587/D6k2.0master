
#ifndef _101_RECVER_H
#define _101_RECVER_H

#include "define_101.h"
#include "101cache.h"
#include <QObject>



class  CCommPlugin;  
class  C101Impl;

class C101Recver : public QObject
{
	Q_OBJECT
public:
	C101Recver(QObject * parent = 0);
	virtual ~C101Recver();

public:
	bool OnReceive(char* pBuff, int nLen);
public:
	bool AnalyseAvdu(char* pBuff, int nLen);
public:
	bool AnalyseE5(char* pBuff, int nLen);//���ֽ�
	bool AnalyseF(char* pBuff, int nLen);//�̶�֡
	bool AnalyseV(char* pBuff, int nLen);//�ɱ�֡
	bool AnalyseAsdu(char* pBuff, int nLen);//ASDU
public:
	bool OnRecvFfromMonitor(char* pBuff, int nLen);//�̶�֡������װ��
public:
	void LogMsg(const char* ,int nLevel);
public:
	bool LogE(int nType, char* pBuff, int nLength);
	bool LogF(int nType, char* pBuff, int nLength);
	bool LogV(int nType, char* pBuff, int nLength);

private:
	QString GetDownwardFunCodeDesc(int nFuncode);

private:
	bool OnRecvGeneralCallConfirm(char* pBuff, int nLen);//ASDU100
	bool OnRecvSyncTimeConfirm(char* pBuff, int nLen);//ASDU103
private:
	bool OnRecvAnalogNormalized(char* pBuff, int nLen);//��һ������ֵ ASDU9
	bool OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen);//����ʱ��Ĺ�һ������ֵ ASDU10
	bool OnRecvAnalogScaled(char* pBuff, int nLen);//��Ȼ�����ֵ ASDU11
	bool OnRecvAnalogScaledShortTime(char* pBuff, int nLen);//����ʱ��ı�Ȼ�����ֵ ASDU12
	bool OnRecvAnalogShortFloat(char* pBuff, int nLen);//�̸���������ֵ ASDU13
	bool OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen);//����ʱ��Ķ̸���������ֵ ASDU14
	bool OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen);//����Ʒ�������ʵĹ�һ������ֵ ASDU21
	bool OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen);//����ʱ��Ĺ�һ������ֵ ASDU34
	bool OnRecvAnalogScaledLongTime(char* pBuff, int nLen);//����ʱ��ı�Ȼ�����ֵ ASDU35
	bool OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen);//����ʱ��Ķ̸���������ֵ ASDU36
private:
	void OnRecvBinarySinglePointTime(char* pBuff, int nLength);//ASDU30
	void OnRecvBinaryDoublePointTime(char* pBuff, int nLength);//ASDU31
	void OnRecvBinarySinglePointShortTime(char* pBuff, int nLength);//ASDU2
	void OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength);//ASDU4
private:
	bool OnRecvBinarySinglePoint(char* pBuff, int nLen);//���������� ASDU1
	bool OnRecvBinaryDoublePoint(char* pBuff, int nLen);//������˫�� ASDU3
	bool OnRecvBinaryGroupPoint(char* pBuff, int nLen);//����λ����ĳ��鵥�� ASDU20
private:
	void OnRecvKwh(char* pBuff, int nLength);//�ۼ���ASDU15
	void OnRecvKwhShortTime(char* pBuff, int nLength);//����ʱ����ۼ���ASDU16
	void OnRecvKwhLongTime(char* pBuff, int nLength);//����ʱ����ۼ���ASDU37
	void OnRecvFaultMsg(char* pBuff,int nLength);
private://ң��
	void OnRecvSetAnalogNormalizeAck(char* pBuff, int nLength); //ASDU48
	void OnRecvSetAnalogScaleAck(char* pBuff, int nLength);//ASDU49
	void OnRecvSetAnalogShortFloatAck(char* pBuff, int nLength);//ASDU50
	void OnRecvSetBinarySPAck(char* pBuff, int nLength);//ASDU45
	void OnRecvSetBinaryDPAck(char* pBuff, int nLength);//ASDU46

	//�ļ�������
	void OnRecvFileAnalyseInfo(char *pBuff, int nLength);
	//Ŀ¼�ٻ�ȷ��
	void OnRecvCatalogRespond(char *pBuff, int nLength);
	//���ļ�����ȷ��
	void OnRecvFileAction(char *pBuff, int nLength);

	//���ļ����ݴ���
	void OnRecvFileData(char *pBuff, int nLength);
	//�ļ�����ȷ��
	void OnRecvFileConform(char *pBuff, int nLength);

	//д�ļ�����ȷ��
	void OnRecvWriteConform(char *pBuff, int nLength);
	//��������ȷ��
	void OnRecvUpdateActionAck(char *pBuff, int nLength);
	//��ȡ����
	void OnRecvReadIecData(char *pBuff, int nLength);

	//�ļ�Ŀ¼�ٻ���Ӧ
	void OnRecvCatalogRepond(char *pBuff, int nLength);
	//���ļ�������Ӧ
	void OnRecvReadActionResond(char *pBuff, int nLenght);
	//�������ļ�����
	void OnRecvReadFileRespond(char *pBuff, int nLength);
	//���ļ�����ȷ��
	void OnRecvReadConform(char *pBuff, int nLength);
	//д�ļ�����
	void OnRecvWriteAction(char *pBuff, int nLength);
private:
	//�л���ֵ��
	void OnRecvSwitchFixArea(char *pBuff, int nLength);
	//��ȡ��ǰ��ֵ����
	void OnrecvReadCurrentFixArea(char *pBuff, int nLength);
	//������ȡ
	void OnRecvDevReadRequestAck(char* pBuff, int nLength);
	//����д��
	void OnRecvDevWriteRequestAck(char *pBuff, int nLength);
public:
	void ClearFileInfos()
	{
	//	m_arrCatalogRespond.clear();
	//	m_lstFiles.clear();
	}
	void SetFileInfo(const FILE_ATTR_INFO &fileInfo)
	{
		m_fileAttrInfo = fileInfo;
	}
Q_SIGNALS:
	//��������ң������
	void Signal_OnePointRemote(int iDeviceID, int nPointID, int nValue);
	//���������ң������
	void Signal_onePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//���鵥��ң��  ����
	void Signal_BinaryGroupPoint(int iDeviceID, int nPointID, int nValue);
	//���鵥��ң��  ������
	void Signal_BinaryGroupDisPoint(int iDeviceID, int nPointID, int nValue);

	//˫������ң������
	void Signal_DoublePointRemote(int iDeviceID, int nPointID, int nValue);
	//˫�������ң������
	void Signal_DoublePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//��3���ֽڶ�ʱ��ĵ���ң��   SOE
	void Signal_BinarySignalPointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//��ʱ��˫��ң��
	void Signal_BinaryDoublePointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//SOE
	void Signal_SoeBinarySinglePointTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//����ʱ���˫����Ϣ
	void Signal_BinaryDoublePoint(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//�ۻ���  ���  ����
	void Signal_KwhNormal(int iDeviceID, int nPointID, int nValue);
	//�ۼ���  ������
	void Signal_KwhDisNormal(int iDeviceID, int nPointID, int nValue);
	//����ʱ����ۻ���   ���
	void Signal_KwhShortTimeNormal(int iDeviceID, int nPointID, float nValue);
	//����ʱ����ۻ���  ���
	void Signal_KwhLongTimeNormal(int iDeviceID, int nPointID, float nValue);

	//��һ��ң����������
	void Signal_AnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//��Ȼ�ң�ⲻ��������
	void Signal_ScaledAnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//����ʱ��Ĺ�һ������ֵ
	void Signal_AnalogNormalShortTime(int iDeviceID, int nPointID, float nValue, int nQuality);

	//������ң����������
	void Signal_AnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//������ң�ⲻ��������
	void Signal_DisAnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//���ٻ�ȷ��
	void Signal_AllCallRespond();
	//���յ������ݱ���
	void Signal_recv16Data(QByteArray btData, int nLenght);
	//ң�ط�����Ϣ 0����  1 ˫��      0:on 1:off         
	void Signal_ControlFeedBack(int iControlType, int iPointNum, int iOperateType, QString strStatus);
	//��ֵ����
	void Signal_DevReadBack(QMap<int, float> mapInfo);

	//�趨������Ӧ   48/136    6/7   1/0  
	void Signal_devWriteBack(int iControlType, int iCot, int iQos);
	//����
	void Signal_MalFuction(ASDUGZ gzData);
	//
	//����
	void Signal_ZoneFeedBack(int nAddr, float fValue, int qpm, int nCot);
	//д�ļ�
	void Signal_RecvFileData(QString strFileName, QByteArray byData);
	//��ֵ
	void Signal_ReadFixData(DEV_BASE &devData);
	//�ļ�Ŀ¼
	void Signal_FIleCatalogINfo(QList<Catalog_Info>& lstFileInfo);
	//����ȷ��
	void Signal_UpdateConform(int nFloag);
public:
	QList<Catalog_Info> m_lstFiles;
private:
	CCommPlugin* m_p101Impl;
	FILE_ATTR_INFO m_fileAttrInfo;
private:
	C101Cache m_101Cache;
	QByteArray m_ArrbyFileData;
	char m_recv_buf[BUFFER_SIZE_101];
};


#endif//_101_RECVER_H
