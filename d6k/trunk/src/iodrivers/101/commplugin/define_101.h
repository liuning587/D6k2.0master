
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
	//装置基本属性
public:
	int m_nDeviceID;           //装置ID
	int m_nDeviceAddr;         //装置地址

public:
	int m_nACD;//
	int m_nFCB;                //固定帧格式中的上次FCB值
	int m_nDeviceState;        //装置处于通讯过程中的状态
	int m_nRunState;           //处于接收或发送状态
	bool m_bIsInited;          //初始化过程是否完成(复位，请求状态，总召，对时。。。)

							   //前置端状态参数
public:
	int m_nLastCallTime;       //上次总召时间
	int m_nLastSyncTime;       //上次对时时间
	int m_nErrorCount;         //当前错误数目
	int m_nLastHaltTime;       //挂起时间
	bool  m_bIsHalt;           //装置是否挂起
	int m_nReSendCount;        //重发次数

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

		//超过三次没收到，则重发
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


#define BUFFER_SIZE_101 264 //发送和接收缓冲区大小

#ifdef SOURTHSTATEGRID
enum
{
	DIRECTION_DOWN = 0,      //下行
	DIRECTION_UP = 1,        //上行
};
#else
enum
{
	DIRECTION_DOWN = 1,      //下行
	DIRECTION_UP = 0,        //上行
};
#endif


enum
{
        TYPE_E=1,
        TYPE_F=2,
        TYPE_V=4,
};

//链路控制域
class CONTROL
{
public:
        CONTROL()
        {
                
        }
public:
        //低4位(4-7)
        unsigned char  m_FUNCCODE:4;    //链路功能码
        //3
        unsigned char  m_FCV:1;            //FCV:1:FCB有效,0:FCB无效;//DFC:1:被控站不能接收后续报文       
        //2
        unsigned char  m_DATA:1;            //FCB:被控站通过判断FCB是否翻转来决定是否重发上一帧报文;//ACD:1:被控站有一级数据,0:有二级数据
        //1
        unsigned char  m_PRM:1;             //0:上行 1:下行
        //0
        unsigned char  m_Dir:1;             //传输方向位,0,1可配
};

//固定帧格式
class AFCI //fix
{
public:
        AFCI()
        {
                m_head = 0x10;
                m_tail = 0x16;
        }

public:
        unsigned char m_head;//帧头

        CONTROL        m_ctrl;         //链路控制域
        LINKADDR       m_addr;         //链路地址
        unsigned char  m_chksum;       //校验码

        unsigned char   m_tail;         //帧尾
};

//可变帧格式
class AVDU_HEAD //variable
{
public:
        AVDU_HEAD()
        {
                m_head1 = 0x68;
                m_head2 = 0x68;
        }
public:
        unsigned char m_head1;       //帧头
        unsigned char m_length1;     //链路控制域+链路地址+应用服务数据单元
        unsigned char m_length2;     //链路控制域+链路地址+应用服务数据单元
        unsigned char m_head2;       //帧头

        CONTROL        m_ctrl;       //链路控制域
		LINKADDR       m_addr;       //链路地址

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
        unsigned char m_chksum; //校验码
        unsigned char m_tail;//帧尾
};

//////////////////////////////////////////////////////////////////////////

#endif//_DEFINE_101_H
