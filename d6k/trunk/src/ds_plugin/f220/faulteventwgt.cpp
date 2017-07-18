#include "faulteventwgt.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"

CFaultEventWgt::CFaultEventWgt(CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Q_UNUSED(pPointInfo);
    Q_UNUSED(pConfgWgt);

    Q_ASSERT(pCommunicate);
    if (pCommunicate == nullptr)
    {
        return;
    }

    m_pCommunicate = pCommunicate;

    ui.tableWidget->setColumnWidth(0, 100);
    ui.tableWidget->setColumnWidth(1, 100);
    ui.tableWidget->setColumnWidth(2, 100);
    ui.tableWidget->setColumnWidth(3, 100);
    ui.tableWidget->setColumnWidth(4, 100);
    ui.tableWidget->setColumnWidth(5, 200);
    ui.tableWidget->verticalHeader()->setHidden(true);
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->horizontalHeader()->setSelectionBehavior(QAbstractItemView::SelectRows);
//     ui.tableWidget->setColumnHidden(6, true);
    ui.treeWidget->setHidden(true);
    

    connect(m_pCommunicate, SIGNAL(Signal_MalFuction(ASDUGZ)), this, SLOT(Slot_MalFuction(ASDUGZ)));

//     QMap<int, int> ttt;
//     ttt.insert(3344,311);
//     ttt.insert(6677, 44511);
//     ttt.insert(6543, 3445);
//     ttt.insert(3344, 311);
// 
//     Slot_MalFuction(1,2,3,"2016-10-26 326",1,ttt);
}

CFaultEventWgt::~CFaultEventWgt()
{

}

void CFaultEventWgt::Slot_MalFuction(ASDUGZ gzData)
{
    //往表格里插入数据
    int nRow = ui.tableWidget->rowCount();

    ui.tableWidget->insertRow(nRow);
    
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(QString::number(nRow+1));
    ui.tableWidget->setItem(nRow, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(QString::number(gzData.m_BinayNum));
    ui.tableWidget->setItem(nRow, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem;
    
    ui.tableWidget->setItem(nRow, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem;
    item3->setText(QString::number(gzData.m_BinaryAddr.GetAddr()));
    ui.tableWidget->setItem(nRow, 3, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem;
	if (gzData.m_BinaryType !=3)
	{
		//单点
		if (gzData.m_BinaryValue == 1)
		{
			item4->setText(tr("ON"));
		}
		else
		{
			item4->setText(tr("OFF"));
		}

		item2->setText(tr("Single"));
	}
	else
	{
		//双点
		if (gzData.m_BinaryValue == 1)
		{
			item4->setText(tr("OFF"));
		}
		else
		{
			item4->setText(tr("ON"));
		}

	}
    //item4->setText(QString::number(gzData.m_BinaryValue));
    ui.tableWidget->setItem(nRow, 4, item4);

    QTableWidgetItem *item5 = new QTableWidgetItem;
    item5->setText(gzData.m_BinaryTime.Dump());
    ui.tableWidget->setItem(nRow, 5, item5);

	QTableWidgetItem *item6 = new QTableWidgetItem;
	item6->setText(QString::number(gzData.m_AnaloyNum));
	ui.tableWidget->setItem(nRow, 6, item6);

	QTableWidgetItem *item7 = new QTableWidgetItem;
	item7->setText(QString::number(gzData.m_AnalogType));
	ui.tableWidget->setItem(nRow, 7, item7);

	QString strData;
	QStringList lstTitle;
	lstTitle << "UAB" << "UBC" << "UCA" << "U0" << "IA" << "IB" << "IC" << "I0";

	int nDNum = lstTitle.count() > 8 ? 8 : lstTitle.count();

	if (gzData.m_AnalogType == 21)
	{
		SIGNAL_DZ_U *gziData = (SIGNAL_DZ_U *)(gzData.m_pGzDats);
		//short
		for (int i = 0; i<nDNum; i++)
		{
			strData += lstTitle[i] + ":" + QString::number(gziData->m_data[i].m_AnalogValue) + "     ";
		}
	}
	else if (gzData.m_AnalogType == 13)
	{
		SIGNAL_DZ_F *gziData = (SIGNAL_DZ_F *)(gzData.m_pGzDats);
		//float
		for (int i = 0; i<nDNum; i++)
		{
			char *pfStart = gziData->m_data[i].m_AnalogValue;
			qDebug() << QByteArray(pfStart,4).toHex();

			float tt = 0.0;
			*(uint32_t *)(&tt) = (unsigned char)pfStart[0] + (unsigned char)pfStart[1] * 0x100 + (unsigned char)pfStart[2] * 0x10000 + (unsigned char)pfStart[3] * 0x1000000;


			strData += lstTitle[i] + ":" + QString::number(tt) + "     ";
		}

	}



	//for (int i=0; i<ASDUGZ::MAX_DATA_PER_ASDU_GZ; i++)
	//{
	//	strData += lstTitle[i] + ":"  + QString::number(gzData.m_data[i].m_AnalogValue) + "     ";
	//}


    QTableWidgetItem *item8 = new QTableWidgetItem;
    item8->setText(strData);
    ui.tableWidget->setItem(nRow, 8, item8);

    ui.tableWidget->scrollToBottom();
}
