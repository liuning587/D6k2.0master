#pragma once

/*! @file tag.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  tag.h
�ļ�ʵ�ֹ��� :  ��㶨��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��㶨��
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef TAG_DEF_H
#define TAG_DEF_H


#include "datatypes.h"
//#include "fesapi/fesdatadef.h"
#include "log/log.h"
#include <string>
#include <memory>

template<typename T>
class CVar
{
public:
	CVar(std::string strTagName) : m_strTagName(strTagName)
	{
		m_pOccNoItem = nullptr;
	}

	virtual ~CVar()
	{

	}
public:
	T GetValue() const
	{
		return m_Value;
	}
	// �����OccNo����ʼ��ʱ����
	virtual bool GetOccNoByTagName() = 0;
	// ʵʱ��ȡʵ��ֵ����ʱѭ������
	virtual bool GetRealTimeData() = 0;

	void LogMsg(const char *szLogTxt, int nLevel)
	{
		::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
	}

protected:
	T   m_Value;       //!  ��ֵ

					   //struct TAGITEM  m_Item;
	std::shared_ptr<struct TAGITEM > m_pOccNoItem;

	std::string m_strTagName;

	std::string m_szDescription;
};

class CDinData :public CVar<int8u>
{
public:
	CDinData(std::string m_szTagName);
	~CDinData();
public:
	virtual bool GetRealTimeData();
	virtual bool GetOccNoByTagName();
private:

};

class CAinData :public CVar<fp64>
{
public:
	CAinData(std::string strTagName);

	~CAinData();

public:

	bool GetData(fp64* value)
	{
		if (!GetRealTimeData())
		{
			return false;
		}

		*value = GetValue();
		return true;
	}

	virtual bool GetRealTimeData();
	virtual bool GetOccNoByTagName();
	
};


#endif // TAG_DEF_H

/** @}*/


