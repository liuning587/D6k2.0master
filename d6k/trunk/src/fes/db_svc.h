/*! @file db_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  db_svc.h
�ļ�ʵ�ֹ��� :  ʵʱ���ݿ�ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���Դ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef DBSVC_MODULE_H
#define DBSVC_MODULE_H

#include "base_module.h"
#include "fesconfig.h"
#include "fesapi/fes_magicmem.h"

#include <QDomDocument>
#include <QXmlStreamReader>
#include <QMap>
#include <QtCore/QtGlobal>

#include <vector>
#include <unordered_map>
#include <ace/Event.h>

struct FesInfo;
struct NODE;
struct CHANNEL;
struct DEVICE;
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;
struct TRANSFORM_LINEAR;
struct TRANSFORM_NONLINEAR;
struct TRANSFORM;
struct ALARM;
struct AIN_ALARM;
struct AIN_ALARM_LIMIT;
struct DIN_ALARM;
struct DIN_ALARM_LIMIT;
struct TAG_OCCNO;
struct VARDATA;
struct DMSG;

typedef std::shared_ptr<TRANSFORM_LINEAR>    TRANSFORM_LINEAR_DEF;
typedef std::shared_ptr<TRANSFORM_NONLINEAR> TRANSFORM_NONLINEAR_DEF;
typedef std::shared_ptr<AIN_ALARM> AIN_ALARM_DEF;
typedef std::shared_ptr<DIN_ALARM> DIN_ALARM_DEF;
typedef std::shared_ptr<AIN_ALARM_LIMIT> AIN_ALARM_LIMIT_DEF;
typedef std::shared_ptr<DIN_ALARM_LIMIT> DIN_ALARM_LIMIT_DEF;
typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;
typedef std::shared_ptr<VARDATA> VARDATA_DEF;
class CShareMem;
class CServer;
class CIoChannel;
class CFTChannel;
class CSysVarSvc;
class CTagAttMgr;

enum XmlReaderType
{
	_DOM_XML_TYPE    =1,
	_STREAM_XML_TYPE =2,
};

class CDbSvc : public CBaseModule
{
public:
	CDbSvc(CFesSvc* pServer, const std::string&szMailBoxName,int &MailID);
	virtual ~CDbSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();

	// ���ڵ��Node�����кţ���������ӻ����򷵻�Ĭ������
	int32u GetMyNodeOccNo()
	{
		//!todo :��ʵ��
		//return 0;
		return m_nMyNodeOccNo;
	}

	std::pair<size_t, NODE*> GetNodeData()
	{
		return std::make_pair(m_nNodeCount, m_pNodes);
	}

	//�����ڴ�ṹ
	std::shared_ptr<CChannelMgr>&  GetChannelMgr()
	{
		return m_pChannelMgr;
	}
	//�����ڴ���Ϣ
	bool GetChannelByOccNo(int32u nOccNo, CHANNEL** pChannel);

	bool GetDeviceByOccNo(int32u nOccNo, DEVICE** pDev);

	bool GetAinByOccNo(int32u nOccNo, AIN** pAIN);

	bool GetDinByOccNo(int32u nOccNo, DIN** pDIN);

	bool GetAoutByOccNo(int32u nOccNo, AOUT** pAout);

	bool GetDoutByOccNo(int32u nOccNo, DOUT** pDout);

	
	// ǰ���ں˵��ڲ����ݷ��ʽӿ�
	int32u GetNodeOccNo(const char *pszHostName);

	size_t GetDeviceCount(int32u nChannelOccNo)const;
	// ��ȡͨ���е�װ���б�
	size_t GetDeviceOccNos(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);

	size_t GetAinCountInChannel(int32u nChannelOccNo);
	size_t GetDinCountInChannel(int32u nChannelOccNo);
	size_t GetAoutCountInChannel(int32u nChannelOccNo);
	size_t GetDoutCountInChannel(int32u nChannelOccNo);

	size_t GetAinCountInDevice(int32u nDeviceOccNo)const;
	size_t GetDinCountInDevice(int32u nDeviceOccNo)const;
	size_t GetAoutCountInDevice(int32u nDeviceOccNo)const;
	size_t GetDoutCountInDevice(int32u nDeviceOccNo)const;

	// ��ȡͨ���в����б�
	size_t GetAinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);

	// ��ȡװ���в����б�
	size_t GetAinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);

	//�������ݳ�����Ϣ
	size_t GetChannelCount() const
	{
		return m_nChannelCount;
	}
	size_t GetDeviceCount() const
	{
		return m_nDeviceCount;
	}
	size_t GetAINCount() const
	{
		return m_nAinCount;
	}
	size_t GetDINCount() const
	{
		return m_nDinCount;
	}
	size_t GetAoutCount() const
	{
		return m_nAoutCount;
	}
	size_t GetDoutCount() const
	{
		return m_nDoutCount;
	}
	size_t GetUserVarCount() const
	{
		return m_nUserVariableCount;
	}
	size_t GetSysVarCount() const
	{
		return m_nSystemVariableCount;
	}

	// ͨ����Ż�ȡģ����ָ�룬nIdx ��0��ʼ
	CHANNEL* GetChannelByIndex(int32u nIndex);
	DEVICE* GetDeviceByIndex(int32u nIndex);
	AIN*GetAinByIndex(int32u nIdx);
	DIN*GetDinByIndex(int32u nIdx);
	AOUT*GetAoutByIndex(int32u nIdx);
	DOUT*GetDoutByIndex(int32u nIdx);
	VARDATA * GetUserVarByIndex(int32u nIdx);
	VARDATA * GetSysVarByIndex(int32u nIdx);

	NODE * GetNodeByOccNo(int32u nOccNo);
	CHANNEL* GetChannelByOccNo(int32u nOccNo);
	DEVICE* GetDeviceByOccNo(int32u nOccNo);
	AIN*GetAinByOccNo(int32u nOccNo);
	DIN*GetDinByOccNo(int32u nOccNo);
	AOUT*GetAoutByOccNo(int32u nOccNo);
	DOUT*GetDoutByOccNo(int32u nOccNo);
	VARDATA * GetUserVarByOccNo(int32u nOccNo);
	VARDATA * GetSysVarByOccNo(int32u nOccNo);

	std::string GetTagName(int32u nOccNo, int32u nDataType) const;

	size_t GetIoChannelNames(std::vector<std::string>& arrTagNames)const;
	size_t GetRemoteChannelNames(std::vector<std::string>& arrTagNames)const;

	void CreateMailBoxs();
	void DestroyMailBoxs();

	bool IsDBAlive(unsigned int nTimeout);

	int32u GetMyHostState()const;
	int8u GetMyHostScanEnable()const;
	// ��鿪���ı���״̬
	void CheckDoutBlockState(DOUT & dout);

	int32u GetNodeState(int32u nOccNo)const;

	int32s GetChannelMailBoxID(int32u nChannelNo);
	// IO ��Ĳ����ӿ�
	bool IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const;
	bool IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality) const;

	// �迪��
	bool FesSetDoutValue(int32u nOccNo, int8u Value, const char *pszAppTagName);
	// дֵ
	bool FesSetAoutValue(int32u nOccNo, fp64 Value, const char *pszAppTagName);

	void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// ��������
	void IoOperAlarm(int32u nChannleNo, int32u nDeviceNo, const char * pszAlarmTxt, TIMEPAK * pTm);
	// �����������
	void IoRelayDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// ͨ�õĸ澯
	void IoAlarmMsg(int32u nChannleNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);

	bool FesSetNodeValueImpl(DMSG *pMsg);
	bool FesSetChannelValueImpl(DMSG *pMsg);
	bool FesSetDeviceValueImpl(DMSG *pMsg);

	bool FesSetAinValueImpl(DMSG *pMsg);
	bool FesSetDinValueImpl(DMSG *pMsg);

	bool FesSetAoutValueImpl(DMSG *pMsg);
	bool FesSetDoutValueImpl(DMSG *pMsg);

	bool FesSetUserVarValueImpl(DMSG *pMsg);
	bool FesSetSysVarValueImpl(DMSG *pMsg);


private:
	//��ȡ�ڵ���Ϣ
	bool GetNodeInfo(const char* szName);
	//��ȡ���нڵ���Ϣ
	bool GetTotalNodeInfo(const char* szName);
	//��ȡ��ǰǰ��������Ϣ
	bool GetFesInfo(const char* szName);
	//�жϽڵ��Ƿ�����
	bool IsNodeAdded(const QString& str,QDomElement nEle);

	// ����ң�صľ���ִ�к���
//	bool FesSetDoutValueImpl(int32u nOccNo, int8u Value, int8u nSource);
	// дֵ
//	bool FesSetAoutValueImpl(int32u nOccNo, fp64 Value, int32u nSource);


	bool SetNodeAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetIoChannelAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetFtChannelAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

	bool SetDeviceAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetAinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetDinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

	bool SetAoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetDoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

	bool SetUserVarAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetSysVarAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

private:
	QString m_szNodeName;
	int32u  m_nMyNodeOccNo;
	QString m_szCurFesInfoName;
	QVector < QString > m_VecFesFileName;
	
private:
	bool ReadXmlInfo(const char* pszPath, XmlReaderType nType);

/************���¶�ȡ��ʽ��������ʽ��ȡ********************************/
private:
	//��Ҫ��Ϣ
	void ReadFesInfo();
	void ReadChannelInfo();
	void ReadDev(CChannelPtr pChannel);
	void ReadDevInfo(CChannelPtr pChannel);
	void ReadData(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadTransFormInfo();
	void ReadAlarmInfo();
	//���ݵ���Ϣ
	void ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel);
	//ת����Ϣ
	void ReadScale();
	void ReadLinearInfo();
	void ReadNonLinearInfo();
	void ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear);
	//�澯��Ϣ
	void ReadAlarm();
	void ReadAinAlarmInfo();
	void ReadDinAlarmInfo();
	//Խ�޸澯
	void ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm);
	void ReadDinOfflimitInfo(DIN_ALARM_DEF pDinAlarm);
	void SkipUnknownElement();
	//����
	void ReadVariable();
	void ReadSystemVariable();
	void ReadUserVariable();
