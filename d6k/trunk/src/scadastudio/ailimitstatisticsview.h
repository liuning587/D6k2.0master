#ifndef AILIMITSTATISTICSVIEW_H
#define AILIMITSTATISTICSVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class CAILimitStatisticsTable;

class CAILimitStatisticsView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAILimitStatisticsView(QWidget *parent);
	~CAILimitStatisticsView();

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
	CAILimitStatisticsTable *m_pTable;
};

#endif // AILIMITSTATISTICSVIEW_H
