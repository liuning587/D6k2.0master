#include "aicurvesavetable.h"
#include "sortfiltermodel.h"
#include "aicurvesavemodel.h"

CAICurveSaveTable::CAICurveSaveTable(QWidget *parent)
	: QTableView(parent)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CAICurveSaveModel(this);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CAICurveSaveTable::~CAICurveSaveTable()
{

}

void CAICurveSaveTable::Refresh()
{
	m_pModel->RefrushModel();
}
