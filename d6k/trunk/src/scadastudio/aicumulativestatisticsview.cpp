#include "aicumulativestatisticsview.h"
#include "aicumulativestatisticstable.h"

CAICumulativeStatisticsView::CAICumulativeStatisticsView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CAICumulativeStatisticsTable(this);

	setCentralWidget(m_pTable);
}

CAICumulativeStatisticsView::~CAICumulativeStatisticsView()
{

}

void CAICumulativeStatisticsView::Refresh()
{

}

void CAICumulativeStatisticsView::Delete()
{
	m_pTable->setModel(nullptr);
}
