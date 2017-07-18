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
	TREE_ITEM_DEBUG,      //������Ϣ
	TREE_ITEM_BREAKER_ANALOG,   //����ֵ
	TREE_ITEM_PROTECT_DEV,     //������ֵ
	TREE_ITEM_SYSTEM_DEV,    //ϵͳ����
	TREE_ITEM_DI_IFNO,      //������
	TREE_INFO_DO_INFO,      //������
	TREE_INFO_SOFT,          //��ѹ��
	TREE_INFO_SOE,          //soe

	//ʵʱ
    TREE_ITEM_REALTIME_DI,   //����
	TREE_ITEM_REALTIME_DO,   //����
	TREE_ITEM_REALTIME_ACTION,  //����
	TREE_ITEM_REALTIME_DISNORMAL, //�쳣
	TREE_ITEM_REALTIME_SOFT,      //��ѹ��
	//��ʷsoe
	TREE_ITEM_HIOSTORY_SOE,   //��ʷOSE
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



/* 1. ��ֵ���Ͷ� */
enum ANALOG_DATA_TYPE
{
	MEASURE_PARAM_INT = 0x00,			///< �з���������
	MEASURE_PARAM_UINT = 0x01,			///< �޷���������
	MEASURE_PARAM_HEX = 0x02,			///< HEX��
	MEASURE_PARAM_FLOAT = 0x03,			///< ������
	MEASURE_PARAM_BIT = 0x04,			///< ��λ��ʾ����
	MEASURE_PARAM_BOOL = 0x05,			///< BOOLֵ

};


#define FLAG_DEVSTATUS_SOE           0x01   //����SOE�¼�����
#define FLAG_DEVSTATUS_RUN           0x02   //����̬
#define FLAG_DEVSTATUS_REMOTE        0x04   //Զ��̬
#define FLAG_DEVSTATUS_LOCKSTATUS    0x08   //��������״̬
#define FLAG_DEVSTATUS_ACTSTATUS     0x10   //��������״̬


enum {
	SOE_ACTION,				/*!< ���������¼� */
	SOE_DIEVENT,			    /*!< �����λ�¼� */
	SOE_ABNORMAL,			   /*!< �쳣�¼� */
	SOE_RUN,				   /*!< �����¼� */
	SOE_WAVE_FILE,		   /*!< ¼���¼� */
	SOE_TRACE,			   /*!< ������Ϣ */
	SOE_TYPE_COUNT
};

enum 
{
	DI_SOE_TYPE_REAL_DI,		//ʵ�ʿ��� �� ��������/�ڲ����룩
	DI_SOE_TYPE_REAL_DO,		//ʵ�ʿ��� �� ��������/�ڲ�������
	DI_SOE_TYPE_VIRPT			//�����
};


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

// 4�ֽ�
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

	unsigned char ms;     //��
public:
	QString Dump()
	{
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", year.GetAddr(), month, day, hour, minute, ms);
		return buf;
	}
};

struct CPTimeMs
{
	DBG_MSG2 year;    //���
	unsigned char month;   //�·�
	unsigned char day;     //����
	unsigned char hour;    //Сʱ
	unsigned char minute;  //����

