#include "cellwidgts.h"
#include <QDateEdit>
#include <QDate>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>


CCellTimeWgt::CCellTimeWgt(QWidget *parent):
	QWidget(parent)
{
	m_pCalendarWgt = nullptr;
	m_pDateEdit = new QDateEdit(this);
	m_pDateEdit->setDisplayFormat(QStringLiteral("yyyy年mm月dd日"));
	//m_pPushWgt = new QPushButton(this);
	//m_pPushWgt->setFixedWidth(25);
	//m_pPushWgt->setText("...");

	m_pLableWgt = new QLabel(this);
	m_pLableWgt->setText(QStringLiteral("时间:"));

	QHBoxLayout *m_layout = new QHBoxLayout(this);
	m_layout->addWidget(m_pLableWgt);
	m_layout->addWidget(m_pDateEdit);
	//m_layout->addWidget(m_pPushWgt);
	m_layout->addStretch();
	m_layout->setContentsMargins(0, 5, 0, 5);
	connect(m_pDateEdit,SIGNAL(dateChanged(const QDate &)),this,SIGNAL(Signal_UpdateTime(const QDate &)));
}


CCellTimeWgt::~CCellTimeWgt()
{
}

QDate CCellTimeWgt::GetCellData()
{
	return m_pDateEdit->date();
}

void CCellTimeWgt::SetLabelText(const QString &strName)
{
	m_pLableWgt->setText(strName);
}

CCellRadioWgt::CCellRadioWgt(QWidget *parent /*= nullptr*/):
	QWidget(parent)
{
	m_pRaidoCurve = new QRadioButton;
	m_pRaidoCurve->setText(QStringLiteral("折线图"));
	m_pRaidoCurve->setChecked(true);

	m_pRadioRechantle = new QRadioButton;
	m_pRadioRechantle->setText(QStringLiteral("柱状图"));

	QHBoxLayout *pTotalLayout = new QHBoxLayout(this);
	pTotalLayout->addWidget(m_pRaidoCurve);
	pTotalLayout->addStretch();
	pTotalLayout->addWidget(m_pRadioRechantle);
	pTotalLayout->addStretch();
	pTotalLayout->setContentsMargins(0, 5, 0, 5);
	connect(m_pRaidoCurve,SIGNAL(clicked()),this,SIGNAL(Signal_ClickItem(1)));
	connect(m_pRadioRechantle, SIGNAL(clicked()), this, SIGNAL(Signal_ClickItem(2)));
}
CCellRadioWgt::~CCellRadioWgt()
{

}



CCellCheckWgt::CCellCheckWgt(QWidget *parent /*= nullptr*/) :
	QWidget(parent)

{
	m_pDayPre = new QCheckBox;
	m_pDayPre->setText(QStringLiteral("目前"));
	m_pShortPre = new QCheckBox;
	m_pShortPre->setText(QStringLiteral("超短期"));
	m_pCurrentData = new QCheckBox;
	m_pCurrentData->setText(QStringLiteral("今日实测"));
	QHBoxLayout *pTopLayout = new QHBoxLayout(this);
	pTopLayout->addWidget(m_pDayPre);
	pTopLayout->addStretch();
	pTopLayout->addWidget(m_pShortPre);
	pTopLayout->addStretch();
	pTopLayout->addWidget(m_pCurrentData);
	pTopLayout->addStretch();
	pTopLayout->setContentsMargins(0, 5, 0, 5);


}



CCellCheckWgt::~CCellCheckWgt()
{

}
