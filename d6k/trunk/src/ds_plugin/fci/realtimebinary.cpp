#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include "realtimebinary.h"
#include "commthread.h"
#include "fcifile.h"
#include "infoconfigwgt.h"

CRealTimeBinary::CRealTimeBinary(QWidget *parent, CCommThread *pCommunicate, CFciFile *pPointInfo, CInfoConfigWgt *pConfgWgt)
	: QWidget(parent),
	m_pCommunicate(NULL),
	m_pTableView(NULL),
	m_pTableModel(NULL)
{
	m_iCurrentPointsNum = 0;
	//双点
	m_nICurrentDoubelPointNum = 0;

	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}

	m_pFciFile = pPointInfo;
    m_pCommunicate = pCommunicate;
    m_pConfgWgt = pConfgWgt;

	InitWidget();

	InitData();
}

void CRealTimeBinary::InitWidget()
{
	QTabWidget *pTabWgt = new QTabWidget(this);

	//单点
	m_pTableView = new QTableView(this);
	m_pTableModel = new QStandardItemModel(this);
	m_pTableView->setModel(m_pTableModel);

	m_pTableView->verticalHeader()->setHidden(true);
	m_pTableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QStringList lstHorizonLables;
	
	lstHorizonLables << tr("Num") << tr("InnerAddr") << tr("Address") << tr("Name") << tr("Desc")<< tr("Value") << (tr("Time"));

	m_pTableModel->setHorizontalHeaderLabels(lstHorizonLables);


	//双点
	m_pDoubleTableView = new QTableView(this);
	m_pDoubleTableModel = new QStandardItemModel(this);
	m_pDoubleTableView->setModel(m_pDoubleTableModel);

	m_pDoubleTableView->verticalHeader()->setHidden(true);
	m_pDoubleTableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	m_pDoubleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pDoubleTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pDoubleTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_pDoubleTableModel->setHorizontalHeaderLabels(lstHorizonLables);


	pTabWgt->addTab(m_pTableView, tr("单点"));
	pTabWgt->addTab(m_pDoubleTableView, tr("双点"));

	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(pTabWgt);



	connect(m_pCommunicate, SIGNAL(Signal_OnePointRemote(int, int, int)), this, SLOT(Slot_AnalyseoneBinaryData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_onePointDisRemote(int, int, int)), this, SLOT(Slot_AnalyseoneBinaryData(int, int, int)));

	connect(m_pCommunicate, SIGNAL(Signal_DoublePointRemote(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));

	connect(m_pCommunicate, SIGNAL(Signal_BinaryGroupPoint(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));


}

void CRealTimeBinary::InitData()
{
	m_iCurrentPointsNum = m_pFciFile->GetYxNum();

	std::vector<RPT_DEF> vecs;
	bool bRet = m_pFciFile->GetYXArryInfo(vecs);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return;
	}

	for each (RPT_DEF var in vecs)
	{
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
		// 远动序号
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(var->OccNO)));
		//装置序号
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem(var->InnerAddr));
		//描述
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(var->Address));
		//点值
		m_pTableModel->setItem(iRowNum, 3, new QStandardItem(var->Name));
		//遥信类型  单点  双点
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(var->Desccription));
		//当前时间
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(""));

		m_pTableModel->setItem(iRowNum, 6, new QStandardItem(""));
		//记录数据
		bool bRet = false;

		m_mapIdItemObj.insert(var->Address.toInt(&bRet,16), m_pTableModel->item(iRowNum, 0));
	}

	
	
	for each (RPT_DEF var in vecs)
	{
		//获取当前的行数
		int iRowNum = m_pDoubleTableModel->rowCount();
		// 远动序号
		m_pDoubleTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(var->OccNO)));
		//装置序号
		m_pDoubleTableModel->setItem(iRowNum, 1, new QStandardItem(var->InnerAddr));
		//描述
		m_pDoubleTableModel->setItem(iRowNum, 2, new QStandardItem(var->Address));
		//点值
		m_pDoubleTableModel->setItem(iRowNum, 3, new QStandardItem(var->Name));
		//遥信类型  单点  双点
		m_pDoubleTableModel->setItem(iRowNum, 4, new QStandardItem(var->Desccription));
		//当前时间
		m_pDoubleTableModel->setItem(iRowNum, 5, new QStandardItem(""));

		m_pDoubleTableModel->setItem(iRowNum, 6, new QStandardItem(""));
		//记录数据
		bool bRet = false;

		m_mapDoubleIdItemObj.insert(var->Address.toInt(&bRet, 16), m_pDoubleTableModel->item(iRowNum, 0));
	}
}


