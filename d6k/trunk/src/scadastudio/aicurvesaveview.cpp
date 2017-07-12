#include "aicurvesaveview.h"
#include "aicurvesavetable.h"

CAICurveSaveView::CAICurveSaveView(QWidget *parent)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CAICurveSaveTable(this);
	setCentralWidget(m_pTable);
}

CAICurveSaveView::~CAICurveSaveView()
{

}

void CAICurveSaveView::Refresh()
{
	m_pTable->Refresh();
}

void CAICurveSaveView::Delete()
{
	if (m_pTable)
	{
		m_pTable->setModel(nullptr);
	}
}
