#ifndef AICUMULATIVESTATISTICSVIEW_H
#define AICUMULATIVESTATISTICSVIEW_H

#include <QMainWindow>

#include "scadastudio/base.h"

class CAICumulativeStatisticsTable;

class CAICumulativeStatisticsView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAICumulativeStatisticsView(QWidget *parent);
	~CAICumulativeStatisticsView();

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
	CAICumulativeStatisticsTable *m_pTable;
};

#endif // AICUMULATIVESTATISTICSVIEW_H
