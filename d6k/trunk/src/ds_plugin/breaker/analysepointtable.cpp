#include "analysepointtable.h"
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QSettings>

CAnalysePointTable::CAnalysePointTable()
{
	m_nDiIndex = 0;
	m_nDoIndex = 0;
	m_nAction = 0;
	m_nSoft = 0;
	m_nAbnormal = 0;
}


CAnalysePointTable::~CAnalysePointTable()
{
}

void CAnalysePointTable::AnalyseFile(const QString &strFileName)
{
	if (strFileName.isEmpty())
	{
		return;
	}

	QFile tFile(strFileName);

	if (tFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		int nCrrentType = 0;
		while (!tFile.atEnd())
		{
			//单行遍历
			QByteArray tLineData = tFile.readLine();
			QString strData = QString::fromLocal8Bit(tLineData);
			if (strData.contains(QStringLiteral("${测量值}")))
			{
				//测量值
				nCrrentType = ANALOG_VALUE;
				continue;
			}
			else if (strData.contains(QStringLiteral("${调试数据}")))
			{
				nCrrentType = DEBUG_DATA;
				continue;
			}
			else if (strData.contains(QStringLiteral("${开入量}")))
			{
				nCrrentType = DI_DATA;
				continue;
			}
			else if (strData.contains(QStringLiteral("${开出量}")))
			{
				nCrrentType = DO_DATA;
				continue;
			}
			else if (strData.contains(QStringLiteral("${动作信号}")))
			{
				nCrrentType = ACTION_SIGNAL;
				continue;
			}
			else if (strData.contains(QStringLiteral("${异常信号}")))
			{
				nCrrentType = ABNORMAL_SIGNAL;
				continue;
			}
			else if (strData.contains(QStringLiteral("${软压板信号}")))
			{
				nCrrentType = SOFTPLATE_SIGNAL;
				continue;
			}
			else if (strData.contains(QStringLiteral("${保护定值}")))
			{
				nCrrentType = PROTECT_DEV;
				continue;
			}
			else if (strData.contains(QStringLiteral("${系统参数}")))
			{
				nCrrentType = SYSTEM_PARM;
				continue;
			}
			else if (strData.contains(QStringLiteral("${动作事件}")))
			{
				nCrrentType = ACTION_EVENT;
				continue;
			}
			else if (strData.contains(QStringLiteral("${异常事件}")))
			{
				nCrrentType = ABNORMAL_EVENT;
				continue;
			}
			else if (strData.contains(QStringLiteral("${软压板变位事件}")))
			{
				nCrrentType = SOFTLATE_CHANGE_EVENT;
				continue;
			}
			else if (strData.contains(QStringLiteral("${运行事件}")))
			{
				nCrrentType = RUN_EVENT;
				continue;
			}

			if (nCrrentType == ANALOG_VALUE)
			{
				AnalyseAnalogItems(strData);
			}
			else if (nCrrentType == DEBUG_DATA)
			{
				//调试信息
				AnalyseDebugItems(strData);
			}
			else if (nCrrentType == PROTECT_DEV)
			{
				//保护定值
				AnalyseProtectDevItems(strData);
			}
			else if (nCrrentType == SYSTEM_PARM)
			{
				//系统参数
				AnalyseSystemDevItems(strData);
			}
			else if (nCrrentType == DI_DATA)
			{
				//开入量
				AnalyseDIItems(strData);
			}
			else if (nCrrentType == DO_DATA)
			{
				//开出量
				AnalyseDOItems(strData);
			}
			else if (nCrrentType == ACTION_SIGNAL)
			{
				//动作信号
				AnayseAction(strData);
			}
			else if (nCrrentType == ABNORMAL_SIGNAL)
			{
				//异常信号
				AnalyseAbnormal(strData);
			}
			else if (nCrrentType == SOFTPLATE_SIGNAL)
			{
				//软压板信号
				AnayseSoftDev(strData);
			}
			else if (nCrrentType == ACTION_EVENT)
			{
				//动作事件
				AnalyseActionEvent(strData);
			}
			else if (nCrrentType == ABNORMAL_EVENT)
			{
				//异常事件
				AnalyseAbnormalEvent(strData);
			}
			else if (nCrrentType == RUN_EVENT)
			{
				//运行事件
				AnalyseRunEvent(strData);
			}
		}
	}
	else
	{
		QMessageBox::warning(0, QObject::tr("Warning"),QObject::tr("Open %1 file Error,Error Code:%2").arg(strFileName).arg(tFile.errorString()));
	}
}

