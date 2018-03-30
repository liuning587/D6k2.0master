#pragma once

#include <QObject>
#include <QList>
#include <QMap>
#include <QSettings>
#include <memory>
#include <vector>
#include "datadefine.h"

struct SYSPARA;                //系统参数
struct UART;                   //串口参数
struct GPRS;                   //GPRS配置参数
struct GPS;                    //GPS配置参数
struct SMS;                    //SMS配置参数
struct PROTOCOL;               //规约配置参数
struct IND_ADDR_TAB;           //内部点表
struct RPT;                    //遥测点表…………


typedef std::shared_ptr<SYSPARA> SYSPARA_DEF;
typedef std::shared_ptr<UART> UART_DEF;
typedef std::shared_ptr<GPRS> GPRS_DEF;
typedef std::shared_ptr<GPS> GPS_DEF;
typedef std::shared_ptr<SMS> SMS_DEF;
typedef std::shared_ptr<PROTOCOL> PROTOCOL_DEF;
typedef std::shared_ptr<IND_ADDR_TAB> IND_ADDR_TAB_DEF;
typedef std::shared_ptr<RPT> RPT_DEF;


class CFciFile
{
public:
	CFciFile();
	~CFciFile();
public:
	static CFciFile&  GetInstance()
	{
		if (m_nGetInstance==0)
		{
			m_nGetInstance= new CFciFile;
		}
		return *m_nGetInstance;
	}
public:
	bool ReadPrgFile(const QString& fileName);
	void WritePrgFile(const QString& fileName);
public:
	int32u  GetSysNum() const 
	{
		return  m_nSysparaNum;
	}
	int32u  GetUartNum() const 
	{
		return m_nUartNum;
	}
	int32u  GetGprsNum() const
	{
		return m_nGprsNum;
	}
	int32u  GetGpsNum() const
	{
		return m_nGpsNum;
	}
	int32u  GetSmsNum() const
	{
		return m_nSmsNum;
	}
	int32u  GetProtocolNum() const
	{
		return m_nProtocolNum;
	}
	int32u  GetIndAddrTable() const
	{
		return m_nIndAddrTabNum;
	}
	int32u  GetYxNum() const
	{
		return m_nYXNum;
	}
	int32u  GetYCNum() const
	{
		return m_nYcNum;
	}
	int32u  GetYKNum() const
	{
		return m_nYkNum;
	}
	int32u  GetFixNum() const
	{
		return m_nFixNum;
	}
	int32u  GetFlNum() const
	{
		return m_nFlNum;
	}
	int32u  GetYPNum() const
	{
		return m_nYPnum;
	}
public:
	bool   GetSysParaInfo(SYSPARA_DEF& );
	bool   GetUartInfo(int nIndex,UART_DEF& );
	bool   GetGprsInfo(int nIndex,GPRS_DEF& );
	bool   GetGpsInfo (int nIndex, GPS_DEF& );
	bool   GetSmsInfo (int nIndex, SMS_DEF&);
	bool   GetProtocolInfo(int nIndex, PROTOCOL_DEF&);
	bool   GetIndAddrTabInfo(int nIndex, IND_ADDR_TAB_DEF&);
	bool   GetYXInfo(int nIndex, RPT_DEF&);
	bool   GetYcInfo(int nIndex, RPT_DEF&);
	bool   GetYKInfo(int nIndex, RPT_DEF&);
	bool   GetFixInfo(int nIndex, RPT_DEF&);
	bool   GetFFLInfo(int nIndex, RPT_DEF&);
	bool   GetYpInfo(int nIndex, RPT_DEF&);
public:
	bool   GetUartArryInfo(std::vector<UART_DEF>& vecs);
	bool   GetGprsArryInfo(std::vector<GPRS_DEF>& vecs);
	bool   GetGpsArryInfo(std::vector<GPS_DEF>& vecs);
	bool   GetSmsArryInfo(std::vector<SMS_DEF>& vecs);
	bool   GetProtocolArryInfo(std::vector<PROTOCOL_DEF>& vecs);
	bool   GetIndAddrTabArryInfo(std::vector<IND_ADDR_TAB_DEF>& vecs);
	bool   GetYXArryInfo(std::vector<RPT_DEF>& vecs);
	bool   GetYcArryInfo(std::vector<RPT_DEF>& vecs);
	bool   GetYKArryInfo(std::vector<RPT_DEF>& vecs);
	bool   GetFixArryInfo(std::vector<RPT_DEF>& vecs);
	bool   GetFFLArryInfo(std::vector<RPT_DEF>& vecs);
	bool   GetYpArryInfo(std::vector<RPT_DEF>& vecs);
private:
	bool   ParaSysInfo(const QString& filename);
	bool   ParaUartInfo(const QString& filename);
	bool   ParaGPRSInfo(const QString& filename);
	bool   ParaGpsInfo(const QString& filename);
	bool   ParaSmsInfo(const QString& filename);
	bool   ParaProtocolInfo(const QString& filename);
	bool   ParaIndAddrTabInfo(const QString& filename);
	bool   ParaYCInfo(const QString& filename);
	bool   ParaYXInfo(const QString& filename);
	bool   ParaYKInfo(const QString& filename);
	bool   ParaFixInfo(const QString& filename);
	bool   ParaFLInfo(const QString& filename);
	bool   ParaYPInfo(const QString& filename);
private:
	int32u m_nSysparaNum;	
	std::vector<SYSPARA_DEF> m_arrSysParas;

	int32u m_nUartNum;	
	std::vector<UART_DEF> m_arrUartds;

	int32u m_nGprsNum;
	std::vector<GPRS_DEF> m_arrGprss;

	int32u m_nGpsNum;	
	std::vector<GPS_DEF> m_arrGpss;

	int32u m_nSmsNum;	
	std::vector<SMS_DEF> m_arrSmss;

	int32u m_nProtocolNum;
	std::vector<PROTOCOL_DEF> m_arrProtocols;

	int32u m_nIndAddrTabNum;
	std::vector<IND_ADDR_TAB_DEF> m_arrIndTabs;

	int32u m_nYcNum;
	std::vector<RPT_DEF> m_arrYcs;

	int32u m_nYXNum;
	std::vector<RPT_DEF> m_arrYxs;

	int32u m_nYkNum;
	std::vector<RPT_DEF> m_arrYks;

	int32u m_nFixNum;
	std::vector<RPT_DEF> m_arrFixs;

	int32u m_nFlNum;
	std::vector<RPT_DEF> m_arrFfls;

	int m_nYPnum;
	std::vector<RPT_DEF> m_arrYps;
private:
	static CFciFile* m_nGetInstance;
};

