#ifndef KWHCUMULATIVESTATISTICSVIEW_H
#define KWHCUMULATIVESTATISTICSVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class CKwhCumulativeStatisticsTable;

class CKwhCumulativeStatisticsView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CKwhCumulativeStatisticsView(QWidget *parent);
	~CKwhCumulativeStatisticsView();

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
	CKwhCumulativeStatisticsTable *m_pTable;
};

#endif // KWHCUMULATIVESTATISTICSVIEW_H
