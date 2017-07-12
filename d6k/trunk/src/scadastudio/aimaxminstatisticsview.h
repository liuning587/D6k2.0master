#ifndef AIMAXMINSTATISTICSVIEW_H
#define AIMAXMINSTATISTICSVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class CAIMaxMinStatisticsTable;

class CAIMaxMinStatisticsView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAIMaxMinStatisticsView(QWidget *parent);
	~CAIMaxMinStatisticsView();

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
	CAIMaxMinStatisticsTable *m_pTable;
};

#endif // AIMAXMINSTATISTICSVIEW_H
