#include "aireportsaveview.h"
#include "aireportsavetable.h"

CAIReportSaveView::CAIReportSaveView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CAIReportSaveTable(this);
	setCentralWidget(m_pTable);
}

CAIReportSaveView::~CAIReportSaveView()
{

}

void CAIReportSaveView::Refresh()
{
	m_pTable->Refresh();
}

void CAIReportSaveView::Delete()
{
	if (m_pTable)
	{
		m_pTable->setModel(nullptr);
	}
}
