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
		}
	}
	else
	{
		QMessageBox::warning(0, QObject::tr("Warning"),QObject::tr("Open %1 file Error,Error Code:%2").arg(strFileName).arg(tFile.errorString()));
	}
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


	m_mapIDDes.insert(std::make_pair(tPointInfo->m_nPointNum,tPointInfo));
}