	unsigned char ms;     //����
	DBG_INFO4 mmins;      //����

public:
	QString Dump()
	{
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.4d", year.GetAddr(), month, day, hour, minute, ms,mmins.GetAddr());
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

//���ֽ�ͨ�ø�ʽ
class DBG_GET_MEAS : public DBG_HEADER
{
public:
	class MEAS_DATA //
	{
	public:
		unsigned char  infoaddr;
		char measData[4];
	};
public:
	enum { MAX_DATA_MEAS = (MAX_DBG_SIZE - sizeof(DBG_HEADER)) / sizeof(MEAS_DATA), };
	MEAS_DATA m_data[MAX_DATA_MEAS];

};

//���ֽ�ͨ�ø�ʽ
class DEG_GET_MSG2 : public DBG_HEADER
{
public:
	class DEG_MSG2
	{
	public:
		unsigned char  infoaddr;
		char measData[2];

	};
public:
	enum { MAX_DATA_MEAS2 = (MAX_DBG_SIZE - sizeof(DBG_HEADER)) / sizeof(DEG_MSG2), };
	DEG_MSG2 m_data[MAX_DATA_MEAS2];

};

//��ѹ��
class DEG_SOFT_INFO : public DBG_HEADER
{
public:
	char measData[4];

};


//soe
class DEG_SOE_DETAIL : public DBG_HEADER
{
public:
	enum {SOE_MAX_DATALENGTH = MAX_DBG_SIZE - sizeof(DBG_HEADER)};
	char SOEDATA[SOE_MAX_DATALENGTH];
};

//soe
class DEG_GET_SOE : public DBG_HEADER
{
public:
	//soe����
	DBG_MSG2 m_SoeType;
	//soe��δ��
	DBG_MSG2 m_SoeIndex;
	//
};

//soe num
class DEG_GET_SEO_NUM : public DBG_HEADER
{
public:
	//action
	DBG_MSG2 m_SoeAction;
	//DI
	DBG_MSG2 m_SoeDi;
	//abnormal
	DBG_MSG2 m_SoeAbNormal;
	//run
	DBG_MSG2 m_SoeRun;
	//waveFile
	DBG_MSG2 m_SoeWaveFile;
	//trace
	DBG_MSG2 m_SoeTrace;
};

//soe info heade
class SOE_INFO_HEADER : public DBG_HEADER
{
public:
	//soe����
	unsigned char m_nType;
	//soeʱ��
	CPTimeMs m_Time;
};

//soe action info
class SOE_ACTION_INFO : public SOE_INFO_HEADER
{
public:
	//soe�¼�����
	unsigned char m_cSoeType;
	//����״̬
	unsigned char m_cActionStatus;

	class DEG_ACTION_ITEM
	{
	public:
		DBG_MSG2  dataType;
		char measData[4];

	};

public:
	enum { MAX_SOE_ACTIONDATA = (MAX_DBG_SIZE - sizeof(SOE_INFO_HEADER) - 2) / sizeof(DEG_ACTION_ITEM), };
	DEG_ACTION_ITEM m_data[MAX_SOE_ACTIONDATA];

};

//soe����
class SOE_NUM_INFO : public DBG_HEADER
{
public:
	//action
	DBG_MSG2 m_actionLeg;
	//dievent
	DBG_MSG2 m_diLeg;
	//abnormal
	DBG_MSG2 m_abnormalleg;
	//run
	DBG_MSG2 m_runleg;
	//wave
	DBG_MSG2 m_waveleg;
	//trace
	DBG_MSG2 m_traceleg;
};

//��λsoe
class SOE_IO_INFO : public SOE_INFO_HEADER
{
public:

	//ң������
	unsigned char m_CBinaryType;
	//ʵ��ID
	unsigned char m_cID;
	//��λ����
	unsigned char m_cChangeType;

};

//�쳣 ���� ¼�� SOE
class SOE_ID_INFO : public SOE_INFO_HEADER
{
public:
	//ID
	unsigned char m_cType;
};


//����ʱ�䱨��
class TIME_SET_INFO : public DBG_HEADER
{
public:
	//time
	CP56Time2a m_time;
};

//���÷��ر���
class TIME_SET_ACK_INFO : public DBG_HEADER
{
public:
	//code
	unsigned char m_cCode;
	//�������
	unsigned char m_Result;
	//�������
	unsigned char m_AddResult;
};


//ң��
class REMOTE_CONTROM_SET :public DBG_HEADER
{
public:
	//�������� 
	unsigned char m_ControlType;
	//���ƶ���'
	unsigned char m_ControlObj;
	//��������
	unsigned char m_Order;
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

//��ֵ
class CDevInfo
{
public:
	//���
	int m_nPointNum;
	//��ֵ�� ����
	QString m_strDevType;
	//��ֵ����
	QString m_strDevName;
	//��λ
	QString m_strDevUnit;
	//��ֵ��Χ
	QString m_strDevRange;
	//��ǰֵ
	int m_strValue;
};

//���뿪������
class CDIDOInfo
{
public:
	//���
	int m_nPointNum;
	//����
	QString m_nName;
	//ֵ
	int m_nValue;
};

//��ѹ�嶨ֵ
class CSOFTSTRAP
{
public:
	//���
	int m_nPointNum;
	//�ڲ�id
	int m_nOwnId;
	//����
	QString m_strName;
	//ֵ
	int m_nVlaue;
};

//�¼����� ͳ����Ϣ��Ϣ
class SOE_EVENT_STATIC
{
public:
	enum
	{
		SOE_SEO_NULL,
		SOE_GET_NUMBER,   // ��ȡ��Ŀ��
		SOE_GET_ACTION,       //��ȡaction
		SOE_GET_DIEVENT,      //��ȡDi
		SOE_GET_ABNORMAL,    //��ȡ�쳣
		SOE_GET_RUN,        //��ȡrun
		SOE_GET_WAVE,        //��ȡ¼��
		SOE_GET_TRACE,       //��ȡtrace
	};

	SOE_EVENT_STATIC()
	{
		m_nCurrentStatic = SOE_SEO_NULL;

		//action����Ŀ��
		m_nActionTotalNum = 0;
		//aciton��ǰ��Ŀ
		m_nActionCurrentNum = 0;

		//di����Ŀ��
		m_nDiTotalNum = 0;
		//di��ǰ��Ŀ��
		m_nDiCurrentNum = 0;

		//abnormal ����
		m_nAbnormalTotalNum = 0;
		//abnormal   ��ǰ��Ŀ
		m_nAbnoramlCurrentNum = 0;

		//run ������
		m_nRunTotalNum = 0;
		//run ��ǰ��Ŀ��
		m_nRunCurrentNum = 0;

		//wave����
		m_nWaveTotaolNum = 0;
		//wave��ǰ��Ŀ
		m_nVaveCurrentNum = 0;

		//tracel
		m_nTraceTotalNum = 0;
		//tarce
		m_nTraceCurrentNum = 0;
	}

	void ClearStatic()
	{
		m_nCurrentStatic = SOE_SEO_NULL;

		//��ǰ������״̬
		m_nCurrentStatic = 0;

		//action����Ŀ��
		m_nActionTotalNum = 0;
		//aciton��ǰ��Ŀ
		m_nActionCurrentNum = 0;

		//di����Ŀ��
		m_nDiTotalNum = 0;
		//di��ǰ��Ŀ��
		m_nDiCurrentNum = 0;

		//abnormal ����
		m_nAbnormalTotalNum = 0;
		//abnormal   ��ǰ��Ŀ
		m_nAbnoramlCurrentNum = 0;

		//run ������
		m_nRunTotalNum = 0;
		//run ��ǰ��Ŀ��
		m_nRunCurrentNum = 0;

		//wave����
		m_nWaveTotaolNum = 0;
		//wave��ǰ��Ŀ
		m_nVaveCurrentNum = 0;

		//tracel
		m_nTraceTotalNum = 0;
		//tarce
		m_nTraceCurrentNum = 0;
	}
public:
	//��ǰ������״̬
	int m_nCurrentStatic;

	//action����Ŀ��
	int m_nActionTotalNum;
	//aciton��ǰ��Ŀ
	int m_nActionCurrentNum;

	//di����Ŀ��
	int m_nDiTotalNum;
	//di��ǰ��Ŀ��
	int m_nDiCurrentNum;

	//abnormal ����
	int m_nAbnormalTotalNum;
	//abnormal   ��ǰ��Ŀ
	int m_nAbnoramlCurrentNum;

	//run ������
	int m_nRunTotalNum;
	//run ��ǰ��Ŀ��
	int m_nRunCurrentNum;

	//wave����
	int m_nWaveTotaolNum;
	//wave��ǰ��Ŀ
	int m_nVaveCurrentNum;

	//tracel
	int m_nTraceTotalNum;
	//tarce
	int m_nTraceCurrentNum;

public:

	
};