private:
	//����ȡ��Ϣ
	QXmlStreamReader m_XmlReader;

/************���¶�ȡ��ʽ����DOM��ʽ��ȡ****************************/
private:
	//��Ҫ��Ϣ
	bool ReadChannelInfo(QDomElement eleCHannel);
	bool ReadTransFormInfo(QDomElement eleDevice);
	bool ReadAlarmInfo(QDomElement eleData);
	//���ݵ���Ϣ
	bool ReadAinInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	bool ReadDinInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	bool ReadAoutInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	bool ReadDoutInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	//ת����Ϣ
	bool ReadLinearInfo(QDomElement eleData);
	bool ReadNonLinearInfo(QDomElement eleData);
	//�澯��Ϣ
	bool ReadAinAlarmInfo(QDomElement eleData);
	bool ReadDinAlarmInfo(QDomElement eleData);
	//Խ�޸澯
	bool ReadAinOfflimitInfo(QDomElement eleData);
	bool ReadDinOfflimitInfo(QDomElement eleData);
private:
	//��ڵ�
	QDomElement m_DomNodes;
	QDomElement m_DomChannel;
	QDomElement m_DomTransForm;
	QDomElement m_DomAlarm;
	//С�ڵ�
	QDomElement m_DomDev;
	QDomElement m_DomData;
