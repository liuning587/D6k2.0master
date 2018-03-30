#ifndef WAVE_INFO_H
#define WAVE_INFO_H 

#include <string>
#include <QString>
#include <memory>
#include <vector>
#include <functional>


/*! \class CChannel wave_info.h  "wave_info.h"
*  \brief ͨ������
*  \details �����У�
*/
class CChannel
{
public:
	CChannel()
	{

	}
	~CChannel()
	{

	}
public:
	// ���Ͳ�ֵ��ͨ��
	virtual void PushSampValue(float fValue)
	{
		m_arrSampData.emplace_back(fValue);
	}
	// ��ȡ������ԭʼֵ
	float GetSampRawValue(int nIndex) const;

	int GetSampCount()const
	{
		return static_cast<int>(m_arrSampData.size());
	}

	std::vector<float> & GetSampData()
	{
		return m_arrSampData;
	}
protected:
	std::vector<float> m_arrSampData; //! ��������
};
/*! \class CAnaChannel wave_info.h  "wave_info.h"
*  \brief ģ��ͨ��
*  \details �����У� 
*/
class CAnaChannel :public CChannel
{
public:
	CAnaChannel();
	~CAnaChannel();
public:
	enum PSType
	{
		P_TYPE = 0,
		S_TYPE = 1,
	};
	bool ParseInfo(const QString & szInput);

public:
	int GetIndex()const
	{
		return m_nIndex;
	}

	QString GetName() const
	{
		return QString::fromLocal8Bit(m_szID.c_str());
	}

	QString GetUnit() const
	{
		return QString::fromLocal8Bit(m_szUnit.c_str());
	}
	
	float GetMaxSampValue()const
	{
		return m_fMaxSampValue;
	}

	float GetMinSampValue()const
	{
		return m_fMinSampValue;
	}

	float GetMaxValue()const
	{
		return static_cast<float>(m_nMax);
	}

	float GetMinValue()const
	{
		return static_cast<float>(m_nMin);
	}

	virtual void PushSampValue(float fValue)
	{
		if (m_arrSampData.empty())
		{
			m_fMaxSampValue = fValue;
			m_fMinSampValue = fValue;
		}
		else
		{
			if (fValue > m_fMaxSampValue)
			{
				m_fMaxSampValue = fValue;
			}
			if (fValue < m_fMinSampValue)
			{
				m_fMinSampValue = fValue;
			}
		}
		m_arrSampData.emplace_back(fValue);
	}
	bool GetValue(int nIndex, float &fValue)const;
protected:
	bool LogString(const QString & szLog, int nLevel)const;
private:
	int m_nIndex;               //! ͨ������ �������ֶΣ�
	std::string  m_szID;        //! ͨ��ʶ��� �������ֶΣ�
	std::string  m_szPhase;     //! ͨ����λ���� �������ֶΣ�
	std::string  m_szCCBM;      //! �����ӵĻ�·Ԫ��
	std::string  m_szUnit;      //! ��λ
	float  m_fA;
	float  m_fB;
	float  m_fSkew;
	int    m_nMin;
	int    m_nMax;
	float  m_fPrimary;
	float  m_fSecondary;
	PSType m_PSType;

	enum LineIndex1
	{
		LI1_INDEX = 0,
		LI1_ID = 1,
		LI1_PHASE = 2, //!< ͨ����ʶ��
		LI1_CCBM,
		LI1_UU,
		LI1_A,       //!< �任����
		LI1_B,
		LI1_SKEW,   //!< ʱ��ƫ��
		LI1_MIN,    //!< ������¼��������Сֵ
		LI1_MAX,    //!< ������¼���������ֵ
		LI1_ANA_MAX_COUNT
	};

	enum LineIndex2
	{
		LI2_INDEX = 0,
		LI2_ID = 1,
		LI2_PHASE = 2, //!< ͨ����ʶ��
		LI2_CCBM,
		LI2_UU,
		LI2_A,
		LI2_B,
		LI2_SKEW,
		LI2_MIN,
		LI2_MAX,
		LI2_PRIMARY,
		LI2_SECONDARY,
		LI2_PS,
		LI2_ANA_MAX_COUNT
	};

	float m_fMaxSampValue;
	float m_fMinSampValue;
};
/*! \class CDigitalChannel wave_info.h  "wave_info.h"
*  \brief ����ͨ��
*  \details �����У�
*/
class CDigitalChannel :public CChannel
{
public:
	CDigitalChannel();
	~CDigitalChannel();
public:
	bool ParseInfo(const QString & szInput);

public:
	int GetIndex()const
	{
		return m_nIndex;
	}

	QString GetName() const
	{
		return QString(m_szID.c_str());
	}

protected:
	bool LogString(const QString & szLog, int nLevel)const;

private:
	int m_nIndex;               //! ͨ������ �������ֶΣ�
	std::string  m_szID;        //! ͨ��ʶ��� �������ֶΣ�
	std::string  m_szPhase;     //! ͨ����λ���� �������ֶΣ�
	std::string  m_szCCBM;      //! �����ӵĻ�·Ԫ��
	int m_nY;

	enum LineIndex
	{
		LI_INDEX = 0,
		LI_ID = 1,
		LI_PHASE = 2, //!< ͨ����ʶ��
		LI_CCBM,
		LI_Y,
		LI_DIGI_MAX_COUNT
	};
};
/*! \class CWaveCfg wave_info.h  "wave_info.h"
*  \brief �����ļ�
*  \details �����У�
*/
class CWaveCfg
{
public:
	CWaveCfg();
	~CWaveCfg();

	enum FileType
	{
		FT_ASCII = 0,   //!< ASC����
		FT_BINARY = 1   //!< BIN����
	};

