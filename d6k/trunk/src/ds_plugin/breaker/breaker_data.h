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
	TREE_ITEM_DEBUG,      //调试信息
	TREE_ITEM_BREAKER_ANALOG,   //测量值
	TREE_ITEM_PROTECT_DEV,     //保护定值
	TREE_ITEM_SYSTEM_DEV,    //系统参数
	TREE_ITEM_DI_IFNO,      //开入量
	TREE_INFO_DO_INFO,      //开出量
	TREE_INFO_SOFT,          //软压板
	TREE_INFO_SOE,          //soe

	//实时
    TREE_ITEM_REALTIME_DI,   //开入
	TREE_ITEM_REALTIME_DO,   //开出
	TREE_ITEM_REALTIME_ACTION,  //动作
	TREE_ITEM_REALTIME_DISNORMAL, //异常
	TREE_ITEM_REALTIME_SOFT,      //软压板
	//历史soe
	TREE_ITEM_HIOSTORY_SOE,   //历史OSE
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



/* 1. 数值类型段 */
enum ANALOG_DATA_TYPE
{
	MEASURE_PARAM_INT = 0x00,			///< 有符号整数数
	MEASURE_PARAM_UINT = 0x01,			///< 无符号整数数
	MEASURE_PARAM_HEX = 0x02,			///< HEX数
	MEASURE_PARAM_FLOAT = 0x03,			///< 浮点数
	MEASURE_PARAM_BIT = 0x04,			///< 按位表示的数
	MEASURE_PARAM_BOOL = 0x05,			///< BOOL值

};


#define FLAG_DEVSTATUS_SOE           0x01   //有新SOE事件产生
#define FLAG_DEVSTATUS_RUN           0x02   //运行态
#define FLAG_DEVSTATUS_REMOTE        0x04   //远方态
#define FLAG_DEVSTATUS_LOCKSTATUS    0x08   //保护闭锁状态
#define FLAG_DEVSTATUS_ACTSTATUS     0x10   //保护动作状态


enum {
	SOE_ACTION,				/*!< 保护动作事件 */
	SOE_DIEVENT,			    /*!< 开入变位事件 */
	SOE_ABNORMAL,			   /*!< 异常事件 */
	SOE_RUN,				   /*!< 运行事件 */
	SOE_WAVE_FILE,		   /*!< 录波事件 */
	SOE_TRACE,			   /*!< 调试信息 */
	SOE_TYPE_COUNT
};

enum 
{
	DI_SOE_TYPE_REAL_DI,		//实际开入 － （物理开入/内部开入）
	DI_SOE_TYPE_REAL_DO,		//实际开出 － （物理开出/内部开出）
	DI_SOE_TYPE_VIRPT			//虚拟点
};


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

// 4字节
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

	unsigned char ms;     //秒
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
	DBG_MSG2 year;    //年份
	unsigned char month;   //月份
	unsigned char day;     //日期
	unsigned char hour;    //小时
	unsigned char minute;  //分钟

	unsigned char ms;     //毫秒
	DBG_INFO4 mmins;      //毫秒

public:
	QString Dump()
	{
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.4d", year.GetAddr(), month, day, hour, minute, ms,mmins.GetAddr());
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

//四字节通用格式
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

//两字节通用格式
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

//软压板
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
	//soe类型
	DBG_MSG2 m_SoeType;
	//soe暂未用
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
	//soe类型
	unsigned char m_nType;
	//soe时间
	CPTimeMs m_Time;
};

//soe action info
class SOE_ACTION_INFO : public SOE_INFO_HEADER
{
public:
	//soe事件类型
	unsigned char m_cSoeType;
	//动作状态
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

//soe数量
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

//变位soe
class SOE_IO_INFO : public SOE_INFO_HEADER
{
public:

