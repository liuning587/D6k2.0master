#ifndef  _DATA_DEFINE_H
#define  _DATA_DEFINE_H

#include "datatypes.h"
#include <QString>

#define  MAX_TELEPHONE_NUM  8
#define  MAX_NAME_LENGTH   32
#define  MAX_DES_LENGTH     128
//系统参数配置
struct SYSPARA  
{
	int8u  StationAddr;        //厂站地址 管理机地址
	int8s  IsSaveSoe;          //保存历史SOE 0-不保存 1-保存;
	int32u SoeNum;             //保存SOE数量
	int8s  IsSaveYCData;       //保存历史遥测数据 0-不保存 1-保存
	int32u SaveYCFRQ;          //保存YC频率 单位：分
	int32u SaveYcDays;         //保存YC天数 单位：天
	int8s  DataType;           //数据类型 数据类型：0-归一化值 1-标度化值;
	int32u UpLoadYCFRQ;        //上送遥测值频率
	int32u CallYCFRQ;          //召唤遥测值频率
	int32u SyncFRQ;            //对时频率
	int32u GPRSTimeOut;        //GPRS超时
	int32u VBatLimit;          //电池电压报警门限
	int32u LbFileNum;          //录波文件数量
	int32u ComNum;             //串口数量
	int32u NetNum;             //网卡数量
	int32u IndNum;             //指示器数量

	QString  StationAddrDesc;
	QString  IsSaveSoeDesc;
	QString  SoeNumDesc;
	QString  IsSaveYCDataDesc;
	QString  SaveYCFRQDesc;
	QString  SaveYcDaysDesc;
	QString  DataTypeDesc;
	QString  UpLoadYCFRQDesc;
	QString  CallYCFRQDesc;
	QString  SyncFRQDesc;
	QString  GPRSTimeOutDesc;
	QString  VBatLimitDesc;
	QString  LbFileNumDesc;
	QString  ComNumDesc;
	QString  NetNumDesc;
	QString  IndNumDesc;
};

//串口配置
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//format = COM_ID, UseType, bps, verify:0 - None 1 - Odd 2 - Even, data bit, stop bit, protocolID;///////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UART 
{
	int32u OccNO;
	int8u  ComID;              //串口ID 0 - 4 其他无效//固定的      
	int8u  UseType;            //1-debug,2-Com,3-GPS,4-GPRS_COM,5-RF433 //固定的
	int32u Band;               //波特率
	int8u  ParityBit;          //奇偶位
	int8u  DataBit;            //数据位，默认为8，其他无效
	int8u  StopBit;            //停止位，默认为0，其他无效
	int8u  ProtocolID;         //规约号
};

//GPRS配置参数
////////////////////////////////////////////////////////////////
//format=mode,IP1:port1,IP2:port2,APN,USER,PWD,protocolID;//////
////////////////////////////////////////////////////////////////
struct GPRS
{
	int32u    OccNO;
	int8s     WorkMode;                 //工作模式
	QString   IPA;                      //A网地址
	int32u    PortA;                    //A网端口
	QString   IPB;                      //B网地址
	int32u    PortB;                    //B网端口
	QString   APN;     //APN
	QString   UserName;//用户名称
	QString   UserPwd; //用户密码
	int8s     ProtocolID;               //规约ID
};

//GPS配置参数
////////////////////////////////////////////////////////////////////////
//format=enable, sys_interval(5-72*60min),max_recive_time(1-10min)//////
////////////////////////////////////////////////////////////////////////
struct GPS
{
	int32u OccNO;
	int8s  Enable;                   //GPS使能 启用1 不启用0
	int32u SysInterval;              //同步时间间隔  分
	int32u MaxRecvTime;              //最大接受有效数据时间 秒
};

//SMS配置参数
struct SMS
{
	int32u OccNO;

	QString StatationNum;    //主站号码
	QString StatationNumDesc;    //主站号码描述    
};

//规约配置参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//format=name,link_addr_len,cot_len, comm_len,info_addr_len,link_addr,comm_addr,resendtimes,timeout1,timeout2,timeout3,timeout4;/////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PROTOCOL
{
	int32u OccNO;

	QString ProtocolName;  //协议名称
	int8s ProtocolID;                     //协议号
	int8s LinkAddrLen;                    //链路地址长度 1-4
	int8s CotLen;                         //传输原因长度 1/2
	int8s CommLen;                        //链路地址长度 2固定
	int8s InfoAddrLen;                    //信息地址长度 2固定       

	int32u LinkAddr;                      //链路地址 
	int32u CommonAddr;                    //公共地址

	int8s ResendTimes;                    //重发次数
	int8s T1;
	int8s T2;
	int8s T3;
	int8s T4;
};

//内部点表
//注意第一行0 代表是自己 其他为对下通讯
struct IND_ADDR_TAB
{
	int32u OccNO;

	int8s    GrpNo;
	QString  Address1;   //0只使用第一地址
	QString  Address2;
	QString  Address3;
	QString  Desription;
};

//遥信点表
//////////////////////////////////////////////////////////////////
////format=interaddr,	addr,		name,			desc;/////////
//////////////////////////////////////////////////////////////////
/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Name;
	QString  Desccription;
};*/
//遥测点表,均使用同一个点表
struct RPT
{
	int32u OccNO;
	QString InnerAddr;
	QString Address;

	QString  Desccription;
	QString Name;

	fp64   MinValue;
	fp64   MaxValue;
	fp64   KRate;
	QString  Unit;
};
//遥控点表

/*
struct RPT
{
	int32u OccNO;
	QString  InnerAddr;
	QString  Address;
	QString  Desccription;
};*/
//遥脉点表，此处不需要定义
struct FYM;  

//定值参数配置

/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Desccription;
};*/
//滤波定值

/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Desccription;
};*/
//遥调

/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Desccription;
};*/




#endif
