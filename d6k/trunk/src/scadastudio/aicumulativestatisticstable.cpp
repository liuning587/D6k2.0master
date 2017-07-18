#include "aicumulativestatisticstable.h"
#include "sortfiltermodel.h"
#include "aicumulativestatisticsmodel.h"

CAICumulativeStatisticsTable::CAICumulativeStatisticsTable(QWidget *parent)
	: QTableView(parent)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CAICumulativeStatisticsModel(this);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);
}

CAICumulativeStatisticsTable::~CAICumulativeStatisticsTable()
{

}

void CAICumulativeStatisticsTable::Refresh()
{

}
