#include "analysepointtable.h"
#include <QFile>
#include <QMessageBox>
#include <QByteArray>

CAnalysePointTable::CAnalysePointTable()
{
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
		}
	}
	else
	{
		QMessageBox::warning(0, QObject::tr("Warning"),QObject::tr("Open %1 file Error,Error Code:%2").arg(strFileName).arg(tFile.errorString()));
	}
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


	m_mapIDDes.insert(std::make_pair(tPointInfo->m_nPointNum,tPointInfo));
}
