#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QItemSelectionModel>
#include "realtimeanalog.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"
#include "fcifile.h"

CAnalogZoneWgt::CAnalogZoneWgt(QWidget *parent /*= 0*/)
{
    Q_UNUSED(parent);
	setWindowFlags(Qt::WindowCloseButtonHint);
    InitWgt();
}

void CAnalogZoneWgt::InitWgt()
{
    m_pLineEdZone = new QLineEdit(this);
    m_pLineEdZone->setFixedWidth(200);

    m_pPushComform = new QPushButton(this);
    m_pPushComform->setText(tr("Conform"));

    m_pComboQpm = new QComboBox(this);
    m_pComboQpm->setFixedWidth(200);

    QStringList lstData;
    lstData << tr("UnUsed") << tr("Limit") << tr("Smooth") << tr("Upper") << tr("Lower");
    m_pComboQpm->addItems(lstData);

    //布局
    QLabel *pZone = new QLabel(this);
    pZone->setText(tr("ZoneData"));

    QHBoxLayout *pZoneLayout = new QHBoxLayout;
    pZoneLayout->addWidget(pZone);
    pZoneLayout->addWidget(m_pLineEdZone);

    QLabel *pLableQpm = new QLabel(this);
    pLableQpm->setText(tr("QPM"));

    QHBoxLayout *pQpmLayout = new QHBoxLayout;
    pQpmLayout->addWidget(pLableQpm);
    pQpmLayout->addWidget(m_pComboQpm);

    QHBoxLayout *pConformLayout = new QHBoxLayout;
    pConformLayout->addStretch();
    pConformLayout->addWidget(m_pPushComform);

    QVBoxLayout *pWgtLayout = new QVBoxLayout(this);
    pWgtLayout->addLayout(pZoneLayout);
    pWgtLayout->addLayout(pQpmLayout);
    pWgtLayout->addLayout(pConformLayout);

    connect(m_pPushComform,SIGNAL(clicked()),this,SLOT(Slot_PushComform()));
}


CAnalogZoneWgt::~CAnalogZoneWgt()
{

}

float CAnalogZoneWgt::GetZoneData()
{
    return m_pLineEdZone->text().toFloat();
}
int CAnalogZoneWgt::GetQpmData()
{
    return m_pComboQpm->currentIndex();
}


void CAnalogZoneWgt::Slot_PushComform()
{
    if (m_pLineEdZone->text().toFloat() < 0)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Zone Data input Error!"));
        return;
    }

    emit Signal_Comform();

    this->close();
}


CRealTimeAnalog::CRealTimeAnalog(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CFciFile *pChooseWgt, CInfoConfigWgt *pConfgWgt)
	: QWidget(parent),
	m_pCommunicate(NULL),
	m_pTableView(NULL),
	m_pTableModel(NULL)
{
	m_iCurrentPointsNum = 0;

	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}
	m_pPointInfo = pPointInfo;
	m_pCommunicate = pCommunicate;

    m_pFile = pChooseWgt;

    m_pConfgWgt = pConfgWgt;
    m_pZoneWgt = new CAnalogZoneWgt;

	InitWidget();

	InitData();
}

void CRealTimeAnalog::InitWidget()
{
	m_pTableView = new QTableView(this);
	m_pTableModel = new QStandardItemModel(this);
	m_pTableView->setModel(m_pTableModel);
    
	m_pTableView->verticalHeader()->setHidden(true);
	m_pTableView->horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);

	m_pTableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	
	QStringList lstHorizonLables;

	
	lstHorizonLables << tr("Num") << tr("InnerAddr") << tr("Address") << tr("Name") << tr("Desc")<<tr("Value")
		<< tr("KRate") << tr("Unit")<<tr("Time");

	m_pTableModel->setHorizontalHeaderLabels(lstHorizonLables);
    
	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(m_pTableView);

    m_pTableView->setColumnHidden(3, true);

    //归一化
	connect(m_pCommunicate, SIGNAL(Signal_AnalogNormal(int, int, float, int)), this, SLOT(Slot_AnalyseNormalAnalogData(int, int,float, int)));
    //标度化
    connect(m_pCommunicate, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)), this, SLOT(Slot_AnalyseScaledAnalogData(int, int, float, int)));
    //浮点
	connect(m_pCommunicate, SIGNAL(Signal_AnalogShortFloat(int, int, float, int)), this, SLOT(Slot_AnalyseShortFloatAnalogData(int, int, float, int)));
    connect(m_pCommunicate, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SLOT(Slot_ZoneFeedback(int, float, int, int)));

    //右击菜单
    m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTableView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
    
    connect(m_pZoneWgt, SIGNAL(Signal_Comform()), this, SLOT(Slot_ZoomArea()));
}


