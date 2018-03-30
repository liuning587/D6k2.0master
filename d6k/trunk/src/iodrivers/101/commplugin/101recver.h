
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
	bool AnalyseE5(char* pBuff, int nLen);//单字节
	bool AnalyseF(char* pBuff, int nLen);//固定帧
	bool AnalyseV(char* pBuff, int nLen);//可变帧
	bool AnalyseAsdu(char* pBuff, int nLen);//ASDU
public:
	bool OnRecvFfromMonitor(char* pBuff, int nLen);//固定帧：来自装置
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
	bool OnRecvAnalogNormalized(char* pBuff, int nLen);//归一化测量值 ASDU9
	bool OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen);//带短时标的归一化测量值 ASDU10
	bool OnRecvAnalogScaled(char* pBuff, int nLen);//标度化测量值 ASDU11
	bool OnRecvAnalogScaledShortTime(char* pBuff, int nLen);//带短时标的标度化测量值 ASDU12
	bool OnRecvAnalogShortFloat(char* pBuff, int nLen);//短浮点数测量值 ASDU13
	bool OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen);//带短时标的短浮点数测量值 ASDU14
	bool OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen);//不带品质描述词的归一化测量值 ASDU21
	bool OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen);//带长时标的归一化测量值 ASDU34
	bool OnRecvAnalogScaledLongTime(char* pBuff, int nLen);//带长时标的标度化测量值 ASDU35
	bool OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen);//带长时标的短浮点数测量值 ASDU36
private:
	void OnRecvBinarySinglePointTime(char* pBuff, int nLength);//ASDU30
	void OnRecvBinaryDoublePointTime(char* pBuff, int nLength);//ASDU31
	void OnRecvBinarySinglePointShortTime(char* pBuff, int nLength);//ASDU2
	void OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength);//ASDU4
private:
	bool OnRecvBinarySinglePoint(char* pBuff, int nLen);//开关量单点 ASDU1
	bool OnRecvBinaryDoublePoint(char* pBuff, int nLen);//开关量双点 ASDU3
	bool OnRecvBinaryGroupPoint(char* pBuff, int nLen);//带变位检出的成组单点 ASDU20
private:
	void OnRecvKwh(char* pBuff, int nLength);//累计量ASDU15
	void OnRecvKwhShortTime(char* pBuff, int nLength);//带短时标的累计量ASDU16
	void OnRecvKwhLongTime(char* pBuff, int nLength);//带长时标的累计量ASDU37
	void OnRecvFaultMsg(char* pBuff,int nLength);
private://遥控
	void OnRecvSetAnalogNormalizeAck(char* pBuff, int nLength); //ASDU48
	void OnRecvSetAnalogScaleAck(char* pBuff, int nLength);//ASDU49
	void OnRecvSetAnalogShortFloatAck(char* pBuff, int nLength);//ASDU50
	void OnRecvSetBinarySPAck(char* pBuff, int nLength);//ASDU45
	void OnRecvSetBinaryDPAck(char* pBuff, int nLength);//ASDU46

	//文件服务处理
	void OnRecvFileAnalyseInfo(char *pBuff, int nLength);
	//目录召唤确认
	void OnRecvCatalogRespond(char *pBuff, int nLength);
	//读文件激活确认
	void OnRecvFileAction(char *pBuff, int nLength);

	//读文件数据传输
	void OnRecvFileData(char *pBuff, int nLength);
	//文件传输确认
	void OnRecvFileConform(char *pBuff, int nLength);

	//写文件传输确认
	void OnRecvWriteConform(char *pBuff, int nLength);
	//升级激活确认
	void OnRecvUpdateActionAck(char *pBuff, int nLength);
	//读取内容
	void OnRecvReadIecData(char *pBuff, int nLength);

	//文件目录召唤相应
	void OnRecvCatalogRepond(char *pBuff, int nLength);
	//读文件激活相应
	void OnRecvReadActionResond(char *pBuff, int nLenght);
	//读数据文件传输
	void OnRecvReadFileRespond(char *pBuff, int nLength);
	//读文件传输确认
	void OnRecvReadConform(char *pBuff, int nLength);
	//写文件激活
	void OnRecvWriteAction(char *pBuff, int nLength);
private:
	//切换定值区
	void OnRecvSwitchFixArea(char *pBuff, int nLength);
	//读取当前定值区号
	void OnrecvReadCurrentFixArea(char *pBuff, int nLength);
	//参数读取
	void OnRecvDevReadRequestAck(char* pBuff, int nLength);
	//参数写入
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
	//单点连续遥信数据
	void Signal_OnePointRemote(int iDeviceID, int nPointID, int nValue);
	//单点非连续遥信数据
	void Signal_onePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//成组单点遥信  连续
	void Signal_BinaryGroupPoint(int iDeviceID, int nPointID, int nValue);
	//成组单点遥信  非连续
	void Signal_BinaryGroupDisPoint(int iDeviceID, int nPointID, int nValue);

	//双点连续遥信数据
	void Signal_DoublePointRemote(int iDeviceID, int nPointID, int nValue);
	//双点非连续遥信数据
	void Signal_DoublePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//带3个字节短时标的单点遥信   SOE
	void Signal_BinarySignalPointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//短时标双点遥信
	void Signal_BinaryDoublePointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//SOE
	void Signal_SoeBinarySinglePointTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//带长时标的双点信息
	void Signal_BinaryDoublePoint(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//累积量  电度  连续
	void Signal_KwhNormal(int iDeviceID, int nPointID, int nValue);
	//累计量  不连续
	void Signal_KwhDisNormal(int iDeviceID, int nPointID, int nValue);
	//带短时标的累积量   电度
	void Signal_KwhShortTimeNormal(int iDeviceID, int nPointID, float nValue);
	//带长时标的累积量  电度
	void Signal_KwhLongTimeNormal(int iDeviceID, int nPointID, float nValue);

	//归一化遥测连续数据
	void Signal_AnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//标度化遥测不连续数据
	void Signal_ScaledAnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//带短时标的归一化测量值
	void Signal_AnalogNormalShortTime(int iDeviceID, int nPointID, float nValue, int nQuality);

	//浮点型遥测连续数据
	void Signal_AnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//浮点型遥测不连续数据
	void Signal_DisAnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//总召唤确认
	void Signal_AllCallRespond();
	//接收到的数据报文
	void Signal_recv16Data(QByteArray btData, int nLenght);
	//遥控反馈信息 0单点  1 双点      0:on 1:off         
	void Signal_ControlFeedBack(int iControlType, int iPointNum, int iOperateType, QString strStatus);
	//定值数据
	void Signal_DevReadBack(QMap<int, float> mapInfo);

	//设定数据响应   48/136    6/7   1/0  
	void Signal_devWriteBack(int iControlType, int iCot, int iQos);
	//故障
	void Signal_MalFuction(ASDUGZ gzData);
	//
	//死区
	void Signal_ZoneFeedBack(int nAddr, float fValue, int qpm, int nCot);
	//写文件
	void Signal_RecvFileData(QString strFileName, QByteArray byData);
	//定值
	void Signal_ReadFixData(DEV_BASE &devData);
	//文件目录
	void Signal_FIleCatalogINfo(QList<Catalog_Info>& lstFileInfo);
	//升级确认
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
