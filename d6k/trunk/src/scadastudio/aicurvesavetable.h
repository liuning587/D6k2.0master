#ifndef AICURVESAVETABLE_H
#define AICURVESAVETABLE_H

#include <QTableView>

class CSortFilterModel;
class CAICurveSaveModel;

class CAICurveSaveTable : public QTableView
{
	Q_OBJECT

public:
	CAICurveSaveTable(QWidget *parent);
	~CAICurveSaveTable();

	void Refresh();
private:
	CSortFilterModel *m_pSortModel;
	CAICurveSaveModel *m_pModel;
};

#endif // AICURVESAVETABLE_H
