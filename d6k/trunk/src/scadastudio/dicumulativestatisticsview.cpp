#include "dicumulativestatisticsview.h"
#include "dicumulativestatisticstable.h"

CDICumulativeStatisticsView::CDICumulativeStatisticsView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CDICumulativeStatisticsTable(this);
	setCentralWidget(m_pTable);
}

CDICumulativeStatisticsView::~CDICumulativeStatisticsView()
{

}

void CDICumulativeStatisticsView::Refresh()
{
	m_pTable->Refresh();
}

void CDICumulativeStatisticsView::Delete()
{
	if (m_pTable)
	{
		m_pTable->setModel(nullptr);
	}
}
