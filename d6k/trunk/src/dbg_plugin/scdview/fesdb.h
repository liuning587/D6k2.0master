#pragma once
#include "scd_data.h"
#include <string>
#include <unordered_map>
#include <QObject>
#include <QString>


struct  HEAD_MEM;
struct  VARDATA;
class CFesDBR//:public CMemDB
{
public:
	CFesDBR()
	{
		m_pMgr = std::make_shared<CScdChannelMgr>();
	}
	~CFesDBR(){}
public:
	//构建内存关系
	bool LoadFesMemDB(const char* pAddr,int nSize);
	//获取内存关系
	std::shared_ptr<CScdChannelMgr >& GetScdMemRel()
	{
		return m_pMgr;
	}
	std::vector<CHANNEL*>& GetChannelArrs()
	{
		return m_arrChannels;
	}
	CHANNEL* GetChannelByOccNo(int32u  nOccNo);
	DEVICE* GetDeviceByOccNo(int32u nOccNo);
	AIN* GetAinByOccNo(int32u nOccNo);
	DIN* GetDinByOccNo(int32u nOccNo);
	AOUT * GetAoutByOccNo(int32u nOccNo);
	DOUT* GetDoutByOccNo(int32u nOccNo);

	size_t GetUserVarCount() const
	{
		return m_nUserVarCount;
	}

	size_t GetSysVarCount() const
	{
		return m_nSysVarCount;
	}

	VARDATA * GetUserVarByIndex(int nIndex);
	
	VARDATA * GetSysVarByIndex(int nIndex);
	
protected:
	size_t CreateChannel(unsigned char* pAddr);
	size_t CreateDevice(unsigned char* pAddr);
	size_t CreateAin(unsigned char* pAddr);
	size_t CreateDin(unsigned char* pAddr);
	size_t CreateAout(unsigned char* pAddr);
	size_t CreateDout(unsigned char* pAddr);

	size_t CreateAinAlarm(unsigned char* pAddr);
	size_t CreateAinLimitAlarm(unsigned char* pAddr);
	size_t CreateDinAlarm(unsigned char* pAddr);
	size_t CreateDinLimitAlarm(unsigned char* pAddr);

	size_t CreateUserVars(unsigned char* pAddr);
	size_t CreateSysVars(unsigned char* pAddr);
protected:
	void LogMsg(const char * pszText, int nLevel);
private:
	//关系表
	std::shared_ptr<CScdChannelMgr > m_pMgr;

	std::string  m_szProjName;

	HEAD_MEM* m_pMagicMem;

	CHANNEL *m_pChannels;
	unsigned int m_nChannelCount;
	std::vector<CHANNEL*> m_arrChannels;  //! 记住地址，方便遍历
	std::unordered_map<int32u, CHANNEL*> m_mapChannels;

	DEVICE *m_pDevices;
	unsigned int m_nDeviceCount;
	std::vector<DEVICE*> m_arrDevices;
	std::unordered_map<int32u, DEVICE*> m_mapDevices;
	
	AIN *m_pAins;
	unsigned int m_nAinCount;
	std::vector<AIN*> m_arrAins;
	std::unordered_map<int32u, AIN*> m_mapAins;

	DIN *m_pDins;
	unsigned int m_nDinCount;
	std::vector<DIN*> m_arrDins;
	std::unordered_map<int32u, DIN*> m_mapDins;

	DOUT *m_pDouts;
	unsigned int m_nDoutCount;
	std::vector<DOUT*> m_arrDouts;
	std::unordered_map<int32u, DOUT*> m_mapDouts;

	AOUT *m_pAouts;
	unsigned int m_nAoutCount;
	std::vector<AOUT*> m_arrAouts;
	std::unordered_map<int32u, AOUT*> m_mapAouts;
	
	AIN_ALARM  *m_pAinAlarm;
	unsigned int m_nAinAlarmCount;
	std::vector<AIN_ALARM*> m_arrAinAlarms;
	std::unordered_map<int32u, AIN_ALARM*> m_mapAinAlarms;

	AIN_ALARM_LIMIT *m_pAinAlarmLimit;
	unsigned int m_nAinAlarmLimitCount;
	std::vector<AIN_ALARM_LIMIT*> m_arrAinAlarmLimits;
	std::unordered_map<int32u, AIN_ALARM_LIMIT*> m_mapAinAlarmLimits;

	DIN_ALARM *m_pDinAlarm;
	unsigned int m_nDinAlarmCount;
	std::vector<DIN_ALARM*> m_arrDinAlarms;
	std::unordered_map<int32u, DIN_ALARM*> m_mapDinAlarms;

	DIN_ALARM_LIMIT *m_pDinAlarmLimit;
	unsigned int m_nDinAlarmLimitCount;
	std::vector<DIN_ALARM_LIMIT*> m_arrDinAlarmLimits;
	std::unordered_map<int32u, DIN_ALARM_LIMIT*> m_mapDinAlarmLimits;

	VARDATA * m_pUserVars;
	unsigned int m_nUserVarCount;
	std::vector<VARDATA*> m_arrUserVars;
	std::unordered_map<int32u, VARDATA*> m_mapUserVars;

	VARDATA* m_pSysVars;
	unsigned int m_nSysVarCount;
	std::vector<VARDATA*> m_arrSysVars;
	std::unordered_map<int32u, VARDATA*> m_mapSysVars;
};

