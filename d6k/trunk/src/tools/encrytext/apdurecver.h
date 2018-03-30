/********************************************************************************
模块名       :  apdu接收类
文件名       :  apdurecver.h
文件实现功能 :  接收网络数据解析104数据
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef CAPDURECVER_H
#define CAPDURECVER_H
#include <QObject>
#include <QThread>
#include <QAbstractSocket>
#include <QDateTime>
#include <QThread>
#include <QTcpServer>
#include "104cache.h"
#include "asdu_info.h"

class CApduSender;
class CApduRecver : public QObject
{
    Q_OBJECT
public:
    CApduRecver(QObject *parent=0);
    ~CApduRecver();
public:
	//
	void StartTcpServer();

    //解析数据
    bool OnReceive(char *pBuff, int nLen);
	//收到装置侧信息
	bool OnDeviceRecv(char *pBuff, int nLen);

    bool AnalyseApdu(char* pBuff,int nLength);
	bool AnalyseDeviceApdu(char* pBuff, int nLength);

    bool AnalyseI(char* pBuff,int nLength);
    void AnalyseAsdu(char* pBuff,int nLength);

    bool AnalyseU(char* pBuff,int nLength);
	//
	bool AnalyseS(char* pBuff, int nLength);
	//发送总召信息
	void SendRequestAllMsg();
	//
	void SetSender(CApduSender *pSender);

	//
	QTcpSocket * GetZuTcpSocket();
	//接收到装置返回的信息进行解密操作
	//解密
	void OnDeCode(char *pBuff, int nLength);
	//读取终端芯片证书
	void ReadCerFile(const QString &fileName,char *cel, int *len);
	

signals:

public slots:
    void Slot_NewConnect();
	//
	void Slot_ReadMessage();

private:
	//遥信
	void OnRecvBinarySinglePoint(char* pBuff, int nLength);
	//双点遥信
	void OnRecvBinaryDoublePoint(char* pBuff, int nLength);
	//成组单点遥信
	void OnRecvBinaryGroupPoint(char* pBuff, int nLength);
	//SOE
	void OnRecvBinarySinglePointTime(char* pBuff, int nLength);
	//带短时标的单点信息  SOE
	void OnRecvBinarySinglePointShortTime(char* pBuff, int nLength);
	//
	void OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength);
	void OnRecvBinaryDoublePointTime(char* pBuff, int nLength);
    //归一化遥测
	void OnRecvAnalogNormalized(char* pBuff, int nLength);//ASDU9
	//浮点型遥测
	void OnRecvAnalogShortFloat(char* pBuff, int nLength);//ASDU13 
	//带短时标的归一化测量值
	void OnRecvAnalogNormalizedShortTime(char* pBuff, int nLength); 
	//标度化测量值
	void OnRecvAnalogScaled(char* pBuff, int nLength);
	//带短时标的标度化测量值
	void OnRecvAnalogScaledShortTime(char* pBuff, int nLength);
	//带短时标的短浮点数测量值
	void OnRecvAnalogShortFloatShortTime(char* pBuff, int nLength);
	//不带品质描述词的归一化测量值
	void OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLength);
    //复位进程
    void OnRecvResetProcess(char *pBuff, int nLength);
	//切换定值区
	void OnRecvSwitchFixArea(char *pBuff, int nLength);
	//读取当前定值区号
	void OnrecvReadCurrentFixArea(char *pBuff, int nLength);

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

	//累计量 ASDU15
	void OnRecvKwh(char* pBuff, int nLength);//ASDU15
	//带短时标的累计量
	void OnRecvKwhShortTime(char* pBuff, int nLength);//ASDU16
	//带长时标的累计量
	void OnRecvKwhLongTime(char* pBuff, int nLength);//ASDU37
	//单点遥控反馈数据解析
	void OnRecvSetBinarySPAck(char* pBuff, int nLength);
	//双点遥控反馈数据解析
	void OnRecvSetBinaryDPAck(char* pBuff, int nLength);
	//参数读取
	void OnRecvDevReadRequestAck(char* pBuff, int nLength);
	//参数写入
	void OnRecvDevWriteRequestAck(char *pBuff, int nLength);
	//iec  写入
	void OnRecvIecWriteRequestAck(char *pBuff, int nLength);

	//iec read
	void OnRecvIecReadRequestAck(char* pBuff, int nLength);

    //和校验
    unsigned char checkAllData(const unsigned char *data, int length);
    //故障事件
    void OnRecvDefalutMsg(char *pBuff, int nLength);
    //死区
    void OnRecvZoneMsg(char *pBuff, int nLength);
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
	
	//写文件传输确认
	void OnRecvWriteConform(char *pBuff, int nLength);
	//升级激活确认
	void OnRecvUpdateActionAck(char *pBuff, int nLength);
	//读取内容
	void OnRecvReadIecData(char *pBuff, int nLength);
	//
	bool OnAnalyseDecode(char *pBuff, int nLength);
	


private:
    //104数据  主站发送
    C104Cache m_104Cache;
	//104  装置发送 解密前数据
	CDecodeCache m_DeviceEncodeCache;
	//104  数据  装置发送  解密后数据
	C104Cache m_104DeviceCache;
	//
	//tcp服务端
	QTcpServer *m_pTcpServer;
	//发送端
	CApduSender *m_pSender;
	//主站已经连接 装置还未连接前  保存的数据
	QByteArray m_btArray;
	// 主站和中间之间tcp
	QTcpSocket *m_pzMidTcp;
};

#endif // CAPDURECVER_H
