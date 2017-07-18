#ifndef DICUMULATIVESTATISTICSVIEW_H
#define DICUMULATIVESTATISTICSVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class CDICumulativeStatisticsTable;

class CDICumulativeStatisticsView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDICumulativeStatisticsView(QWidget *parent);
	~CDICumulativeStatisticsView();

	virtual void Save()
	{

	}
	virtual void Refresh();

	//module file
	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return m_bModify;
	}

	virtual void Delete();

private:
	CDICumulativeStatisticsTable *m_pTable;
};

#endif // DICUMULATIVESTATISTICSVIEW_H
