#ifndef KWHCUMULATIVESTATISTICSTABLE_H
#define KWHCUMULATIVESTATISTICSTABLE_H

#include <QTableView>

class CSortFilterModel;
class CKwhCumulativeStatisticsModel;

class CKwhCumulativeStatisticsTable : public QTableView
{
	Q_OBJECT

public:
	CKwhCumulativeStatisticsTable(QWidget *parent);
	~CKwhCumulativeStatisticsTable();

	void Refresh();

private:
	CSortFilterModel *m_pSortModel;
	CKwhCumulativeStatisticsModel *m_pModel;
};

#endif // KWHCUMULATIVESTATISTICSTABLE_H
