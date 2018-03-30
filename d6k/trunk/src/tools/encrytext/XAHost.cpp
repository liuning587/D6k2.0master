//#include "stdafx.h"
#include "XAHost.h"

#include "./include/TZhuZhanDll.h"

#pragma comment (lib, "TZhuZhanDll.lib")


CXAHost * CXAHost::m_pSingleControl = nullptr;

CXAHost::CXAHost()
{
    this->m_HostHandle = -1;
	m_nKeyIndex = 1;
	m_nDivTimes = 1;

}

CXAHost * CXAHost::GetInstance()
{
	if (m_pSingleControl == nullptr)
	{
		m_pSingleControl = new CXAHost;
	}

	return m_pSingleControl;
}

CXAHost::~CXAHost()
{
    if (this->m_HostHandle > 0)
    {
        this->DisConnect();
    }

    this->m_HostHandle = -1;
}

int CXAHost::ConnectHSM(char* hostIP, int hostPort, int timeout)
{
	strncpy_s(this->m_hostIP, hostIP, 128);
	this->m_hostPort = hostPort;

    this->m_HostHandle = ::ConnectHSM(this->m_hostIP, this->m_hostPort, timeout);

    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return this->m_HostHandle;
}

int CXAHost::DisConnect()
{
    int result;

    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    result = ::DisConnect(this->m_HostHandle);

    this->m_HostHandle = -1;

    return result;
}

int CXAHost::GenRand(int sRandLen, uint8_t * random)
{
    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return ::GenRand(this->m_HostHandle, sRandLen, random);
}

int CXAHost::InternalSign(int keyIndex, int inDataLen, uint8_t *inData, uint8_t *sData, int *sDataLen)
{
    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return ::InternalSign(this->m_HostHandle, keyIndex, inDataLen, inData, sData, sDataLen);
}

int CXAHost::VerSign(int cerLen, uint8_t* cer, int oriDataLen, uint8_t* oriData, int sDataLen, uint8_t* sData)
{
    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return ::VerSign(this->m_HostHandle, cerLen, cer, oriDataLen, oriData, sDataLen, sData);
}

int CXAHost::EnData(int plainDataLen, uint8_t * plainData, uint8_t * enData, int * enDataLen)
{
    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return ::EnData(this->m_HostHandle, this->m_nKeyVersion, this->m_nKeyIndex, this->m_nDivTimes, this->m_arrDivData, this->m_arrIvData, plainDataLen, plainData, enData, enDataLen);
}

int CXAHost::DeData(int enDataLen, uint8_t * enData, uint8_t * plainData, int * plainDataLen)
{
    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return ::DeData(this->m_HostHandle, this->m_nKeyVersion, this->m_nKeyIndex, this->m_nDivTimes, this->m_arrDivData, this->m_arrIvData, enDataLen, enData, plainData, plainDataLen);
}

int CXAHost::GetUpdateKey(int oldKeyVersion, int NewKeyVersion, int divTimes, uint8_t * divData, uint8_t * ivData, int asKid, uint8_t * updateKeyData, int * olen)
{
    if (this->m_HostHandle <= 0)
    {
        return -1;
    }

    return ::GetUpdateKey(this->m_HostHandle, oldKeyVersion, NewKeyVersion, divTimes, divData, ivData, asKid, updateKeyData, olen);
}


unsigned char CXAHost::checkAllData(const unsigned char *data, int length)
{
	unsigned char lValue = 0;
	for (int i = 0; i < length; i++)
	{
		lValue += data[i];
	}
	return lValue;
}

void CXAHost::SetCellIndex(char * pIndex, int nLength)
{
	strncpy((char*)this->m_arrDivData, pIndex, nLength);
}

void CXAHost::SetVersionIndex(int nIndex)
{
	this->m_nKeyVersion = nIndex;
}

void CXAHost::SetOrignialVer(char * pIvData)
{
	strncpy((char*)this->m_arrIvData, pIvData, 8);
	for (int i=0; i<8; i++)
	{
		this->m_arrIvData[8 + i] = ~this->m_arrIvData[i];
	}
}
