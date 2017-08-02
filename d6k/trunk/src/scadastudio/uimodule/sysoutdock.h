#ifndef SYSOUTDOCK_H
#define SYSOUTDOCK_H

#include "log/ui_log.h"

#include <QDockWidget>

class CSystemOutputView;
class CSystemOutputModel;
class QTcpServer;
class QTabWidget;
struct MSG_LOG;

class CSysOutDock : public QDockWidget
{
	Q_OBJECT

public:
	CSysOutDock(CUiLog & uilog,QWidget *parent);
	~CSysOutDock();


public slots:
	void ModelRowsInserted(const QModelIndex & parent, int start, int end);
	//ÓÒ»÷²Ëµ¥
	void Slot_ContextMenuRequest(const QPoint &cPoint);
	//copy
	void Slot_CpyAction();
	//clear
	void Slot_ClearItemAction();
	//run
	void Slot_StartRunAction();
	//stop
	void Slot_StopAction();
	//Export
	void Slot_ExportAction();
protected:
	void closeEvent(QCloseEvent *event)
	{
		this->setHidden(true);
		
		QDockWidget::closeEvent(event);
	}
	void timerEvent(QTimerEvent *);
	void SetShow(bool bFlag)
	{
		this->SetShow(bFlag);
	}

	void resizeEvent(QResizeEvent *event);
private:
	void CreateCommandView();

	void CreateLogCommandView();

private:
	QTabWidget	*m_pMessageTab;

	CSystemOutputView *m_pCommandView;
	CSystemOutputModel *m_pCommandModel;

	CSystemOutputView *m_pLogView;
	CSystemOutputModel *m_pLogModel;
	int m_nLogTimerId;
	//ÊÇ·ñ¹ö¶¯
	bool m_bSrcollFlag;

	CUiLog & m_UiLog;
};

#endif // SYSOUTDOCK_H
