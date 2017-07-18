#include "dicumulativestatisticstable.h"
#include "dicumulativestatisticsmodel.h"
#include "sortfiltermodel.h"


CDICumulativeStatisticsTable::CDICumulativeStatisticsTable(QWidget *parent)
	: QTableView(parent)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CDICumulativeStatisticsModel(this);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CDICumulativeStatisticsTable::~CDICumulativeStatisticsTable()
{

}

void CDICumulativeStatisticsTable::Refresh()
{
	m_pModel->RefrushModel();
}