private:
	//! �������ļ��ж�ȡ��������Ϣ�������ڴ�����
	FesInfo_PTR           m_pFesInfo;
	std::vector<std::shared_ptr<NODE>>    m_arrTempNodes;
	std::vector<std::shared_ptr<CHANNEL>> m_arrTempChannels;
	std::vector<std::shared_ptr<DEVICE>>  m_arrTempDevices;
	std::vector<std::shared_ptr<AIN>>     m_arrTempAins;
	std::vector<std::shared_ptr<DIN>>     m_arrTempDins;
	std::vector<std::shared_ptr<AOUT>>    m_arrTempAouts;
	std::vector<std::shared_ptr<DOUT>>    m_arrTempDouts;

	std::vector<TRANSFORM_LINEAR_DEF>     m_arrTempTransFormLinears;
	std::vector<TRANSFORM_NONLINEAR_DEF>  m_arrTempTransFormNonLinears;

	std::vector<AIN_ALARM_DEF>            m_arrTempAinAlarms;
	std::vector<DIN_ALARM_DEF>            m_arrTempDinAlarms;
	std::vector<AIN_ALARM_LIMIT_DEF>      m_arrTempAinLimitAlarms;
	std::vector<DIN_ALARM_LIMIT_DEF>      m_arrTempDinLimitAlarms;
	std::shared_ptr<CChannelMgr>          m_pChannelMgr;

	std::vector<VARDATA_DEF> m_arrTempSystemVariables;
	std::vector<VARDATA_DEF> m_arrTempUserVariables;
