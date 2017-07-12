#include "ailimitstatisticsview.h"
#include "ailimitstatisticstable.h"

CAILimitStatisticsView::CAILimitStatisticsView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CAILimitStatisticsTable(this);
	setCentralWidget(m_pTable);
}

CAILimitStatisticsView::~CAILimitStatisticsView()
{

}

void CAILimitStatisticsView::Refresh()
{

}

void CAILimitStatisticsView::Delete()
{
	if (m_pTable)
	{
		m_pTable->setModel(nullptr);
	}
}
