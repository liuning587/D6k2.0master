/********************************************************************************
模块名       :  apdu接收类
文件名       :  apdusender.h
文件实现功能 :  发送104数据类
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef CAPDUSENDER_H
#define CAPDUSENDER_H
#include <QObject>

class CSocketThread;
class CApduSender : public QObject
{
	Q_OBJECT
public:
	CApduSender(QObject *parent = 0);
	~CApduSender();
	CSocketThread *GetSendSocket();
	//
	void ConnectToDevice();
	//
	void SetSenderIp(const QString &strIp);
	//
	void SetSenderPort(int nPort);
	//
	bool GetDeviceConnectFlag();
public slots:
	//
    void Slot_DeviceConnectScuess(QString strScuessInfo);

public:

	CSocketThread *m_pSendThread;
	//装置ip
	QString m_strDeviceIp;
	//装置端口
	int m_nDevicePort;
	//装置侧连接成功标识
	bool m_bDeviceConnectFlag;


};

#endif // CAPDUSENDER_H
