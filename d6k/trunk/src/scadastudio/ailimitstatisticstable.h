#ifndef AILIMITSTATISTICSTABLE_H
#define AILIMITSTATISTICSTABLE_H

#include <QTableView>

class 	CSortFilterModel;
class CAILimitStatisticsModel;

class CAILimitStatisticsTable : public QTableView
{
	Q_OBJECT

public:
	CAILimitStatisticsTable(QWidget *parent);
	~CAILimitStatisticsTable();

	void Refresh();

private:
	CSortFilterModel *m_pSortModel;
	CAILimitStatisticsModel *m_pModel;
};

#endif // AILIMITSTATISTICSTABLE_H
