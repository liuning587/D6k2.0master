#include "aireportsavetable.h"
#include "aireportsavemodel.h"
#include "sortfiltermodel.h"

CAIReportSaveTable::CAIReportSaveTable(QWidget *parent)
	: QTableView(parent)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CAIReportSaveModel(this);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CAIReportSaveTable::~CAIReportSaveTable()
{

}

void CAIReportSaveTable::Refresh()
{
	m_pModel->RefrushModel();
}
