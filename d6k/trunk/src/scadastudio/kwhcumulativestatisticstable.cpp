#include "kwhcumulativestatisticstable.h"
#include "kwhcumulativestatisticsmodel.h"
#include "sortfiltermodel.h"

CKwhCumulativeStatisticsTable::CKwhCumulativeStatisticsTable(QWidget *parent)
	: QTableView(parent)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CKwhCumulativeStatisticsModel(this);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CKwhCumulativeStatisticsTable::~CKwhCumulativeStatisticsTable()
{

}

void CKwhCumulativeStatisticsTable::Refresh()
{

}