	struct SampRate
	{
		float Samp;     //! ��HZΪ��λ�Ĳ�����
		int   EndSamp;  //! �ڸò����������һ�β�����
	};

public:
	// �����ļ�
	bool ReadFile(const QString & szFilePathName);

	FileType GetFileType() const
	{
		return m_nFileType;
	}

	int GetTotalChannelCount()const
	{
		return m_nTotoalChannelCount;
	}

	int GetAnaChannelCount()const
	{
		return m_nAChannelCount;
	}

	int GetDigiChannelCount() const
	{
		return m_nDChannelCount;
	}
	// �����ļ��в����ʲ�����
	int GetSampCount()const
	{
		return m_nSampCount;
	}

	bool PushSampValue(int nChannelIdx, float fValue);

	std::vector<std::shared_ptr<CAnaChannel>> & GetAnaChannels()
	{
		return m_arrAnaChannelInfos;
	}

	std::vector<std::shared_ptr<CDigitalChannel>> &GetDigiChannels()
	{
		return m_arrDigiChannelInfos;
	}

protected:
	bool LogString(const QString & szLog, int nLevel)const;

	bool ParseChannelCount(const QString & szInput);
	bool ParseStation(const QString & szInput);
	bool ParseLineFreq (const QString & szInput);
	bool ParseSampRateCount(const QString & szInput);

	bool ParseSampRate(const QString & szInput,SampRate & sr);

	bool ParseFileType(const QString & szInput);

	bool ParseTimeMult(const QString & szInput);

	bool ParseDataTime(const QString & szInput, int nIdx);

private:
	std::string  m_szStationName; //! վ��
	std::string  m_szRecDevID;    //! �豸��� ���Ǳ����ֶΣ�
	std::string  m_szRevYear;     //! ��׼������� ���Ǳ����ֶΣ�

	FileType     m_nFileType;     //! �ļ�����

	int m_nTotoalChannelCount;    //! ͨ������
	int m_nAChannelCount;         //! ģ��ͨ������
	int m_nDChannelCount;         //! ����ͨ������

	float m_fLineFreq;           //! ��·Ƶ��

	int   m_nSampCount;          //! �����ļ��в���������
	std::vector<SampRate>  m_arrSampRates;

	float  m_fTimeMult;          //! ʱ���ǳ���ϵ��

	QString  m_szDataTime1;
	QString  m_szDataTime2;

	enum LineIndex
	{
		LI_STATION_INFO = 1,
		LI_CHANNEL_INFO = 2,
		LI_LINEFREQ_OFFSET = 1,
		LI_SAMPCOUNT_OFFSET = 2,
		LI_SAMP_OFFSET = 1,
		LI_DT1_OFFSET = 1,
		LI_DT2_OFFSET = 2,
		LI_FT_OFFSET = 3,
		LI_MULT_OFFSET = 4,  //! ʱ���ǳ���ϵ�� �Ӳ�����ʼʱ�� LI_DT1_OFFSET ƫ��

		kMaxChannelCount = 999999,
	};

	std::vector<std::shared_ptr<CAnaChannel>> m_arrAnaChannelInfos;

	std::vector<std::shared_ptr<CDigitalChannel>> m_arrDigiChannelInfos;
};
/*! \class CWaveDat wave_info.h  "wave_info.h"
*  \brief �����ļ�
*  \details �����У�
*/
class CWaveDat
{
	class CDataLine
	{
	public:
		CDataLine()
		{
			m_nIndex = 0;
			m_nTimeStamp = 0;
		}
		~CDataLine()
		{

		}
	public:
		bool ParseLine(const QString & szInput, CWaveCfg&conf,int nLineIdx,std::function<bool(const QString &,int)>);

		int GetIndex()const
		{
			return m_nIndex;
		}
	private:
		int m_nIndex;       //! ������
		int m_nTimeStamp;   //! ʱ���

		std::vector<float> m_arrData; //! ����
	};
public:
	CWaveDat(CWaveCfg & conf);
	~CWaveDat();

public:
	// �����ļ�
	bool ReadFile(const QString & szFilePathName);

	const CWaveCfg & GetConfig()const
	{
		return m_Conf;
	}
	// ��ȡ�����ļ��ļ�¼����
	int GetDataLineCount() const
	{
		return static_cast<int>(m_arrDataLines.size());
	}
protected:
	CWaveDat() = delete;
	bool LogString(const QString & szLog, int nLevel)const;

	bool ReadAsciiDataFile(const QString & szFilePathName);
	bool ReadBinDataFile(const QString & szFilePathName);
private:
	CWaveCfg & m_Conf;
	//! ԭʼ���ݼ�¼
	std::vector<std::shared_ptr<CDataLine>> m_arrDataLines;
};
/*! \class CComtrade wave_info.h  "wave_info.h"
*  \brief �����ļ���
*  \details �����У������ļ��������ļ�
*/
class CComtrade
{
public:
	CComtrade();
	~CComtrade();
public:
	bool OpenFileGroup(const QString &szFileName);

	QString GetComtradeName()const
	{
		return m_szBaseName;
	}
	const std::shared_ptr<CWaveCfg> & GetConfig()const
	{
		return m_pCfg;
	}
	const std::shared_ptr<CWaveDat> & GetData()const
	{
		return m_pData;
	}
protected:
	bool LogString(const QString & szLog, int nLevel)const;
private:

	std::shared_ptr<CWaveDat> m_pData;

	std::shared_ptr<CWaveCfg> m_pCfg;

	QString  m_szBaseName;  //! �ļ�����ȥ��·����ȥ����׺��

};


#endif // WAVE_INFO_H
