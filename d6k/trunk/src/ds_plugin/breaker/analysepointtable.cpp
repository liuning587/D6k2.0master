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
			//���б���
			QByteArray tLineData = tFile.readLine();
			QString strData = QString::fromLocal8Bit(tLineData);
			if (strData.contains(QStringLiteral("${����ֵ}")))
			{
				//����ֵ
				nCrrentType = ANALOG_VALUE;
				continue;
			}
			else if (strData.contains(QStringLiteral("${��������}")))
			{
				nCrrentType = DEBUG_DATA;
				continue;
			}
			else if (strData.contains(QStringLiteral("${������}")))
			{
				nCrrentType = DI_DATA;
				continue;
			}
			else if (strData.contains(QStringLiteral("${������}")))
			{
				nCrrentType = DO_DATA;
				continue;
			}
			else if (strData.contains(QStringLiteral("${�����ź�}")))
			{
				nCrrentType = ACTION_SIGNAL;
				continue;
			}
			else if (strData.contains(QStringLiteral("${�쳣�ź�}")))
			{
				nCrrentType = ABNORMAL_SIGNAL;
				continue;
			}
			else if (strData.contains(QStringLiteral("${��ѹ���ź�}")))
			{
				nCrrentType = SOFTPLATE_SIGNAL;
				continue;
			}
			else if (strData.contains(QStringLiteral("${������ֵ}")))
			{
				nCrrentType = PROTECT_DEV;
				continue;
			}
			else if (strData.contains(QStringLiteral("${ϵͳ����}")))
			{
				nCrrentType = SYSTEM_PARM;
				continue;
			}
			else if (strData.contains(QStringLiteral("${�����¼�}")))
			{
				nCrrentType = ACTION_EVENT;
				continue;
			}
			else if (strData.contains(QStringLiteral("${�쳣�¼�}")))
			{
				nCrrentType = ABNORMAL_EVENT;
				continue;
			}
			else if (strData.contains(QStringLiteral("${��ѹ���λ�¼�}")))
			{
				nCrrentType = SOFTLATE_CHANGE_EVENT;
				continue;
			}
			else if (strData.contains(QStringLiteral("${�����¼�}")))
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
				//������Ϣ
				AnalyseDebugItems(strData);
			}
			else if (nCrrentType == PROTECT_DEV)
			{
				//������ֵ
				AnalyseProtectDevItems(strData);
			}
			else if (nCrrentType == SYSTEM_PARM)
			{
				//ϵͳ����
				AnalyseSystemDevItems(strData);
			}
			else if (nCrrentType == DI_DATA)
			{
				//������
				AnalyseDIItems(strData);
			}
			else if (nCrrentType == DO_DATA)
			{
				//������
				AnalyseDOItems(strData);
			}
			else if (nCrrentType == ACTION_SIGNAL)
			{
				//�����ź�
				AnayseAction(strData);
			}
			else if (nCrrentType == ABNORMAL_SIGNAL)
			{
				//�쳣�ź�
				AnalyseAbnormal(strData);
			}
			else if (nCrrentType == SOFTPLATE_SIGNAL)
			{
				//��ѹ���ź�
				AnayseSoftDev(strData);
			}
			else if (nCrrentType == ACTION_EVENT)
			{
				//�����¼�
				AnalyseActionEvent(strData);
			}
			else if (nCrrentType == ABNORMAL_EVENT)
			{
				//�쳣�¼�
				AnalyseAbnormalEvent(strData);
			}
			else if (nCrrentType == RUN_EVENT)
			{
				//�����¼�
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

//��������ֵitem
void CAnalysePointTable::AnalyseAnalogItems(const QString &strLineItem)
{
	//�������
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
	//�������
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

//������ֵ
void CAnalysePointTable::AnalyseProtectDevItems(const QString &strLineItem)
{
	//�������
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

//ϵͳ������ֵ
void CAnalysePointTable::AnalyseSystemDevItems(const QString &strLineItem)
{
	//�������
	//�������
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
	//�������
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
	//�������
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
