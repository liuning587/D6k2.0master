#pragma once

#include <QString>
#include <QMap>

enum 
{
	DBG_HEADER0 = 0xAA,
	DBG_HEADER1 = 0x55,
};

//ÿ֡������󳤶�
enum
{
	MAX_DBG_SIZE = 2048,
};

//���νṹitem���
enum
{
	TREE_ITEM_BREAKER,    //ֱ����·��
	TREE_ITEM_BREAKER_ANALOG,   //����ֵ
};

typedef unsigned short  BIT16;  //16λ
typedef unsigned long   BIT32;  //32λ
typedef unsigned char   BIT08;  //08λ


#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))
#define MYMAKE32(l, h)      ((BIT32)(((BIT16)(((BIT32)(l)) & 0xffff)) | ((BIT32)((BIT16)(((BIT32)(h)) & 0xffff))) << 16))



#define DBG_CODE_GET_SYS_INFO      1 //��ȡϵͳ��Ϣ
#define DBG_CODE_GET_MEAS          2 //��ȡ����ֵ��Ϣ
#define DBG_CODE_GET_DEBUG_DATA    3 //��ȡ����ֵ��Ϣ
#define DBG_CODE_GET_SETTING       4 //��ȡ������ֵ
#define DBG_CODE_GET_SYSCFG        5 //��ȡϵͳ����
#define DBG_CODE_GET_DICFG         6 //��ȡ�������
#define DBG_CODE_GET_DOCFG         7 //��ȡ��������
#define DBG_CODE_GET_SOFTSTRAP     8 //��ȡ��ѹ�嶨ֵ

#define DBG_CODE_GET_SOE           40 //��ȡ�¼���Ϣ

#define DBG_CODE_SET_TIME          10 //����ʱ��
#define DBG_CODE_SET_SETTING       11 //���ñ�����ֵ
#define DBG_CODE_SET_SYSCFG        12 //����ϵͳ����
#define DBG_CODE_SET_DICFG         13 //���ÿ������
#define DBG_CODE_SET_DOCFG         14 //���ÿ�������
#define DBG_CODE_SET_SOFTSTRAP     15 //������ѹ�嶨ֵ

#define DBG_CODE_DBG_FORCE           20//ǿ�Ʋ��
#define DBG_CODE_DBG_UNFORCE         21//���ǿ��
#define DBG_CODE_DBG_GENSOE          22//�����¼�SOE
#define DBG_CODE_DBG_START_WAVE      23//���Բ���¼��

#define DBG_CODE_RM_CTRL             30//ң��



//���ĳ���   2�ֽ�
class   DBG_MSG2
{
public:
	unsigned char m_asduaddr0;
	unsigned char m_asduaddr1;
public:
	void SetAddr(unsigned short addr)
	{
		m_asduaddr0 = MYLO8(addr);
		m_asduaddr1 = MYHI8(addr);
	}
	unsigned short GetAddr()
	{
		return MYMAKE16(m_asduaddr0, m_asduaddr1);
	}
};

//����ֵ   4�ֽ�
class DBG_INFO4
{
public:
	unsigned char m_infoaddr0;
	unsigned char m_infoaddr1;
	unsigned char m_infoaddr2;
	unsigned char m_infoaddr3;
public:
	void SetAddr(int addr)
	{
		unsigned short  lo16 = MYLO16(addr);
		unsigned short  hi16 = MYHI16(addr);

		m_infoaddr0 = MYLO8(lo16);
		m_infoaddr1 = MYHI8(lo16);

		m_infoaddr2 = MYLO8(hi16);
		m_infoaddr3 = MYHI8(hi16);
	}

	int GetAddr()
	{
		unsigned short lo16 = MYMAKE16(m_infoaddr0, m_infoaddr1);
		unsigned short hi16 = MYMAKE16(m_infoaddr2, m_infoaddr3);

		return MYMAKE32(lo16, hi16);
	}
};

//CP56Time2aʱ��ṹ
struct CP56Time2a
{
	DBG_MSG2 year;    //���
	unsigned char month;   //�·�
	unsigned char day;     //����
	unsigned char hour;    //Сʱ
	unsigned char minute;  //����

	unsigned char ms;     //����
		
public:
	QString Dump()
	{
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", year.GetAddr(), month, day, hour, minute, ms);
		return buf;
	}
};

//����ͷ��Ϣ
class DBG_HEADER
{
public:
	unsigned char header0;   //��ʼͷ0
	unsigned char header1;   //��ʼͷ1
	DBG_MSG2 msgLeg;   //���ĳ���
	unsigned char type;      //���ͱ�ʶ
};

//��ȡ��ϵͳ��Ϣ
class DBG_GET_SYS_INFO : public DBG_HEADER
{
public:
	unsigned char version;  //����汾
	CP56Time2a time;               //����ʱ��
};

//����ֵ
class DBG_GET_MEAS : public DBG_HEADER
{
private:
	class MEAS_DATA //��ɢ���ź���
	{
	public:
		unsigned char  infoaddr;
		DBG_INFO4 measData;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	enum { MAX_DATA_MEAS = (MAX_DBG_SIZE - sizeof(DBG_HEADER)) / sizeof(MEAS_DATA), };
	MEAS_DATA m_data[MAX_DATA_MEAS];

};

//��ʼ����Ϣ  ip�Ͷ˿�
class CBreakInitData
{
public:
	CBreakInitData()
	{
		m_nPort = 0;
	}

	~CBreakInitData() {};

	const QString &GetIpAddress()
	{
		return m_strIpAddress;
	}

	void SetIpAddress(const QString &strAddr)
	{
		m_strIpAddress = strAddr;
	}

	void SetPort(int nPort)
	{
		m_nPort = nPort;
	}

	int GetPort()
	{
		return m_nPort;
	}

private:
	//ip��ַ
	QString m_strIpAddress;
	//�˿�
	int m_nPort;
};


//�����Ϣ
//����ֵ
class CAnalogPointInfo
{
public:
	//���
	int m_nPointNum;
	//����
	QString m_strType;
	//��ֵ����
	QString m_strName;
	//��λ
	QString m_strUnit;
	//��ֵ����
	int m_nValueType;
	//С�����λ
	int m_nSmallNum;
	//ֵ
	QString m_strValue;
};
