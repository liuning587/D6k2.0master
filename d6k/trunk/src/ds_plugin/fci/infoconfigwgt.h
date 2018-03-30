#ifndef INFOCONFIGWGT_H
#define INFOCONFIGWGT_H

#include <QDialog>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QLineEdit>

#define  POINTTABLEPATH "/ini/fci/pointtable/"
#define  DEVCONFIGPATH "/ini/fci/devConfig/"  
#define  RECORDPATH "/ini/fci/lbtable/"
#define  REMOTETABLE "/ini/fci/remotetable/"
#define  HISTORYTABLE "/ini/fci/history/"
#define  FTPCONFIGPATH "/ini/fci/ftp/"
#define  PROJECTPATH "/ini/fci/project/"
#define  CMDHISTORYPATH "/ini/fci/cmdhistory/"
#define  RECORDFILEPATH "/ini/fci/lbfile/"
#define  INITFILE "/ini/"

#define  PORT104 "2404"
class QLineEdit;
class QComboBox;
class CCheckRemoteWgt;
class QListWidget;
class QIODevice;
class QListWidgetItem;

class CInfoConfigWgt : public QDialog
{
	Q_OBJECT

public:
	CInfoConfigWgt(QWidget *parent=0);
	void  InitWgt();
	~CInfoConfigWgt();
	//获取IP地址
	QString GetIpAddress();
	//获取端口号
	int GetPort();
	//获取点表名称
	QString GetPointTable();
    //
	bool InitComFile();
	//加载点表
	bool InitCurrentPointTable();
    
	//获取ftp配置文件名称
    QString GetFtpConfigName();
    //获取DeviceID
    int GetDeviceAddr();
    //获取binaryaddr 
    int GetBinaryStartAddr();
	//获取双点遥信地址
	int GetDoubleBinaryStartAddr();
    //获取analogaddr
    int GetAnalogStartAddr();
    //获取kwh
    int GetKwhStartAddr();
    //获取Control
    int GetControlStart();
    //获取文件列表
    QStringList GetProjectFile();
    //初始化已经存在的工程
    void InitProjectList();
    //获取工程名称
    QString GetProjectName();
    //获取总召时间
    int GetGereralTime();
    //获取时间同步时间
    int GetSyncGenertalTime();
    //获取电度召唤时间
    int GetKwhCallTime();
    //解析文件
    bool OpenXmlFile(const QString &fileName);
    //解析
    void AnalyseXmlFile(QIODevice *pDevice);
    //解析down数据
    void AnalyseDownItemData(QXmlStreamReader &xmlReader);
    //写入保存文件
    bool WriteXmlFile(const QString &strFilename);
    //组装基础数据
    void WriteBaseInfo(QXmlStreamWriter &xmlWriter);
    //组装FTP数据
    void WriteFtpInfo(QXmlStreamWriter &xmlWriter);

	//
	int GetTime0()
	{
		return m_pLineEdTime0->text().toInt();
	}

	int GetTime1()
	{
		return m_pLineEdTime1->text().toInt();
	}

	int GetTime2()
	{
		return m_pLineEdTime2->text().toInt();
	}

	int GetTime3()
	{
		return m_pLineEdTime3->text().toInt();
	}

signals:
    void Signal_UpdateFtpConfig(const QString &strFilename);
public slots:
    void Slot_Comform();
	void Slot_Cancel();
    //更新ftp配置
    void Slot_UpdateFtpConfig(const QString &strFilename);
    //点击list
    void Slot_ClickListItem(QListWidgetItem *pItem);
	//list double
	void Slot_lstDoubleClick(QListWidgetItem *pItem);

private:
    //工程名称
    QLineEdit *m_pLineProjectName;
	//ip
	//QLineEdit *m_pLinECom;
	//port
	//QLineEdit *m_pLineBps;	
	//COMID
	QComboBox * m_pComID;	
	//USETYPE
	QComboBox * m_pComUseType;
	//BPS
	QComboBox * m_pComBps;
	QComboBox * m_pComParity;
	//默认参数
	QLineEdit*  m_pLineEditStopBit;
	QLineEdit * m_pLineEditDataBit;
	//使用某类规约
	QComboBox * m_pComProtocol;
	//点表
	QComboBox *m_pComBoxDebiceType;
    
	//规约 信息
	QLineEdit * m_pLinkAddrLen;
	QLineEdit * m_pCotLen;
	QLineEdit * m_pAsduAddrLen;
	QLineEdit * m_pInfoAddrLen;
	QLineEdit * m_pLinkAddr;
	QLineEdit * m_pAsduAddr;
	QLineEdit * m_pResendTimes;

	//装置地址
    QLineEdit *m_pDeviceAddress;
    //遥信起始地址
    QLineEdit *m_pBinaryStartAddr;
	//双点遥信
	QLineEdit *m_pDoubleBinaryStartAddr;
    //遥测起始地址
    QLineEdit *m_pAnalyStartAddr;
    //遥脉起始地址
    QLineEdit *m_pKwhStartAddr;
    //遥控起始地址
    QLineEdit *m_pControlAddr;
    //工程列表
    QListWidget *m_pLstProject;
    //总召时间 
    QLineEdit *m_pLineEdTotalCall;
    //对时时间间隔
    QLineEdit *m_pLineEdCheckTimeCall;
    //遥脉召唤
    QLineEdit *m_pLineEdKwhTimeCall;
	//time0
	QLineEdit *m_pLineEdTime0;
	//time1
	QLineEdit *m_pLineEdTime1;
	//time2
	QLineEdit *m_pLineEdTime2;
	//time3
	QLineEdit *m_pLineEdTime3;
	
};

#endif // INFOCONFIGWGT_H