void CRealTimeAnalog::AnalyseRealAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty, const QString &strType, int nAnalogFlag)
{
	Q_UNUSED(iQuilty);
	Q_UNUSED(iDeviceID);
	//过滤点  去除不需要的点
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

	nPointID = nPointID;//-// m_pConfgWgt->GetAnalogStartAddr();

	QMap<unsigned int, RPT> mapPoint = m_pPointInfo->GetAnalogMap();

	auto iter=m_mapIdItemObj.find(nPointID);
	if (iter!=m_mapIdItemObj.end())
	{
		//更新
		int iUpdateRow = m_mapIdItemObj[nPointID]->row();
		m_pTableModel->item(iUpdateRow, 8)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
		m_pTableModel->item(iUpdateRow, 5)->setText(QString::number(nValue, 'f', 3));
	}
	else
	{
		//插入
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
        
		//插入数据
        fp64 fZoom = m_pPointInfo->GetAnalogMap()[nPointID].KRate;
       
		//序号
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem((mapPoint[nPointID].OccNO)));
		//站点ID
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem((mapPoint[nPointID].InnerAddr)));
		//地址
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(mapPoint[nPointID].Address));
		//点值
		m_pTableModel->setItem(iRowNum, 3, new QStandardItem(mapPoint[nPointID].Name));
		//质量
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(mapPoint[nPointID].Desccription));
		//遥测类型
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(QString::number(nValue, 'f', 6)));
		//当前时间
		m_pTableModel->setItem(iRowNum, 6, new QStandardItem(QString::number(fZoom)));
        //缩放比例
        m_pTableModel->setItem(iRowNum, 7, new QStandardItem(mapPoint[nPointID].Unit));
		
		m_pTableModel->setItem(iRowNum, 8, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
		//记录数据
		m_mapIdItemObj.insert(std::make_pair (nPointID, m_pTableModel->item(iRowNum, 0)));
	}

}

void CRealTimeAnalog::Slot_AnalyseNormalAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty)
{
    //归一化遥测
	AnalyseRealAnalogData(iDeviceID, nPointID, nValue,iQuilty, tr("Normal Analog"),NORMAANALOG);
}

void CRealTimeAnalog::Slot_AnalyseShortFloatAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty)
{
	AnalyseRealAnalogData(iDeviceID, nPointID, nValue, iQuilty, tr("Float Analog"),FOLATANALOG);
}


CRealTimeAnalog::~CRealTimeAnalog()
{
    m_pZoneWgt->deleteLater();
}

void CRealTimeAnalog::Slot_AnalyseScaledAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty)
{
    AnalyseRealAnalogData(iDeviceID, nPointID, nValue, iQuilty, tr("Scaled Analog"), SCALEDANALOG);
}

void CRealTimeAnalog::Slot_ContextMenuRequest(const QPoint &pPoint)
{
    Q_UNUSED(pPoint);
    QMenu *pMenu = new QMenu(m_pTableView);

    QAction *pZoneAct = new QAction(tr("DeadZone"), m_pTableView);

    pMenu->addAction(pZoneAct);

    connect(pZoneAct, SIGNAL(triggered()), this, SLOT(Slot_DeadZoneAct()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();

}

void CRealTimeAnalog::Slot_DeadZoneAct()
{
    m_pZoneWgt->show();
}

void CRealTimeAnalog::Slot_ZoomArea()
{
    ZOOM_BASE zoomData;
    zoomData.fValue = m_pZoneWgt->GetZoneData();
    zoomData.nBaseAddr = m_pConfgWgt->GetDeviceAddr();
    zoomData.nReason = 6;
    zoomData.qpm = m_pZoneWgt->GetQpmData();

    QItemSelectionModel *selectionModel = m_pTableView->selectionModel(); 
    QModelIndexList indexes = selectionModel->selectedRows();

    zoomData.nNumber = indexes.count();

    zoomData.nAddr = 0;
    if (indexes.count() > 0)
    {
        zoomData.nAddr = m_pTableModel->item(indexes[0].row(),indexes[0].column())->text().toInt() + ZONEBEGINNUM;
        m_pCommunicate->SetZoomArea(zoomData);

    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("please choose Rows!"));
    }
    

    
}

void CRealTimeAnalog::Slot_ZoneFeedback(int nAddr, float fValue, int qpm, int nCot)
{
    //暂时不用

    Q_UNUSED(nAddr);
    Q_UNUSED(qpm);
    Q_UNUSED(fValue);

    if (nCot == 7 || nCot == 3)
    {
        //激活确认
        QMessageBox::information(0, tr("Warning"), tr("Operator Success!"));
    }
    else
    {
        //失败
        QMessageBox::warning(0, tr("Success"), tr("Operator failed"));
    }
}


void CRealTimeAnalog::InitData()
{
	m_iCurrentPointsNum =m_pFile->GetYCNum();

	std::vector<RPT_DEF> vecs;

	bool bRet = m_pFile->GetYcArryInfo(vecs);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return;
	}

	for each (RPT_DEF var in vecs)
	{
		//插入
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
		//序号
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem((QString::number(var->OccNO))));
		//站点ID
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem((var->InnerAddr)));
		//地址
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(var->Address));
		//点值
		m_pTableModel->setItem(iRowNum, 3, new QStandardItem(var->Name));
		//质量
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(var->Desccription));
		//遥测类型
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(""));
		//当前时间
		m_pTableModel->setItem(iRowNum, 6, new QStandardItem(QString::number(var->KRate)));
		//缩放比例
		m_pTableModel->setItem(iRowNum, 7, new QStandardItem(var->Unit));

		m_pTableModel->setItem(iRowNum, 8, new QStandardItem(""));
		//记录数据
		bool bRet = false;
		m_mapIdItemObj.insert(std::make_pair(var->Address.toInt(&bRet,16), m_pTableModel->item(iRowNum, 0)));
	}
}