CRealTimeBinary::~CRealTimeBinary()
{

}


void CRealTimeBinary::AnalyseRealBinaryData(int iDeviceID, int nPointID, int nValue, const QString &strType)
{
    //过滤点  去除不需要的点

	Q_UNUSED(iDeviceID);
	Q_ASSERT(m_pFciFile);

	if (strType.toInt() == eSignal)
	{
		//单点
		nPointID = nPointID;// -m_pConfgWgt->GetBinaryStartAddr();

		if (m_pFciFile == NULL)
		{
			return;
		}


		m_iCurrentPointsNum = m_pFciFile->GetYxNum();

		if (m_mapIdItemObj.contains(nPointID))
		{
			//更新
			int iUpdateRow = m_mapIdItemObj[nPointID]->row();
			m_pTableModel->item(iUpdateRow, 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

			if (strType.toInt() == eSignal)
			{
				QString strShow;
				if (nValue == 0)
				{
					strShow = tr("OFF");
					m_pTableModel->item(iUpdateRow, 5)->setForeground(QColor("red"));
				}
				else if (nValue == 1)
				{
					strShow = tr("ON");
					m_pTableModel->item(iUpdateRow, 5)->setForeground(QColor("black"));
				}
				m_pTableModel->item(iUpdateRow, 5)->setText(strShow);
				//m_pTableModel->item(iUpdateRow, 4)->setText(tr("Single"));

			}
			else if (strType.toInt() == eDouble)
			{
				QString strShow;
				if (nValue == 0)
				{
					strShow = tr("Unknown");
				}
				else if (nValue == 1)
				{
					strShow = tr("OFF");
					m_pTableModel->item(iUpdateRow, 5)->setForeground(QColor("red"));
				}
				else if (nValue == 2)
				{
					strShow = tr("ON");
					m_pTableModel->item(iUpdateRow, 5)->setForeground(QColor("black"));
				}
				else if (nValue == 3)
				{
					strShow = tr("Invaild");
				}
				m_pTableModel->item(iUpdateRow, 5)->setText(strShow);
				//m_pTableModel->item(iUpdateRow, 4)->setText(tr("Double"));

			}
			else if (strType.toInt() == eSignalGroup)
			{
				m_pTableModel->item(iUpdateRow, 5)->setText(QString::number(nValue));
				//m_pTableModel->item(iUpdateRow, 4)->setText(tr("Group"));

			}
		}
		else
		{
			//插入
			//获取当前的行数
		/*	int iRowNum = m_pTableModel->rowCount();
			//插入数据
			// 远动序号
			m_pTableModel->setItem(iRowNum, 0, new QStandardItem((mapPoint[nPointID].Address)));
			//装置序号
			m_pTableModel->setItem(iRowNum, 1, new QStandardItem((mapPoint[nPointID].InnerAddr)));
			//描述
			m_pTableModel->setItem(iRowNum, 2, new QStandardItem(mapPoint[nPointID].Desccription));
			//点值
			m_pTableModel->setItem(iRowNum, 3, new QStandardItem(QString::number(nValue)));
			//遥信类型  单点  双点
			m_pTableModel->setItem(iRowNum, 4, new QStandardItem(strType));
			//当前时间
			m_pTableModel->setItem(iRowNum, 5, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
			//记录数据
			m_mapIdItemObj.insert(nPointID, m_pTableModel->item(iRowNum, 0));*/
		}

	}
	else
	{
		//双点
		nPointID = nPointID;// -m_pConfgWgt->GetDoubleBinaryStartAddr();

		if (m_pFciFile == NULL)
		{
			return;
		}


		m_nICurrentDoubelPointNum = m_pFciFile->GetYxNum();
		

		//QMap<unsigned int, RPT> mapPoint = m_pFciFile->GetDoubleBinaryMap();

		if (m_mapDoubleIdItemObj.contains(nPointID))
		{
			//更新
			int iUpdateRow = m_mapDoubleIdItemObj[nPointID]->row();
	
			m_pDoubleTableModel->item(iUpdateRow, 6)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

			if (strType.toInt() == eSignal)
			{
				QString strShow;
				if (nValue == 0)
				{
					strShow = tr("OFF");
					m_pDoubleTableModel->item(iUpdateRow, 5)->setForeground(QColor("red"));
				}
				else if (nValue == 1)
				{
					strShow = tr("ON");
					m_pDoubleTableModel->item(iUpdateRow, 5)->setForeground(QColor("black"));
				}
				m_pDoubleTableModel->item(iUpdateRow, 5)->setText(strShow);
			//	m_pDoubleTableModel->item(iUpdateRow, 4)->setText(tr("Single"));

			}
			else if (strType.toInt() == eDouble)
			{
				QString strShow;
				if (nValue == 0)
				{
					strShow = tr("Unknown");
				}
				else if (nValue == 1)
				{
					strShow = tr("OFF");
					m_pDoubleTableModel->item(iUpdateRow, 5)->setForeground(QColor("red"));
				}
				else if (nValue == 2)
				{
					strShow = tr("ON");
					m_pDoubleTableModel->item(iUpdateRow, 5)->setForeground(QColor("black"));
				}
				else if (nValue == 3)
				{
					strShow = tr("Invaild");
				}
				m_pDoubleTableModel->item(iUpdateRow, 5)->setText(strShow);
		//		m_pDoubleTableModel->item(iUpdateRow, 4)->setText(tr("Double"));

			}
			else if (strType.toInt() == eSignalGroup)
			{
				m_pDoubleTableModel->item(iUpdateRow, 5)->setText(QString::number(nValue));
		//		m_pDoubleTableModel->item(iUpdateRow, 4)->setText(tr("Group"));

			}
		}
		else
		{
			//插入
			//获取当前的行数
			int iRowNum = m_pDoubleTableModel->rowCount();
			//插入数据
		/*	// 远东序号
			m_pDoubleTableModel->setItem(iRowNum, 0, new QStandardItem((mapPoint[nPointID].Address)));
			//装置序号
			m_pDoubleTableModel->setItem(iRowNum, 1, new QStandardItem((mapPoint[nPointID].InnerAddr)));
			//描述
			m_pDoubleTableModel->setItem(iRowNum, 2, new QStandardItem(mapPoint[nPointID].Desccription));
			//点值
			m_pDoubleTableModel->setItem(iRowNum, 3, new QStandardItem(QString::number(nValue)));
			//遥信类型  单点  双点
			m_pDoubleTableModel->setItem(iRowNum, 4, new QStandardItem(strType));
			//当前时间
			m_pDoubleTableModel->setItem(iRowNum, 5, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
			//记录数据
			m_mapIdItemObj.insert(nPointID, m_pDoubleTableModel->item(iRowNum, 0));*/
		}
	}
}

void CRealTimeBinary::Slot_AnalyseoneBinaryData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealBinaryData(iDeviceID, nPointID, nValue, QString::number(eSignal));
}

void CRealTimeBinary::Slot_AnalyseDoubleBinaryData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealBinaryData(iDeviceID, nPointID, nValue, QString::number(eDouble));
}

void CRealTimeBinary::Slot_AnalyseGroupBinaryData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealBinaryData(iDeviceID, nPointID, nValue, QString::number(eSignalGroup));
}

