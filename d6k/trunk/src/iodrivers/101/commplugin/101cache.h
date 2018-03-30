
#ifndef _101_CACHE_H
#define _101_CACHE_H

#include "define_101.h"

class C101Cache
{
        enum
        {
                MAX_AVDU_BUFFER_LEN = 256,
        };
private:
        char m_buf[MAX_AVDU_BUFFER_LEN];
        int  m_nCurPos;
public:
        C101Cache()
        {
                m_nCurPos = 0;
        }
public:
        char* GetDataBuff();
        int     GetDataLen();
public:
        int GetCurByteLength();
        int GetNeedByteLength();
public:
        int GetFrameType();
        char* FindStartChar(char* pBuff,int nLen);
public:
        void CopyData(char* pBuff,int nLen);
        bool IsEmpty();
        void Clear();
};

#endif //_AVDU_CACHE_H