	//遥信类型
	unsigned char m_CBinaryType;
	//实际ID
	unsigned char m_cID;
	//变位类型
	unsigned char m_cChangeType;

};

//异常 运行 录波 SOE
class SOE_ID_INFO : public SOE_INFO_HEADER
{
public:
	//ID
	unsigned char m_cType;
};


//设置时间报文
class TIME_SET_INFO : public DBG_HEADER
{
public:
	//time
	CP56Time2a m_time;
};

//设置返回报文
class TIME_SET_ACK_INFO : public DBG_HEADER
{
public:
	//code
	unsigned char m_cCode;
	//操作结果
	unsigned char m_Result;
	//附件结果
	unsigned char m_AddResult;
};


//遥控
class REMOTE_CONTROM_SET :public DBG_HEADER
{
public:
	//控制类型 
	unsigned char m_ControlType;
	//控制对象'
	unsigned char m_ControlObj;
	//控制命令
	unsigned char m_Order;
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

//定值
class CDevInfo
{
public:
	//序号
	int m_nPointNum;
	//定值组 分类
	QString m_strDevType;
	//定值名称
	QString m_strDevName;
	//单位
	QString m_strDevUnit;
	//定值范围
	QString m_strDevRange;
	//当前值
	int m_strValue;
};

//开入开出参数
class CDIDOInfo
{
public:
	//序号
	int m_nPointNum;
	//名称
	QString m_nName;
	//值
	int m_nValue;
};

//软压板定值
class CSOFTSTRAP
{
public:
	//序号
	int m_nPointNum;
	//内部id
	int m_nOwnId;
	//名称
	QString m_strName;
	//值
	int m_nVlaue;
};

//事件传输 统计信息信息
class SOE_EVENT_STATIC
{
public:
	enum
	{
		SOE_SEO_NULL,
		SOE_GET_NUMBER,   // 获取条目数
		SOE_GET_ACTION,       //获取action
		SOE_GET_DIEVENT,      //获取Di
		SOE_GET_ABNORMAL,    //获取异常
		SOE_GET_RUN,        //获取run
		SOE_GET_WAVE,        //获取录波
		SOE_GET_TRACE,       //获取trace
	};

	SOE_EVENT_STATIC()
	{
		m_nCurrentStatic = SOE_SEO_NULL;

		//action总条目数
		m_nActionTotalNum = 0;
		//aciton当前条目
		m_nActionCurrentNum = 0;

		//di总条目数
		m_nDiTotalNum = 0;
		//di当前条目数
		m_nDiCurrentNum = 0;

		//abnormal 总数
		m_nAbnormalTotalNum = 0;
		//abnormal   当前条目
		m_nAbnoramlCurrentNum = 0;

		//run 总条数
		m_nRunTotalNum = 0;
		//run 当前条目数
		m_nRunCurrentNum = 0;

		//wave总条
		m_nWaveTotaolNum = 0;
		//wave当前条目
		m_nVaveCurrentNum = 0;

		//tracel
		m_nTraceTotalNum = 0;
		//tarce
		m_nTraceCurrentNum = 0;
	}

	void ClearStatic()
	{
		m_nCurrentStatic = SOE_SEO_NULL;

		//当前所处的状态
		m_nCurrentStatic = 0;

		//action总条目数
		m_nActionTotalNum = 0;
		//aciton当前条目
		m_nActionCurrentNum = 0;

		//di总条目数
		m_nDiTotalNum = 0;
		//di当前条目数
		m_nDiCurrentNum = 0;

		//abnormal 总数
		m_nAbnormalTotalNum = 0;
		//abnormal   当前条目
		m_nAbnoramlCurrentNum = 0;

		//run 总条数
		m_nRunTotalNum = 0;
		//run 当前条目数
		m_nRunCurrentNum = 0;

		//wave总条
		m_nWaveTotaolNum = 0;
		//wave当前条目
		m_nVaveCurrentNum = 0;

		//tracel
		m_nTraceTotalNum = 0;
		//tarce
		m_nTraceCurrentNum = 0;
	}
public:
	//当前所处的状态
	int m_nCurrentStatic;

	//action总条目数
	int m_nActionTotalNum;
	//aciton当前条目
	int m_nActionCurrentNum;

	//di总条目数
	int m_nDiTotalNum;
	//di当前条目数
	int m_nDiCurrentNum;

	//abnormal 总数
	int m_nAbnormalTotalNum;
	//abnormal   当前条目
	int m_nAbnoramlCurrentNum;

	//run 总条数
	int m_nRunTotalNum;
	//run 当前条目数
	int m_nRunCurrentNum;

	//wave总条
	int m_nWaveTotaolNum;
	//wave当前条目
	int m_nVaveCurrentNum;

	//tracel
	int m_nTraceTotalNum;
	//tarce
	int m_nTraceCurrentNum;

public:

	
};