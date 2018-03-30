#pragma once

#include <stdint.h>


class CXAHost
{
private:
	//���Ӿ��
    int     m_HostHandle;
    char    m_hostIP[128];
    int     m_hostPort;
public:
	//��ǰ�ն���Կ�汾
	int m_nKeyVersion;
	//��Կ����
	int m_nKeyIndex;
	//��ɢ����
	int m_nDivTimes;
	//��ɢ���� (�ն�оƬ���к�)
	unsigned char m_arrDivData[8];
	//��ʼ����
	unsigned char m_arrIvData[16];


	CXAHost();
	static CXAHost *m_pSingleControl;


public:

	static CXAHost *GetInstance();

    /*
    ���ܣ� ���캯��
    ��Σ�
    char *cHostIp,���ܻ�IP��ַ���ַ���������"192.168.1.12"��ʹ�ö����������ֵ��������
    int inPort,���ܻ��˿ڣ�ʹ�ö����������ֵ������
    */

    ~CXAHost();

public:
    /*
    ���ܣ� ��������ܻ�����������������
    ��Σ�
    int timeout,��ʱʱ�䣨��λ���룩��ʹ�ö����������ֵ������
    ���Σ���
    ���أ�
    �ɹ����ض��������������0��С��0ʧ��
    */
    int ConnectHSM(char* hostIP, int hostPort, int timeout);


    /*
    ���ܣ� �Ͽ�����ܻ�����������������
    ��Σ���
    ���Σ���
    ���أ�
    0���ɹ�������ʧ��
    */
    int DisConnect();


    /*
    ���ܣ� ���������
    ��Σ�
    int sRandLen,���������
    ���Σ�
    unsigned char *random�����������
    ���أ�
    0���ɹ�������ʧ��
    */
    int GenRand(int sRandLen, uint8_t *random);



    /*
    ���ܣ� ˽Կǩ����ԭʼ������󳤶�2K
    ��Σ�
    int keyIndex,�ǶԳ���Կ����,1-4��վ��Կ��5������Կ
    int inDataLen, ԭʼ���ݳ���
    unsigned char *inData,ԭʼ����
    ���Σ�
    unsigned char *sData,ǩ�����
    int *sDataLen��ǩ���������
    ���أ�
    0���ɹ�������ʧ��
    */
    int InternalSign(int keyIndex, int inDataLen, uint8_t *inData, uint8_t *sData, int *sDataLen);


    /*
    ���ܣ� ��Կ��ǩ
    ��Σ�
    int cerLen,֤�鳤��
    unsigned char *cer,֤��
    int oriDataLen��ԭʼ���ݳ���
    unsigned char *oriData,ԭʼ����
    int sDataLen,ǩ�����ݳ��ȣ��̶�ֵΪ64
    unsigned char *sData������֤��ǩ��ֵ
    ���Σ�
    ��
    ���أ�
    0���ɹ���-1ʧ��
    */
    int VerSign(int cerLen, uint8_t *cer, int oriDataLen, uint8_t *oriData, int sDataLen, uint8_t *sData);


    /*
    ���ܣ� ���ݼ��ܲ���MAC
    ��Σ�
    int keyVersion,��Կ�汾,����С��0
    int keyIndex,��Կ����������
    int divTimes,��Կ��ɢ���������ֵ3
    unsigned char *divData,��ɢ���ӣ�8�ֽڵ���������divTimes*8
    unsigned char *ivData,��ʼ�������̶�����16�ֽ�
    int plainDataLen���������ݳ���
    unsigned char *plainData,����ԭʼ����
    ���Σ�
    unsigned char *enData����������
    int *enDataLen���������ݳ���
    ���أ�
    0���ɹ�������ʧ��
    */
    int EnData(int plainDataLen, uint8_t *plainData, uint8_t *enData, int *enDataLen);


    /*
    ���ܣ� MAC��֤������
    ��Σ�
    int keyVersion,��Կ�汾
    int keyIndex,��Կ����������
    int divTimes,��Կ��ɢ����
    unsigned char *divData,��ɢ���ӣ�8�ֽڵ���������divTimes*8
    unsigned char *ivData,��ʼ������8�ֽ�
    int enDataLen���������ݳ���
    unsigned char *enData����������
    ���Σ�
    unsigned char *plainData,���ܺ���������
    int *plainDataLen ���������ݳ���
    ���أ�
    0���ɹ�������ʧ��
    */
    int DeData(int enDataLen, uint8_t *enData, uint8_t *plainData, int *plainDataLen);

	

    /*
    ���ܣ������Գ���Կ
    ���룺
    int oldKeyVersion ����Կ�汾��>=0
    int NewKeyVersion ����Կ�汾��>=0
    ����ʱ��NewKeyVersion> oldKeyVersion,��NewKeyVersion - oldKeyVersion=1
    ��Կ�ָ�ʱ��NewKeyVersion=0��oldKeyVersion>0
    int divTimes����ɢ������ֵ�̶�Ϊ1
    unsigned char *divData����ɢ���ӣ�8�ֽڣ�оƬ���к�
    char *ivData,��ʼ������16�ֽ�
    int asKid,�ǶԳ���Կ����,1-4��վ��Կ
    �����
    char *updateKeyData �Գ���Կ��������
    int *olen �������ݵĳ���
    ���أ�
    0���ɹ�������ʧ��
    */
    int GetUpdateKey(int oldKeyVersion, int NewKeyVersion, int divTimes, uint8_t *divData, uint8_t *ivData, int asKid, uint8_t *updateKeyData, int *olen);

	//��У��
	unsigned char checkAllData(const unsigned char *data, int length);
	//
	//int GetHostHandle()
	//{
	//	return m_HostHandle;
	//}
	//�����ն�оƬ���к�
	void SetCellIndex(char *pIndex, int nLength);
	//���ð汾��
	void SetVersionIndex(int nIndex);
	//���ó�ʼ����
	void SetOrignialVer(char *pIvData);
};

