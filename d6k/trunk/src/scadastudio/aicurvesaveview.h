#ifndef AICURVESAVEVIEW_H
#define AICURVESAVEVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class CAICurveSaveTable;

class CAICurveSaveView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAICurveSaveView(QWidget *parent);
	~CAICurveSaveView();

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
	CAICurveSaveTable *m_pTable;
};

#endif // AICURVESAVEVIEW_H
