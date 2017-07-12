#include "aimaxminstatisticstable.h"
#include "aimaxminstatisticsmodel.h"
#include "sortfiltermodel.h"

CAIMaxMinStatisticsTable::CAIMaxMinStatisticsTable(QWidget *parent)
	: QTableView(parent)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CAIMaxMinStatisticsModel(this);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CAIMaxMinStatisticsTable::~CAIMaxMinStatisticsTable()
{

}

void CAIMaxMinStatisticsTable::Refresh()
{
	m_pModel->RefrushModel();
}
