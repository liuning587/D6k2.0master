#include "ailimitstatisticstable.h"
#include "ailimitstatisticsmodel.h"
#include "sortfiltermodel.h"

CAILimitStatisticsTable::CAILimitStatisticsTable(QWidget *parent)
	: QTableView(parent), m_pSortModel(nullptr), m_pModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CAILimitStatisticsModel(this);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CAILimitStatisticsTable::~CAILimitStatisticsTable()
{

}

void CAILimitStatisticsTable::Refresh()
{
	m_pModel->RefrushModel();
}
