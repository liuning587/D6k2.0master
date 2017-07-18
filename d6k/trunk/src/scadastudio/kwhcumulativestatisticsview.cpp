#include "kwhcumulativestatisticsview.h"
#include "kwhcumulativestatisticstable.h"

CKwhCumulativeStatisticsView::CKwhCumulativeStatisticsView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CKwhCumulativeStatisticsTable(this);
	setCentralWidget(m_pTable);
}

CKwhCumulativeStatisticsView::~CKwhCumulativeStatisticsView()
{

}

void CKwhCumulativeStatisticsView::Refresh()
{
	m_pTable->Refresh();
}

void CKwhCumulativeStatisticsView::Delete()
{
	if (m_pTable)
	{
		m_pTable->setModel(nullptr);
	}
}