void CAnalysePointTable::AnalyseRealFile(const QString &strFileName)
{
	if (strFileName.isEmpty())
	{
		return;
	}

	QSettings *configIniRead = new QSettings(strFileName, QSettings::IniFormat);
	//
	m_nDiIndex = configIniRead->value("DI/Index",1).toInt();
	m_nDoIndex = configIniRead->value("DO/Index").toInt();
	m_nAction = configIniRead->value("ACTION/Index").toInt();
	m_nAbnormal = configIniRead->value("/ABNORMAL/Index").toInt();
	m_nSoft = configIniRead->value("SOFT/Index").toInt();
}

//解析测量值item
void CAnalysePointTable::AnalyseAnalogItems(const QString &strLineItem)
{
	//检查数据
	if (strLineItem.split(",").count() != 7)
	{
		return;
	}

	std::shared_ptr<CAnalogPointInfo> tPointInfo = std::make_shared<CAnalogPointInfo>();

	QStringList lstItems = strLineItem.split(",");

	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strType = lstItems[1];
	tPointInfo->m_strName = lstItems[2];
	tPointInfo->m_strUnit = lstItems[3];
	tPointInfo->m_strValue = lstItems[6];
	

	tPointInfo->m_nValueType = lstItems[4].remove("[").toInt();
	tPointInfo->m_nSmallNum = lstItems[5].remove("]").toInt();


	m_mapAnalogIDDes.insert(std::make_pair(tPointInfo->m_nPointNum,tPointInfo));
}

