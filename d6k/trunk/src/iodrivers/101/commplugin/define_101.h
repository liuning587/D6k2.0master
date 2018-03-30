
#ifndef _DEFINE_101_H
#define _DEFINE_101_H


#include "asdu_info.h"


class CDeviceData
{
public:
	CDeviceData()
	{
		Reset();
	}
	//װ�û�������
public:
	int m_nDeviceID;           //װ��ID
	int m_nDeviceAddr;         //װ�õ�ַ

public:
	int m_nACD;//
	int m_nFCB;                //�̶�֡��ʽ�е��ϴ�FCBֵ
	int m_nDeviceState;        //װ�ô���ͨѶ�����е�״̬
	int m_nRunState;           //���ڽ��ջ���״̬
	bool m_bIsInited;          //��ʼ�������Ƿ����(��λ������״̬�����٣���ʱ������)

							   //ǰ�ö�״̬����
public:
	int m_nLastCallTime;       //�ϴ�����ʱ��
	int m_nLastSyncTime;       //�ϴζ�ʱʱ��
	int m_nErrorCount;         //��ǰ������Ŀ
	int m_nLastHaltTime;       //����ʱ��
	bool  m_bIsHalt;           //װ���Ƿ����
	int m_nReSendCount;        //�ط�����

public:
	bool canSend()
	{
		if (m_nRunState == DEVICE_RUN_STATE_UNKNOWN)
		{
			m_nReSendCount = 0;
			return true;
		}

		if (m_nRunState == DEVICE_RUN_STATE_RECV)
		{
			m_nReSendCount = 0;
			return true;
		}

		//��������û�յ������ط�
		if (m_nRunState == DEVICE_RUN_STATE_SEND && m_nReSendCount >= 3)
		{
			m_nReSendCount = 0;

			if (m_nErrorCount >= 2)
			{
				m_bIsHalt = true;
				return false;
			}

			m_nErrorCount++;
			
			return true ;
		}

		m_nReSendCount++;

		return false;
	}

	bool canRecv()
	{
		if (m_nRunState == DEVICE_RUN_STATE_SEND)
			return true;

		return false;
	}

public:
	void Reset()
	{
		m_nErrorCount = 0;
		m_nLastHaltTime = 0;
		m_nLastCallTime = 0;
		m_nLastSyncTime = 0;
		m_bIsHalt = false;
		m_nDeviceState = DEVICE_STATE_UNKNOWN;
		m_nRunState = DEVICE_RUN_STATE_UNKNOWN;
		m_bIsInited = false;

		m_nReSendCount = 0;

		m_nFCB = 0;
		m_nACD = 0;
	}
};


#define BUFFER_SIZE_101 264 //���ͺͽ��ջ�������С

#ifdef SOURTHSTATEGRID
enum
{
	DIRECTION_DOWN = 0,      //����
	DIRECTION_UP = 1,        //����
};
#else
enum
{
	DIRECTION_DOWN = 1,      //����
	DIRECTION_UP = 0,        //����
};
#endif


enum
{
        TYPE_E=1,
        TYPE_F=2,
        TYPE_V=4,
};

//��·������
class CONTROL
{
public:
        CONTROL()
        {
                
        }
public:
        //��4λ(4-7)
        unsigned char  m_FUNCCODE:4;    //��·������
        //3
        unsigned char  m_FCV:1;            //FCV:1:FCB��Ч,0:FCB��Ч;//DFC:1:����վ���ܽ��պ�������       
        //2
        unsigned char  m_DATA:1;            //FCB:����վͨ���ж�FCB�Ƿ�ת�������Ƿ��ط���һ֡����;//ACD:1:����վ��һ������,0:�ж�������
        //1
        unsigned char  m_PRM:1;             //0:���� 1:����
        //0
        unsigned char  m_Dir:1;             //���䷽��λ,0,1����
};

//�̶�֡��ʽ
class AFCI //fix
{
public:
        AFCI()
        {
                m_head = 0x10;
                m_tail = 0x16;
        }

public:
        unsigned char m_head;//֡ͷ

        CONTROL        m_ctrl;         //��·������
        LINKADDR       m_addr;         //��·��ַ
        unsigned char  m_chksum;       //У����

        unsigned char   m_tail;         //֡β
};

//�ɱ�֡��ʽ
class AVDU_HEAD //variable
{
public:
        AVDU_HEAD()
        {
                m_head1 = 0x68;
                m_head2 = 0x68;
        }
public:
        unsigned char m_head1;       //֡ͷ
        unsigned char m_length1;     //��·������+��·��ַ+Ӧ�÷������ݵ�Ԫ
        unsigned char m_length2;     //��·������+��·��ַ+Ӧ�÷������ݵ�Ԫ
        unsigned char m_head2;       //֡ͷ

        CONTROL        m_ctrl;       //��·������
		LINKADDR       m_addr;       //��·��ַ

public:
        void SetDataLength(int nAsduLen)
        {
                m_length1 = nAsduLen + sizeof LINKADDR +1;
                m_length2 = m_length1;
        }
};

class AVDU_TAIL
{
public:
        AVDU_TAIL()
        {
                m_tail = 0x16;
        }
public:
        unsigned char m_chksum; //У����
        unsigned char m_tail;//֡β
};

//////////////////////////////////////////////////////////////////////////

#endif//_DEFINE_101_H
