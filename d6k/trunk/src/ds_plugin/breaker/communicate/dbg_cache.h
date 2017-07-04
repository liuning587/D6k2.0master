#pragma once


#include <string.h>


class CDbgCache
{
	enum
	{
		MAX_DBG_LEN = 2048,
	};

private:
	char m_buf[MAX_DBG_LEN];
	int  m_nCurPos;

public:
	CDbgCache()
	{
		m_nCurPos = 0;
	}

public:
	char* GetDataBuff();
	int GetDataLen();

public:
	int GetCurByteLength();
	int GetNeedByteLength();

public:
	void CopyData(char* pBuff, int nLen);
	bool IsEmpty();
	void Clear();
};