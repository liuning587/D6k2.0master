
#ifndef _101_SENDER__H
#define _101_SENDER__H

#include "define_101.h"
#include <QObject>



#ifndef MSB
#define MSB(x) (((x) >> 8) & 0xff)	  /* most signif byte of 2-byte integer */
#define LSB(x) ((x)&0xff)			  /* least signif byte of 2-byte integer*/
#define MSW(x) (((x) >> 16) & 0xffff) /* most signif word of 2-word integer */
#define LSW(x) ((x)&0xffff)			  /* least signif byte of 2-word integer*/
#endif

class CDeviceData;
class CCommPlugin;

enum
{
	HAS_COMMAND,
	HAS_TELECTRL,
	HAS_NOTHING,
};

class C101Sender : public QObject
{
	Q_OBJECT
public:
	C101Sender(QObject* parent = 0);
	virtual ~C101Sender();

public:
	int   OnControl(NBM_TELECTRL* pTelectrl);//遥控
	bool  OnTimer();//定时器
	bool  OnSend();//常规逻辑
public:
	bool LogE(int nType, char* pBuff, int nLength);
	bool LogF(int nType, char* pBuff, int nLength);
	bool LogV(int nType, char* pBuff, int nLength);
	void LogMsg(const char*, int nLevel);
public:
	bool Send_F(char* pBuff, int nLen);//固定帧长
	bool Send_V(char* pBuff, int nLen);//可变帧长
public:
	char CheckSum(char *buf, short len);
public://后台发往装置
	bool SendRequireLinkReset(); //复位远方链路
	bool SendRequireLinkState(); //请求链路状态
	bool SendRequireLinkData();  //请求链路数据
	bool SendRequireAllData();   //请求总召数据

	bool SendGeneralCall();//总召
	bool SendSyncTime();   //对时
	bool SendSyncRdTime(); //读取对时时间
	bool SendConfirm();    //确认报文
	bool SendHeartBeat();
	bool SendResetProcess(NBM_TELECTRL* pTelectrl);
private://遥控
	bool OnSendSetAnalogNormalizeRequest(NBM_TELECTRL& command);
	bool OnSendSetAnalogScaleRequest(NBM_TELECTRL& command);
	bool OnSendSetAnalogShortFloatRequest(NBM_TELECTRL& command);
	bool OnSendSetBinarySPRequest(NBM_TELECTRL& command);
	bool OnSendSetBinaryDPRequest(NBM_TELECTRL& command);

public://装置发往后台
	bool SendReplyLinkReset();//回复复位远方链路
	bool SendReplyLinkState(); //回复请求链路状态
public:
	//召唤报文目录
	bool OnSendGetCatalogRequest(const FILE_CATALOG_REQUEST_1 &lbCatalog);
	//读文件激活
	bool OnSendReadFileAction(const FILE_ATTR_INFO& fileAttr);
	//写文件激活
	bool OnSendWriteFileAction(const FILE_ATTR_INFO& fileAttr);
	//写文件
	bool OnSendWriteFileData();
	//和校验
	unsigned char checkAllData(const char *data, int length);

	void ClearFilepath()
	{
		m_strFilePath.clear();
	}
	void SetOperatorFlag(int nFlag)
	{
		m_nCurrentOperFlag = nFlag;
	}
public:
	bool OnSendDevDataRequest(DEV_BASE *pRequestDz);

	//iec 获取和写入
	bool OnSendIecDataRequest(IEC_BASE *pRequestIec);
	//定值写入确认
	bool OnSendDevWriteConform();
	//死区设置
	bool OnSendZoomDataRequest(ZOOM_BASE *pZoomData);
	//录波文件获取
	bool OnSendRecordCatalog(LB_DATA dLbData);
	//定值区切换
	bool OnSendFixAreaCode(ASDU200 *pRequestArea);
	//获取录波文件信息
	LB_DATA GetLbData() const;
	//选择文件
	bool OnSendChooseLbFile();
	//请求文件
	bool OnSendRequestFile(int nIndex);
	//定值取查询
	bool OnSendFixSearch();
	//
	void SetLbAllInfo(const QList<LB_DATA> &lstLbData);
	//最后结束
	void OnSendFileConformNode();
	//
	QList<LB_DATA> GetLbAllData();

	void SetGhFlag(int nFlag)
	{
		m_nGhFlag = nFlag;
	}
	int GetGhFlag()
	{
		return m_nGhFlag;
	}
private:
	unsigned char m_send_buf[BUFFER_SIZE_101];
	unsigned char m_send_len;

private:
	CCommPlugin * m_p101Impl;
	//数据包起始借点编号
	int m_nStartNode;
	//当前 所处的状态  0可以操作  1不可以操作
	int m_nCurrentOperFlag;
	//开始发送标识
	int m_nGhFlag;
	//写出的文件内容
	QByteArray m_btWriteData;
	//录波数据
	LB_DATA m_lbData;
	//所有需要下载的录波数据
	QList<LB_DATA> m_lstLbData;

	QString m_strFilePath;
	//
	DEV_BASE m_WriteDevInfo;
	//dev index
	int m_nDevIndex;
};


#endif//_101_SENDER__H
