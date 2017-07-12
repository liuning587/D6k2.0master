#include "analogwgt.h"
#include "breaker_module.h"
#include "analysepointtable.h"
#include "breaker_data.h"
#include "realtimediwgt.h"
#include "realtimedowgt.h"
#include "realtimeactionwgt.h"
#include "realtimeabnormalwgt.h"
#include "realtimesoftwgt.h"
#include <QHeaderView>
#include <QDateTime>

CAnalogWgt::CAnalogWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	InitData();
}

void CAnalogWgt::InitData()
{
	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetAnalogInfo();
	
	for (auto item : pPointTable)
	{
		int nRow = ui.tableWidget->rowCount();

		QTableWidgetItem *pItem0 = new QTableWidgetItem;
		pItem0->setText(QString::number(nRow+1));

		QTableWidgetItem *pItem1 = new QTableWidgetItem;
		pItem1->setText(QString::number(item.second->m_nPointNum));

		QTableWidgetItem *pItem2 = new QTableWidgetItem;
		pItem2->setText(item.second->m_strType);

		QTableWidgetItem *pItem3 = new QTableWidgetItem;
		pItem3->setText(item.second->m_strName);

		QTableWidgetItem *pItem4 = new QTableWidgetItem;
		pItem4->setText(item.second->m_strUnit);

		QTableWidgetItem *pItem5 = new QTableWidgetItem;
		pItem5->setText(item.second->m_strValue);

		QTableWidgetItem *pItem6 = new QTableWidgetItem;

		ui.tableWidget->insertRow(nRow);
		ui.tableWidget->setItem(nRow, 0, pItem0);
		ui.tableWidget->setItem(nRow, 1, pItem1);
		ui.tableWidget->setItem(nRow, 2, pItem2);
		ui.tableWidget->setItem(nRow, 3, pItem3);
		ui.tableWidget->setItem(nRow, 4, pItem4);
		ui.tableWidget->setItem(nRow, 5, pItem5);
		ui.tableWidget->setItem(nRow, 6, pItem6);

		m_IdItem.insert(std::make_pair(item.second->m_nPointNum, pItem5));
	}
}

CAnalogWgt::~CAnalogWgt()
{
}

void CAnalogWgt::Slot_RecvNewRealTimeData(DBG_GET_MEAS & tMeas)
{
	int nDataNum = tMeas.msgLeg.GetAddr() / sizeof(DBG_GET_MEAS::MEAS_DATA);

	if (nDataNum > 0)
	{
		int *nType = (int *)tMeas.m_data[0].measData;
		emit Signal_SeoInfo(*nType);
	}

	const std::map<int, std::shared_ptr<CAnalogPointInfo> > &pPointTable = GetBreakerModuleApi()->GetPointTable()->GetAnalogInfo();


	for (int i = 1; i < nDataNum; i++)
	{
		if (pPointTable.find(tMeas.m_data[i].infoaddr) != pPointTable.end())
		{
			if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_INT)
			{
				//int ��������
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					QString strValue = QString::number(*(int *)tMeas.m_data[i].measData);
					//��������
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;

					if (nSmallNum != 0)
					{
						strValue = QString::number(strValue.toInt()*1.0 / (nSmallNum * 10.0), 'g', strValue.length());
					}

					m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
				}
			}
			else if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_UINT)
			{
				//�޷�������
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					QString strValue = QString::number(*(unsigned int *)tMeas.m_data[i].measData);
					//��������
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;

					if (nSmallNum != 0)
					{
						strValue = QString::number(strValue.toInt()*1.0 / (nSmallNum * 10.0), 'g', strValue.length());
					}

					m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
				}
			}
			else if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_HEX)
			{
				//hex��������
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					QString strValue = QString::number(*(int *)tMeas.m_data[i].measData, 16);
					strValue = "0x" + strValue;
					//��������
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;

					for (int i = 0; i < nSmallNum; i++)
					{
						if (i == 0)
						{
							strValue += ".";
						}
						strValue += "0";
					}

					m_IdItem[tMeas.m_data[i].infoaddr]->setText(strValue);


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

					if (GetBreakerModuleApi()->GetPointTable()->GetDiIndex() == tMeas.m_data[i].infoaddr)
					{
						//����ʵʱ������
						GetBreakerModuleApi()->GetRealDiWgt()->UpdateItem(*(int *)tMeas.m_data[i].measData);
					}
					else if (GetBreakerModuleApi()->GetPointTable()->GetDoIndex() == tMeas.m_data[i].infoaddr)
					{
						// ����ʵʱ������
						GetBreakerModuleApi()->GetRealDoWgt()->UpdateItem(*(int *)tMeas.m_data[i].measData);
					}
					else if (GetBreakerModuleApi()->GetPointTable()->GetAction() == tMeas.m_data[i].infoaddr)
					{
						//���¶����ź�
						GetBreakerModuleApi()->GetRealAction()->UpdateItem(*(int *)tMeas.m_data[i].measData);
					}
					else if (GetBreakerModuleApi()->GetPointTable()->GetAbnormal() == tMeas.m_data[i].infoaddr)
					{
						//�����쳣�ź�
						GetBreakerModuleApi()->GetAbnormalWgt()->UpdateItem(*(int *)tMeas.m_data[i].measData);

					}
					else if (GetBreakerModuleApi()->GetPointTable()->GetSoftIndex() == tMeas.m_data[i].infoaddr)
					{
						//������ѹ��״̬
						GetBreakerModuleApi()->GetRealSoftWgt()->UpdateItem(*(int *)tMeas.m_data[i].measData);
					}
				}
			}
			else if (pPointTable.at(tMeas.m_data[i].infoaddr)->m_nValueType == MEASURE_PARAM_FLOAT)
			{
				//������
				if (m_IdItem.find(tMeas.m_data[i].infoaddr) != m_IdItem.end())
				{
					float fValue = *(float *)tMeas.m_data[i].measData;
					//��������
					int nSmallNum = pPointTable.at(tMeas.m_data[i].infoaddr)->m_nSmallNum;


					m_IdItem[tMeas.m_data[i].infoaddr]->setText(QString::number(fValue, 'g', nSmallNum));


					ui.tableWidget->item(m_IdItem[tMeas.m_data[i].infoaddr]->row(), 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
				}
			}

		}

	}
}
