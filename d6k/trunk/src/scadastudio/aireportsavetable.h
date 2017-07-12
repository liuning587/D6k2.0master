#ifndef AIREPORTSAVETABLE_H
#define AIREPORTSAVETABLE_H

#include <QTableView>

class CSortFilterModel;
class CAIReportSaveModel;

class CAIReportSaveTable : public QTableView
{
	Q_OBJECT

public:
	CAIReportSaveTable(QWidget *parent);
	~CAIReportSaveTable();

	void Refresh();

private:
	CSortFilterModel *m_pSortModel;
	CAIReportSaveModel *m_pModel;
};

#endif