void CAnalysePointTable::AnalyseDebugItems(const QString &strLineItem)
{
	//检查数据
	if (strLineItem.split(",").count() != 7)
	{
		return;
	}

	std::shared_ptr<CAnalogPointInfo> tPointInfo = std::make_shared<CAnalogPointInfo>();

	QStringList lstItems = strLineItem.split(",");

	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strType = lstItems[1];
	tPointInfo->m_strName = lstItems[2];
	tPointInfo->m_strUnit = lstItems[3];
	tPointInfo->m_strValue = lstItems[6];


	tPointInfo->m_nValueType = lstItems[4].remove("[").toInt();
	tPointInfo->m_nSmallNum = lstItems[5].remove("]").toInt();


	m_mapDebugIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

//保护定值
void CAnalysePointTable::AnalyseProtectDevItems(const QString &strLineItem)
{
	//检查数据
	if (strLineItem.split(",").count() < 6)
	{
		return;
	}

	std::shared_ptr<CDevInfo> tPointInfo = std::make_shared<CDevInfo>();

	QStringList lstItems = strLineItem.split(",");

	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strDevType = lstItems[1];
	tPointInfo->m_strDevName = lstItems[2];
	tPointInfo->m_strDevUnit = lstItems[3];
	tPointInfo->m_strDevRange = strLineItem.mid(strLineItem.indexOf("[")+1, strLineItem.indexOf("]")-1 - strLineItem.indexOf("["));

	tPointInfo->m_strValue = lstItems.last().toInt();

	m_mapProtectDevIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

//系统参数定值
void CAnalysePointTable::AnalyseSystemDevItems(const QString &strLineItem)
{
	//检查数据
	//检查数据
	if (strLineItem.split(",").count() < 6)
	{
		return;
	}

	std::shared_ptr<CDevInfo> tPointInfo = std::make_shared<CDevInfo>();

	QStringList lstItems = strLineItem.split(",");

	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strDevType = lstItems[1];
	tPointInfo->m_strDevName = lstItems[2];
	tPointInfo->m_strDevUnit = lstItems[3];
	tPointInfo->m_strDevRange = strLineItem.mid(strLineItem.indexOf("[")+1, strLineItem.indexOf("]")-1- strLineItem.indexOf("["));

	tPointInfo->m_strValue = lstItems.last().toInt();

	m_mapSystemDevIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnalyseDIItems(const QString & strLineItem)
{
	//检查数据
	if (strLineItem.split(",").count() < 3)
	{
		return;
	}

	std::shared_ptr<CDIDOInfo> tPointInfo = std::make_shared<CDIDOInfo>();

	QStringList lstItems = strLineItem.split(",");

	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_nName = lstItems[1];
	tPointInfo->m_nValue = lstItems[2].toInt();

	m_mapDIIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnalyseDOItems(const QString & strLineItem)
{
	//检查数据
	if (strLineItem.split(",").count() < 3)
	{
		return;
	}

	std::shared_ptr<CDIDOInfo> tPointInfo = std::make_shared<CDIDOInfo>();

	QStringList lstItems = strLineItem.split(",");

	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_nName = lstItems[1];
	tPointInfo->m_nValue = lstItems[2].toInt();

	m_mapDOIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnayseAction(const QString & strLineItem)
{
	if (strLineItem.split(",").count() < 3)
	{
		return;
	}

	std::shared_ptr<CSOFTSTRAP> tPointInfo = std::make_shared<CSOFTSTRAP>();

	QStringList lstItems = strLineItem.split(",");

	QString strAllNum = lstItems[0];
	tPointInfo->m_nPointNum = strAllNum.split("(").first().toInt();
	tPointInfo->m_nOwnId = strAllNum.split("(").last().remove(")").toInt();

	tPointInfo->m_strName = lstItems[1];
	tPointInfo->m_nVlaue = lstItems[1].toInt();


	m_mapActionIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnalyseAbnormal(const QString &strLineItem)
{
	if (strLineItem.split(",").count() < 3)
	{
		return;
	}

	std::shared_ptr<CSOFTSTRAP> tPointInfo = std::make_shared<CSOFTSTRAP>();

	QStringList lstItems = strLineItem.split(",");

	QString strAllNum = lstItems[0];
	tPointInfo->m_nPointNum = strAllNum.split("(").first().toInt();
	tPointInfo->m_nOwnId = strAllNum.split("(").last().remove(")").toInt();

	tPointInfo->m_strName = lstItems[1];
	tPointInfo->m_nVlaue = lstItems[1].toInt();


	m_mapAbNormalIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnayseSoftDev(const QString & strLineItem)
{
	if (strLineItem.split(",").count() < 3)
	{
		return;
	}

	std::shared_ptr<CSOFTSTRAP> tPointInfo = std::make_shared<CSOFTSTRAP>();

	QStringList lstItems = strLineItem.split(",");

	QString strAllNum = lstItems[0];
	tPointInfo->m_nPointNum = strAllNum.split("(").first().toInt();
	tPointInfo->m_nOwnId = strAllNum.split("(").last().remove(")").toInt();

	tPointInfo->m_strName = lstItems[1];
	tPointInfo->m_nVlaue = lstItems[1].toInt();


	m_mapSoftIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnalyseActionEvent(const QString &strLineItem)
{
	if (strLineItem.split(",").count() < 2)
	{
		return;
	}

	std::shared_ptr<CSOFTSTRAP> tPointInfo = std::make_shared<CSOFTSTRAP>();

	QStringList lstItems = strLineItem.split(",");

	QString strAllNum = lstItems[0];
	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strName = lstItems[1];

	m_mapActionEventIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnalyseAbnormalEvent(const QString &strLineItem)
{
	if (strLineItem.split(",").count() < 2)
	{
		return;
	}

	std::shared_ptr<CSOFTSTRAP> tPointInfo = std::make_shared<CSOFTSTRAP>();

	QStringList lstItems = strLineItem.split(",");

	QString strAllNum = lstItems[0];
	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strName = lstItems[1];

	m_mapAbnormalEventIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}

void CAnalysePointTable::AnalyseRunEvent(const QString &strLineItem)
{
	if (strLineItem.split(",").count() < 2)
	{
		return;
	}

	std::shared_ptr<CSOFTSTRAP> tPointInfo = std::make_shared<CSOFTSTRAP>();

	QStringList lstItems = strLineItem.split(",");

	QString strAllNum = lstItems[0];
	tPointInfo->m_nPointNum = lstItems[0].toInt();
	tPointInfo->m_strName = lstItems[1];

	m_mapRunEventIDDes.insert(std::make_pair(tPointInfo->m_nPointNum, tPointInfo));

}
