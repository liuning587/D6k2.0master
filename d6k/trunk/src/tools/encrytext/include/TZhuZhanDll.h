#include <windows.h>
/*
���ܣ� ��������ܻ�����������������
��Σ�
char *cHostIp,���ܻ�IP��ַ���ַ���������"192.168.1.12"��ʹ�ö����������ֵ��������
int inPort,���ܻ��˿ڣ�ʹ�ö����������ֵ������
int timeout,��ʱʱ�䣨��λ���룩��ʹ�ö����������ֵ������
���Σ���
���أ�
�ɹ����ض��������������0��С��0ʧ��
*/
int WINAPI ConnectHSM(char * cHostIp,int inPort,int timeout);


/*
���ܣ� �Ͽ�����ܻ�����������������
��Σ�
int sConnect�����ܻ��������������
���Σ���
���أ�
0���ɹ�������ʧ��
*/
int WINAPI DisConnect(int sConnect);


/*
���ܣ� ���������
��Σ�
int sConnect�����ܻ����
int sRandLen,���������
���Σ�
unsigned char *random�����������
���أ�
0���ɹ�������ʧ��
*/
int WINAPI GenRand(int sConnect, int sRandLen, unsigned char *random);



/*
���ܣ� ˽Կǩ����ԭʼ������󳤶�2K
��Σ�
int sConnect�����ܻ����
int keyIndex,�ǶԳ���Կ����,1-4��վ��Կ��5������Կ
int inDataLen, ԭʼ���ݳ���
unsigned char *inData,ԭʼ����
���Σ�
unsigned char *sData,ǩ�����
int *sDataLen��ǩ���������
���أ�
0���ɹ�������ʧ��
*/
int WINAPI InternalSign(int sConnect, int keyIndex, int inDataLen, unsigned char *inData, unsigned char *sData, int *sDataLen);


/*
���ܣ� ��Կ��ǩ
��Σ�
int sConnect�����ܻ����
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
int WINAPI VerSign(int sConnect, int cerLen, unsigned char *cer, int oriDataLen, unsigned char *oriData, int sDataLen, unsigned char *sData);


/*
���ܣ� ���ݼ��ܲ���MAC
��Σ�
int sConnect�����ܻ����
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
int WINAPI EnData(int sConnect,int keyVersion,int keyIndex, int divTimes,unsigned char *divData,unsigned char *ivData,int plainDataLen,unsigned char *plainData,unsigned char *enData,int *enDataLen);


/*
���ܣ� MAC��֤������
��Σ�
int sConnect�����ܻ����
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
int WINAPI DeData(int sConnect,int keyVersion,int keyIndex, int divTimes,unsigned char *divData,unsigned char *ivData,int enDataLen,unsigned char *enData,unsigned char *plainData,int *plainDataLen);


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
int WINAPI GetUpdateKey(int sConnect, int oldKeyVersion,  int NewKeyVersion, int divTimes,unsigned char *divData, 
						unsigned char *ivData, int asKid, unsigned char *updateKeyData, int *olen);


int WINAPI ImportPriKey(int sConnect, char *plic);