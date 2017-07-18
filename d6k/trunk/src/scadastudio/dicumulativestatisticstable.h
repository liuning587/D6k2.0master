#ifndef DICUMULATIVESTATISTICSTABLE_H
#define DICUMULATIVESTATISTICSTABLE_H

#include <QTableView>

class CSortFilterModel;
class CDICumulativeStatisticsModel;

class CDICumulativeStatisticsTable : public QTableView
{
	Q_OBJECT

public:
	CDICumulativeStatisticsTable(QWidget *parent);
	~CDICumulativeStatisticsTable();

	void Refresh();

private:
	CSortFilterModel *m_pSortModel;
	CDICumulativeStatisticsModel *m_pModel;
};

#endif // DICUMULATIVESTATISTICSTABLE_H
