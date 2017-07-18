#ifndef AICUMULATIVESTATISTICSTABLE_H
#define AICUMULATIVESTATISTICSTABLE_H

#include <QTableView>

class CSortFilterModel;
class CAICumulativeStatisticsModel;

class CAICumulativeStatisticsTable : public QTableView
{
	Q_OBJECT

public:
	CAICumulativeStatisticsTable(QWidget *parent);
	~CAICumulativeStatisticsTable();

	void Refresh();

private:
	CSortFilterModel *m_pSortModel;
	CAICumulativeStatisticsModel *m_pModel;
};

#endif // AICUMULATIVESTATISTICSTABLE_H
