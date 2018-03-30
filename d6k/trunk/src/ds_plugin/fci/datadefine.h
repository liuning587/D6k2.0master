#ifndef  _DATA_DEFINE_H
#define  _DATA_DEFINE_H

#include "datatypes.h"
#include <QString>

#define  MAX_TELEPHONE_NUM  8
#define  MAX_NAME_LENGTH   32
#define  MAX_DES_LENGTH     128
//ϵͳ��������
struct SYSPARA  
{
	int8u  StationAddr;        //��վ��ַ �������ַ
	int8s  IsSaveSoe;          //������ʷSOE 0-������ 1-����;
	int32u SoeNum;             //����SOE����
	int8s  IsSaveYCData;       //������ʷң������ 0-������ 1-����
	int32u SaveYCFRQ;          //����YCƵ�� ��λ����
	int32u SaveYcDays;         //����YC���� ��λ����
	int8s  DataType;           //�������� �������ͣ�0-��һ��ֵ 1-��Ȼ�ֵ;
	int32u UpLoadYCFRQ;        //����ң��ֵƵ��
	int32u CallYCFRQ;          //�ٻ�ң��ֵƵ��
	int32u SyncFRQ;            //��ʱƵ��
	int32u GPRSTimeOut;        //GPRS��ʱ
	int32u VBatLimit;          //��ص�ѹ��������
	int32u LbFileNum;          //¼���ļ�����
	int32u ComNum;             //��������
	int32u NetNum;             //��������
	int32u IndNum;             //ָʾ������

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

//��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//format = COM_ID, UseType, bps, verify:0 - None 1 - Odd 2 - Even, data bit, stop bit, protocolID;///////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UART 
{
	int32u OccNO;
	int8u  ComID;              //����ID 0 - 4 ������Ч//�̶���      
	int8u  UseType;            //1-debug,2-Com,3-GPS,4-GPRS_COM,5-RF433 //�̶���
	int32u Band;               //������
	int8u  ParityBit;          //��żλ
	int8u  DataBit;            //����λ��Ĭ��Ϊ8��������Ч
	int8u  StopBit;            //ֹͣλ��Ĭ��Ϊ0��������Ч
	int8u  ProtocolID;         //��Լ��
};

//GPRS���ò���
////////////////////////////////////////////////////////////////
//format=mode,IP1:port1,IP2:port2,APN,USER,PWD,protocolID;//////
////////////////////////////////////////////////////////////////
struct GPRS
{
	int32u    OccNO;
	int8s     WorkMode;                 //����ģʽ
	QString   IPA;                      //A����ַ
	int32u    PortA;                    //A���˿�
	QString   IPB;                      //B����ַ
	int32u    PortB;                    //B���˿�
	QString   APN;     //APN
	QString   UserName;//�û�����
	QString   UserPwd; //�û�����
	int8s     ProtocolID;               //��ԼID
};

//GPS���ò���
////////////////////////////////////////////////////////////////////////
//format=enable, sys_interval(5-72*60min),max_recive_time(1-10min)//////
////////////////////////////////////////////////////////////////////////
struct GPS
{
	int32u OccNO;
	int8s  Enable;                   //GPSʹ�� ����1 ������0
	int32u SysInterval;              //ͬ��ʱ����  ��
	int32u MaxRecvTime;              //��������Ч����ʱ�� ��
};

//SMS���ò���
struct SMS
{
	int32u OccNO;

	QString StatationNum;    //��վ����
	QString StatationNumDesc;    //��վ��������    
};

//��Լ���ò���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//format=name,link_addr_len,cot_len, comm_len,info_addr_len,link_addr,comm_addr,resendtimes,timeout1,timeout2,timeout3,timeout4;/////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PROTOCOL
{
	int32u OccNO;

	QString ProtocolName;  //Э������
	int8s ProtocolID;                     //Э���
	int8s LinkAddrLen;                    //��·��ַ���� 1-4
	int8s CotLen;                         //����ԭ�򳤶� 1/2
	int8s CommLen;                        //��·��ַ���� 2�̶�
	int8s InfoAddrLen;                    //��Ϣ��ַ���� 2�̶�       

	int32u LinkAddr;                      //��·��ַ 
	int32u CommonAddr;                    //������ַ

	int8s ResendTimes;                    //�ط�����
	int8s T1;
	int8s T2;
	int8s T3;
	int8s T4;
};

//�ڲ����
//ע���һ��0 �������Լ� ����Ϊ����ͨѶ
struct IND_ADDR_TAB
{
	int32u OccNO;

	int8s    GrpNo;
	QString  Address1;   //0ֻʹ�õ�һ��ַ
	QString  Address2;
	QString  Address3;
	QString  Desription;
};

//ң�ŵ��
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
//ң����,��ʹ��ͬһ�����
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
//ң�ص��

/*
struct RPT
{
	int32u OccNO;
	QString  InnerAddr;
	QString  Address;
	QString  Desccription;
};*/
//ң������˴�����Ҫ����
struct FYM;  

//��ֵ��������

/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Desccription;
};*/
//�˲���ֵ

/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Desccription;
};*/
//ң��

/*
struct RPT
{
	int32u OccNO;

	QString InnerAddr;
	QString Address;
	QString  Desccription;
};*/




#endif
