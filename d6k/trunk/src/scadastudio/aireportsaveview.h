#ifndef AIREPORTSAVEVIEW_H
#define AIREPORTSAVEVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"

class CAIReportSaveTable;

class CAIReportSaveView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAIReportSaveView(QWidget *parent);
	~CAIReportSaveView();

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
	CAIReportSaveTable *m_pTable;
};

#endif // AIREPORTSAVEVIEW_H
