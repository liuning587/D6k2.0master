#include "aimaxminstatisticsview.h"
#include "aimaxminstatisticstable.h"

CAIMaxMinStatisticsView::CAIMaxMinStatisticsView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CAIMaxMinStatisticsTable(this);
	setCentralWidget(m_pTable);
}

CAIMaxMinStatisticsView::~CAIMaxMinStatisticsView()
{

}

void CAIMaxMinStatisticsView::Refresh()
{
	m_pTable->Refresh();
}

void CAIMaxMinStatisticsView::Delete()
{
	if (m_pTable)
	{
		m_pTable->setModel(nullptr);
	}
}
