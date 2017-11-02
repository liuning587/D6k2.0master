#ifndef FAULTEVENTWGT_H
#define FAULTEVENTWGT_H

#include <QWidget>
#include "ui_faulteventwgt.h"
#include "asdu_info.h"

class CCommThread;
class CPointInfo;
class CInfoConfigWgt;

class CFaultEventWgt : public QWidget
{
    Q_OBJECT

public:
    CFaultEventWgt(CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt,QWidget *parent = 0);
    ~CFaultEventWgt();
public slots:
    void Slot_MalFuction(ASDUGZ gzData);

	void Slot_ContextMenuRequest(const QPoint &point);
	//
	void Slot_clearAction();
private:
    Ui::CFaultEventWgt ui;
    //
    CCommThread *m_pCommunicate;
	//
	//点表数据
	CPointInfo *m_pPointInfo;
	//
	CInfoConfigWgt *m_pConfgWgt;

};

#endif // FAULTEVENTWGT_H