private:
	// �ڴ洴��
	void BuildMemDB(const char *szDBName);	
	//����ͨ���ڴ�
	size_t BuildChannelDB(char* pAddress);
	//�����ڵ��ڴ�
	size_t BuildNodeDB(char* pAddress);
	//�����豸�ڴ�
	size_t BuildDeviceDB(char* pAddress);
	//����AIN�ڴ�
	size_t BuildAinDB(char* pAddress);
	//����DIN�ڴ�
	size_t BuildDinDB(char* pAddress);
	//����AOUT�ڴ�
	size_t BuildAoutDB(char* pAddress);
	//����DOUT�ڴ�
	size_t BuildDoutDB(char* pAddress);
	//�����澯�ڴ�
	size_t BuildAlarmDB(char* pAddress);
	//ģ�����澯�Լ���ֵ
	size_t BuildAinAlarmDB(char* pAddress);
	//�������澯
	size_t BuildDinAlarmDB(char* pAddress);
	//���Ա仯
	size_t BuildLinearDB(char* pAddress);
	//�����Ա任
	size_t BuildNonLinearDB(char* pAddress);
	//ϵͳ����
	size_t BuildSystemVariableDB(char* pAddress);
	//�û�����
	size_t BuildUserVariableDB(char* pAddress);
	//�����䳤�ڴ�
	size_t BuildExtraDB(char* pAddress);

	void OutPutShm(char* pAddress);

	void LoadDrivers();
	void StartDrivers();
	void StopDrivers();

	// ǿ�������ʱ���ݣ������ڴ�ռ��
	void ClearTempArray();
private:
	QString  m_szRdbName;

	HEAD_MEM* m_pMagicMem;

	std::shared_ptr<CSysVarSvc> m_pSysSvc;

	std::shared_ptr<ACE_Event> m_pDBAliveFlag;
	ACE_Event m_DBAliveFlag;
