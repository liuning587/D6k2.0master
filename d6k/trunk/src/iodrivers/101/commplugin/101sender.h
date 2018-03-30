
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
	int   OnControl(NBM_TELECTRL* pTelectrl);//ң��
	bool  OnTimer();//��ʱ��
	bool  OnSend();//�����߼�
public:
	bool LogE(int nType, char* pBuff, int nLength);
	bool LogF(int nType, char* pBuff, int nLength);
	bool LogV(int nType, char* pBuff, int nLength);
	void LogMsg(const char*, int nLevel);
public:
	bool Send_F(char* pBuff, int nLen);//�̶�֡��
	bool Send_V(char* pBuff, int nLen);//�ɱ�֡��
public:
	char CheckSum(char *buf, short len);
public://��̨����װ��
	bool SendRequireLinkReset(); //��λԶ����·
	bool SendRequireLinkState(); //������·״̬
	bool SendRequireLinkData();  //������·����
	bool SendRequireAllData();   //������������

	bool SendGeneralCall();//����
	bool SendSyncTime();   //��ʱ
	bool SendSyncRdTime(); //��ȡ��ʱʱ��
	bool SendConfirm();    //ȷ�ϱ���
	bool SendHeartBeat();
	bool SendResetProcess(NBM_TELECTRL* pTelectrl);
private://ң��
	bool OnSendSetAnalogNormalizeRequest(NBM_TELECTRL& command);
	bool OnSendSetAnalogScaleRequest(NBM_TELECTRL& command);
	bool OnSendSetAnalogShortFloatRequest(NBM_TELECTRL& command);
	bool OnSendSetBinarySPRequest(NBM_TELECTRL& command);
	bool OnSendSetBinaryDPRequest(NBM_TELECTRL& command);

public://װ�÷�����̨
	bool SendReplyLinkReset();//�ظ���λԶ����·
	bool SendReplyLinkState(); //�ظ�������·״̬
public:
	//�ٻ�����Ŀ¼
	bool OnSendGetCatalogRequest(const FILE_CATALOG_REQUEST_1 &lbCatalog);
	//���ļ�����
	bool OnSendReadFileAction(const FILE_ATTR_INFO& fileAttr);
	//д�ļ�����
	bool OnSendWriteFileAction(const FILE_ATTR_INFO& fileAttr);
	//д�ļ�
	bool OnSendWriteFileData();
	//��У��
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

	//iec ��ȡ��д��
	bool OnSendIecDataRequest(IEC_BASE *pRequestIec);
	//��ֵд��ȷ��
	bool OnSendDevWriteConform();
	//��������
	bool OnSendZoomDataRequest(ZOOM_BASE *pZoomData);
	//¼���ļ���ȡ
	bool OnSendRecordCatalog(LB_DATA dLbData);
	//��ֵ���л�
	bool OnSendFixAreaCode(ASDU200 *pRequestArea);
	//��ȡ¼���ļ���Ϣ
	LB_DATA GetLbData() const;
	//ѡ���ļ�
	bool OnSendChooseLbFile();
	//�����ļ�
	bool OnSendRequestFile(int nIndex);
	//��ֵȡ��ѯ
	bool OnSendFixSearch();
	//
	void SetLbAllInfo(const QList<LB_DATA> &lstLbData);
	//������
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
	//���ݰ���ʼ�����
	int m_nStartNode;
	//��ǰ ������״̬  0���Բ���  1�����Բ���
	int m_nCurrentOperFlag;
	//��ʼ���ͱ�ʶ
	int m_nGhFlag;
	//д�����ļ�����
	QByteArray m_btWriteData;
	//¼������
	LB_DATA m_lbData;
	//������Ҫ���ص�¼������
	QList<LB_DATA> m_lstLbData;

	QString m_strFilePath;
	//
	DEV_BASE m_WriteDevInfo;
	//dev index
	int m_nDevIndex;
};


#endif//_101_SENDER__H
