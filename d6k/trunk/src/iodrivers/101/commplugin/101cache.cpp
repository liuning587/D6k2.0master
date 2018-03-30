#include "101cache.h"
//#include "mylib.h"

char* C101Cache::GetDataBuff()
{
        return m_buf;
}
int     C101Cache::GetDataLen()
{
        int nLen = 0;
        switch (m_buf[0])
        {
        case 0xE5:
                {
                        nLen = 1;
                }
                break;
        case 0x10:
                {
                        nLen = sizeof(AFCI);
                }
                break;
        case 0x68:
                {
                        unsigned char lengthByte = m_buf[1];
                        nLen = lengthByte+2+4;
                }
                break;
        }
        
        return nLen;
}

int C101Cache::GetCurByteLength()
{
        return m_nCurPos;
}
int C101Cache::GetNeedByteLength()
{
        int nDataLen = GetDataLen();
        if(nDataLen <= 0)
                return 0;

        if(nDataLen > MAX_AVDU_BUFFER_LEN)
                nDataLen = MAX_AVDU_BUFFER_LEN;

        return nDataLen - m_nCurPos;
}
int C101Cache::GetFrameType()
{
        switch (m_buf[0])
        {
        case 0xE5:
                return TYPE_E;
        case 0x10:
                return TYPE_F;
        case 0x68:
                return TYPE_V;
                break;
        }
        return 0;
}

char* C101Cache::FindStartChar(char* pBuff,int nLen)
{       
        for(int i=0;i<nLen;i++)
        {
                char* pNewBuff = pBuff+i;
                if( *pNewBuff  == 0xE5)
                        return pNewBuff;
                else if( *pNewBuff  == 0x10)
                        return pNewBuff;
                else if( *pNewBuff  == 0x68)
                        return pNewBuff;
        }
        return NULL;
}

void C101Cache::CopyData(char* pBuff,int nLen)
{
        char* pFrom = m_buf + m_nCurPos;

        memcpy(pFrom,pBuff,nLen);

        m_nCurPos  =  m_nCurPos + nLen;
}
bool C101Cache::IsEmpty()
{
        if(m_nCurPos == 0)
                return true;
        return false;
}
void C101Cache::Clear()
{
        m_nCurPos = 0;
}
