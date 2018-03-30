#ifndef WAVE_INFO_H
#define WAVE_INFO_H 

#include <string>
#include <QString>
#include <memory>
#include <vector>
#include <functional>


/*! \class CChannel wave_info.h  "wave_info.h"
*  \brief 通道基类
*  \details 包含有：
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
	// 推送测值进通道
	virtual void PushSampValue(float fValue)
	{
		m_arrSampData.emplace_back(fValue);
	}
	// 获取采样的原始值
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
	std::vector<float> m_arrSampData; //! 采样数据
};
/*! \class CAnaChannel wave_info.h  "wave_info.h"
*  \brief 模拟通道
*  \details 包含有： 
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
	int m_nIndex;               //! 通道索引 （必需字段）
	std::string  m_szID;        //! 通道识别符 （必需字段）
	std::string  m_szPhase;     //! 通道相位特征 （必需字段）
	std::string  m_szCCBM;      //! 被监视的回路元件
	std::string  m_szUnit;      //! 单位
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
		LI1_PHASE = 2, //!< 通道标识符
		LI1_CCBM,
		LI1_UU,
		LI1_A,       //!< 变换因子
		LI1_B,
		LI1_SKEW,   //!< 时间偏移
		LI1_MIN,    //!< 采样记录中数据最小值
		LI1_MAX,    //!< 采样记录中数据最大值
		LI1_ANA_MAX_COUNT
	};

	enum LineIndex2
	{
		LI2_INDEX = 0,
		LI2_ID = 1,
		LI2_PHASE = 2, //!< 通道标识符
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
*  \brief 数字通道
*  \details 包含有：
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
	int m_nIndex;               //! 通道索引 （必需字段）
	std::string  m_szID;        //! 通道识别符 （必需字段）
	std::string  m_szPhase;     //! 通道相位特征 （必需字段）
	std::string  m_szCCBM;      //! 被监视的回路元件
	int m_nY;

	enum LineIndex
	{
		LI_INDEX = 0,
		LI_ID = 1,
		LI_PHASE = 2, //!< 通道标识符
		LI_CCBM,
		LI_Y,
		LI_DIGI_MAX_COUNT
	};
};
/*! \class CWaveCfg wave_info.h  "wave_info.h"
*  \brief 配置文件
*  \details 包含有：
*/
class CWaveCfg
{
public:
	CWaveCfg();
	~CWaveCfg();

	enum FileType
	{
		FT_ASCII = 0,   //!< ASC类型
		FT_BINARY = 1   //!< BIN类型
	};

	struct SampRate
	{
		float Samp;     //! 以HZ为单位的采样率
		int   EndSamp;  //! 在该采样率下最后一次采样数
	};

public:
	// 加载文件
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
	// 数据文件中采样率采样率
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
	std::string  m_szStationName; //! 站名
	std::string  m_szRecDevID;    //! 设备编号 （非必需字段）
	std::string  m_szRevYear;     //! 标准发布年份 （非必需字段）

	FileType     m_nFileType;     //! 文件类型

	int m_nTotoalChannelCount;    //! 通道总数
	int m_nAChannelCount;         //! 模拟通道数量
	int m_nDChannelCount;         //! 数字通道数量

	float m_fLineFreq;           //! 线路频率

	int   m_nSampCount;          //! 数据文件中采样率数字
	std::vector<SampRate>  m_arrSampRates;

	float  m_fTimeMult;          //! 时间标记乘数系数

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
		LI_MULT_OFFSET = 4,  //! 时间标记乘数系数 从采样开始时间 LI_DT1_OFFSET 偏移

		kMaxChannelCount = 999999,
	};

	std::vector<std::shared_ptr<CAnaChannel>> m_arrAnaChannelInfos;

	std::vector<std::shared_ptr<CDigitalChannel>> m_arrDigiChannelInfos;
};
/*! \class CWaveDat wave_info.h  "wave_info.h"
*  \brief 数据文件
*  \details 包含有：
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
		int m_nIndex;       //! 采样数
		int m_nTimeStamp;   //! 时间戳

		std::vector<float> m_arrData; //! 数据
	};
public:
	CWaveDat(CWaveCfg & conf);
	~CWaveDat();

public:
	// 加载文件
	bool ReadFile(const QString & szFilePathName);

	const CWaveCfg & GetConfig()const
	{
		return m_Conf;
	}
	// 获取波形文件的记录总数
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
	//! 原始数据记录
	std::vector<std::shared_ptr<CDataLine>> m_arrDataLines;
};
/*! \class CComtrade wave_info.h  "wave_info.h"
*  \brief 波形文件组
*  \details 包含有：配置文件、波形文件
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

	QString  m_szBaseName;  //! 文件名，去除路径，去除后缀名

};


#endif // WAVE_INFO_H
