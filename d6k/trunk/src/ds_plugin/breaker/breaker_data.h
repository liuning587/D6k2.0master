#pragma once

#include <QString>
#include <QMap>

enum 
{
	DBG_HEADER0 = 0xAA,
	DBG_HEADER1 = 0x55,
};

//每帧报文最大长度
enum
{
	MAX_DBG_SIZE = 2048,
};

//树形结构item编号
enum
{
	TREE_ITEM_BREAKER,    //直流断路器
	TREE_ITEM_BREAKER_ANALOG,   //测量值
};

typedef unsigned short  BIT16;  //16位
typedef unsigned long   BIT32;  //32位
typedef unsigned char   BIT08;  //08位


#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))
#define MYMAKE32(l, h)      ((BIT32)(((BIT16)(((BIT32)(l)) & 0xffff)) | ((BIT32)((BIT16)(((BIT32)(h)) & 0xffff))) << 16))



#define DBG_CODE_GET_SYS_INFO      1 //获取系统信息
#define DBG_CODE_GET_MEAS          2 //获取测量值信息
#define DBG_CODE_GET_DEBUG_DATA    3 //获取调试值信息
#define DBG_CODE_GET_SETTING       4 //获取保护定值
#define DBG_CODE_GET_SYSCFG        5 //获取系统参数
#define DBG_CODE_GET_DICFG         6 //获取开入参数
#define DBG_CODE_GET_DOCFG         7 //获取开出参数
#define DBG_CODE_GET_SOFTSTRAP     8 //获取软压板定值

#define DBG_CODE_GET_SOE           40 //获取事件信息

#define DBG_CODE_SET_TIME          10 //设置时间
#define DBG_CODE_SET_SETTING       11 //设置保护定值
#define DBG_CODE_SET_SYSCFG        12 //设置系统参数
#define DBG_CODE_SET_DICFG         13 //设置开入参数
#define DBG_CODE_SET_DOCFG         14 //设置开出参数
#define DBG_CODE_SET_SOFTSTRAP     15 //设置软压板定值

#define DBG_CODE_DBG_FORCE           20//强制测点
#define DBG_CODE_DBG_UNFORCE         21//解除强制
#define DBG_CODE_DBG_GENSOE          22//生成事件SOE
#define DBG_CODE_DBG_START_WAVE      23//调试产生录波

#define DBG_CODE_RM_CTRL             30//遥控



//报文长度   2字节
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

//测量值   4字节
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

//CP56Time2a时间结构
struct CP56Time2a
{
	DBG_MSG2 year;    //年份
	unsigned char month;   //月份
	unsigned char day;     //日期
	unsigned char hour;    //小时
	unsigned char minute;  //分钟

	unsigned char ms;     //毫秒
		
public:
	QString Dump()
	{
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", year.GetAddr(), month, day, hour, minute, ms);
		return buf;
	}
};

//报文头信息
class DBG_HEADER
{
public:
	unsigned char header0;   //起始头0
	unsigned char header1;   //起始头1
	DBG_MSG2 msgLeg;   //报文长度
	unsigned char type;      //类型标识
};

//获取的系统信息
class DBG_GET_SYS_INFO : public DBG_HEADER
{
public:
	unsigned char version;  //软件版本
	CP56Time2a time;               //编译时间
};

//测量值
class DBG_GET_MEAS : public DBG_HEADER
{
private:
	class MEAS_DATA //离散的信号量
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

//初始化信息  ip和端口
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
	//ip地址
	QString m_strIpAddress;
	//端口
	int m_nPort;
};


//点表信息
//测量值
class CAnalogPointInfo
{
public:
	//序号
	int m_nPointNum;
	//分类
	QString m_strType;
	//侧值名称
	QString m_strName;
	//单位
	QString m_strUnit;
	//侧值类型
	int m_nValueType;
	//小数点后几位
	int m_nSmallNum;
	//值
	QString m_strValue;
};
