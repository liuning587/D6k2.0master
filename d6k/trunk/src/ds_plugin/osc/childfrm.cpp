

#include "childfrm.h"

#include "left_view.h"
#include "right_view.h"

#include "wave_info.h"
#include "channelview_info.h"
 
CChildFrm::CChildFrm(std::shared_ptr<CComtrade> pWave,QWidget *parent) :m_pWave(pWave), QMainWindow(parent)
{
	m_pChannelViewInfo = std::make_shared<CChannelViewInfo>(pWave);

	m_pMainSplitter = new QSplitter(Qt::Horizontal, this);
// 	m_pLeftSplitter = new QSplitter(Qt::Vertical, m_pMainSplitter);
// 	m_pRightSplitter = new QSplitter(Qt::Horizontal);

	m_pLeftView = std::make_shared<CLeftView>(pWave,this);
	m_pRightView = std::make_shared<CRightView>(pWave,this);

// 	m_pRightSplitter->addWidget(m_pLeftView.get());
// 	m_pRightSplitter->addWidget(m_pRightView.get());

	m_pMainSplitter->addWidget(m_pLeftView.get());
	m_pMainSplitter->addWidget(m_pRightView.get());

	this->setCentralWidget(m_pMainSplitter);
	QString szName = pWave->GetComtradeName();
	if (szName.length())
	{
		this->setWindowTitle(szName);
	}
	
	m_pRightView->Initial(m_pChannelViewInfo);
	m_pLeftView->Initial(m_pChannelViewInfo);

	connect(m_pRightView->GetRenderArea(), SIGNAL(ShowFirstIndicatePointNumberSignal(int)),
		m_pLeftView->GetRenderArea(), SLOT(ShowFirstIndicatePointNumberSlot(int)));
}

CChildFrm::~CChildFrm()
{
	Destroy();
}

void CChildFrm::Destroy()
{
// 	if (m_pRightSplitter)
// 	{
// 		delete m_pRightSplitter;
// 		m_pRightSplitter = nullptr;
// 	}
// 
// 	if (m_pLeftSplitter)
// 	{
// 		delete m_pLeftSplitter;
// 		m_pLeftSplitter = nullptr;
// 	}

// 	if (m_pMainSplitter)
// 	{
// 		delete m_pMainSplitter;
// 		m_pMainSplitter = nullptr;
// 	}
}

QSize CChildFrm::minimumSizeHint() const
{
	return QSize(600, 600);
}

QSize CChildFrm::sizeHint() const
{
	return QSize(800, 600);
}
