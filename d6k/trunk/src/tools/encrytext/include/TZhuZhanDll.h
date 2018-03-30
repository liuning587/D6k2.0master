#include <windows.h>
/*
功能： 建立与加密机（读卡器）的连接
入参：
char *cHostIp,加密机IP地址，字符串，例如"192.168.1.12"，使用读卡器，这个值可以任意
int inPort,加密机端口，使用读卡器，这个值可任意
int timeout,超时时间（单位毫秒），使用读卡器，这个值可任意
出参：无
返回：
成功返回读卡器句柄，大于0；小于0失败
*/
int WINAPI ConnectHSM(char * cHostIp,int inPort,int timeout);


/*
功能： 断开与加密机（读卡器）的连接
入参：
int sConnect；加密机（读卡器）句柄
出参：无
返回：
0，成功；其他失败
*/
int WINAPI DisConnect(int sConnect);


/*
功能： 生成随机数
入参：
int sConnect；加密机句柄
int sRandLen,随机数长度
出参：
unsigned char *random，随机数数据
返回：
0，成功；其他失败
*/
int WINAPI GenRand(int sConnect, int sRandLen, unsigned char *random);



/*
功能： 私钥签名，原始数据最大长度2K
入参：
int sConnect；加密机句柄
int keyIndex,非对称密钥索引,1-4主站密钥，5网关密钥
int inDataLen, 原始数据长度
unsigned char *inData,原始数据
出参：
unsigned char *sData,签名结果
int *sDataLen，签名结果长度
返回：
0，成功；其他失败
*/
int WINAPI InternalSign(int sConnect, int keyIndex, int inDataLen, unsigned char *inData, unsigned char *sData, int *sDataLen);


/*
功能： 公钥验签
入参：
int sConnect；加密机句柄
int cerLen,证书长度
unsigned char *cer,证书
int oriDataLen，原始数据长度
unsigned char *oriData,原始数据
int sDataLen,签名数据长度，固定值为64
unsigned char *sData，待验证的签名值
出参：
无
返回：
0，成功；-1失败
*/
int WINAPI VerSign(int sConnect, int cerLen, unsigned char *cer, int oriDataLen, unsigned char *oriData, int sDataLen, unsigned char *sData);


/*
功能： 数据加密并算MAC
入参：
int sConnect；加密机句柄
int keyVersion,密钥版本,不能小于0
int keyIndex,密钥索引，保留
int divTimes,密钥分散次数，最大值3 
unsigned char *divData,分散因子，8字节的整数倍，divTimes*8
unsigned char *ivData,初始向量，固定长度16字节
int plainDataLen，明文数据长度
unsigned char *plainData,明文原始数据
出参：
unsigned char *enData，密文数据
int *enDataLen，密文数据长度
返回：
0，成功；其他失败
*/
int WINAPI EnData(int sConnect,int keyVersion,int keyIndex, int divTimes,unsigned char *divData,unsigned char *ivData,int plainDataLen,unsigned char *plainData,unsigned char *enData,int *enDataLen);


/*
功能： MAC验证并解密
入参：
int sConnect；加密机句柄
int keyVersion,密钥版本
int keyIndex,密钥索引，保留
int divTimes,密钥分散次数
unsigned char *divData,分散因子，8字节的整数倍，divTimes*8
unsigned char *ivData,初始向量，8字节
int enDataLen，密文数据长度
unsigned char *enData，密文数据
出参：
unsigned char *plainData,解密后明文数据
int *plainDataLen ，密文数据长度
返回：
0，成功；其他失败
*/
int WINAPI DeData(int sConnect,int keyVersion,int keyIndex, int divTimes,unsigned char *divData,unsigned char *ivData,int enDataLen,unsigned char *enData,unsigned char *plainData,int *plainDataLen);


/*
功能：导出对称密钥
输入：
int oldKeyVersion 旧密钥版本，>=0
int NewKeyVersion 新密钥版本，>=0
升级时：NewKeyVersion> oldKeyVersion,且NewKeyVersion - oldKeyVersion=1
密钥恢复时：NewKeyVersion=0，oldKeyVersion>0
int divTimes，分散次数，值固定为1
unsigned char *divData，分散因子，8字节，芯片序列号
char *ivData,初始向量，16字节
int asKid,非对称密钥索引,1-4主站密钥
输出：
char *updateKeyData 对称密钥更新数据
int *olen 更新数据的长度
返回：
0，成功；其他失败
*/
int WINAPI GetUpdateKey(int sConnect, int oldKeyVersion,  int NewKeyVersion, int divTimes,unsigned char *divData, 
						unsigned char *ivData, int asKid, unsigned char *updateKeyData, int *olen);


int WINAPI ImportPriKey(int sConnect, char *plic);