private:
	//! ��ν���ڴ����ݿ�
	std::shared_ptr<CShareMem> m_pMem;

	std::string m_szDataBaseName;

	// �����ڴ��еĸ����ݵ��Ų�
	NODE*  m_pNodes;
	size_t m_nNodeCount;

	CHANNEL *m_pChannels;
	size_t m_nChannelCount;
	//! ��ס��ַ���������
	std::vector<CHANNEL*> m_arrChannelPtrs;
	std::vector<std::shared_ptr<CIoChannel>> m_arrIoChannels;

	DEVICE *m_pDevices;
	size_t m_nDeviceCount;
	std::vector<DEVICE*> m_arrDevices;

	AIN *m_pAins;
	size_t m_nAinCount;
	std::vector<AIN*> m_arrAins;

	DIN *m_pDins;
	size_t m_nDinCount;
	std::vector<DIN*> m_arrDins;

	DOUT *m_pDouts;
	size_t m_nDoutCount;
	std::vector<DOUT*> m_arrDouts;

	AOUT *m_pAouts;
	size_t m_nAoutCount;
	std::vector<AOUT*> m_arrAouts;


	TRANSFORM_LINEAR *m_pLinear;
	size_t m_nLinearCount;
	std::vector<TRANSFORM_LINEAR*> m_arrLinears;

	TRANSFORM_NONLINEAR *m_pNonLinear;
	size_t m_nNonLinearCount;
	std::vector<TRANSFORM_NONLINEAR*> m_arrNonLinears;

	AIN_ALARM  *m_pAinAlarm;
	size_t m_nAinAlarmCount;
	std::vector<AIN_ALARM*> m_arrAinAlarms;

	AIN_ALARM_LIMIT *m_pAinAlarmLimit;
	size_t m_nAinAlarmLimitCount;
	std::vector<AIN_ALARM_LIMIT*> m_arrAinAlarmLimits;

	DIN_ALARM *m_pDinAlarm;
	size_t m_nDinAlarmCount;
	std::vector<DIN_ALARM*> m_arrDinAlarms;

	DIN_ALARM_LIMIT *m_pDinAlarmLimit;
	size_t m_nDinAlarmLimitCount;
	std::vector<DIN_ALARM_LIMIT*> m_arrDinAlarmLimits;

	VARDATA * m_pSystemVariable;
	size_t m_nSystemVariableCount;
	std::vector<VARDATA* >m_arrSystemVariables;

	VARDATA*  m_pUserVariable;
	size_t m_nUserVariableCount;
	std::vector<VARDATA* >m_arrUserVariables;

	FT_CHANNEL *m_pFTChannels;
	size_t m_nFTChannelCount;
	//! ��ס��ַ���������
	std::vector<CHANNEL*> m_arrFTChannelPtrs;
	std::vector<std::shared_ptr<CFTChannel>> m_arrFTChannels;

private:
	//����TagName�洢��
	void BuildTagNamePoolDB(const char* szName);

	size_t BuildTagNameDB(char* pAddress,const std::vector<TAG_OCCNO_DEF>& vec);
	
	void StartMainTask();
	void StopMainTask();

	void MainTask(void *pImpl);
private:
	//! ��ν���ڴ����ݿ�
	std::shared_ptr<CShareMem> m_pTagMem;

	HEAD_MEM* m_pMagicTagMem;

	std::vector<TAG_OCCNO_DEF> m_arrNodeOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrChannelOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDeviceOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAoutOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDoutOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmLimitOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmLimitOccNos;	
	std::vector<TAG_OCCNO_DEF> m_arrTransLinearOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrTransNonLinearOccNos;

	std::vector<TAG_OCCNO_DEF> m_arrSyetemVarOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrUserVarOccNos;

private:
	std::shared_ptr<CShareMem> m_pStringMen;
	HEAD_STRING * m_pMagicStringMem;
	StringUnit * m_pStringUnit;
	std::vector< std::shared_ptr < StringUnit > > m_arrStrings;
	std::unordered_map<int32u, std::shared_ptr < StringUnit > >m_MapNo2String;


	std::shared_ptr<CTagAttMgr> m_pTagAttrMgr;
private:
	bool BuildStringNamePool(const char* pszPathName);
	bool LoadStringNamePool(const char* pszPatnName);
	bool ReadStringUnit(QXmlStreamReader& reader);
};

#endif // _DBSVC_MODULE_H


/** @}*/

