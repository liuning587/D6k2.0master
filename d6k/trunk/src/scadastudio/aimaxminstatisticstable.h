#ifndef AIMAXMINSTATISTICSTABLE_H
#define AIMAXMINSTATISTICSTABLE_H

#include <qtableview.h>

class CSortFilterModel;
class CAIMaxMinStatisticsModel;

class CAIMaxMinStatisticsTable : public QTableView
{
	Q_OBJECT

public:
	CAIMaxMinStatisticsTable(QWidget *parent);
	~CAIMaxMinStatisticsTable();

	void Refresh();

private:
	CSortFilterModel *m_pSortModel;
	CAIMaxMinStatisticsModel *m_pModel;
};

#endif // AIMAXMINSTATISTICSTABLE_H
