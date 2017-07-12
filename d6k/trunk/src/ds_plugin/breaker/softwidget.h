#pragma once

#include <QWidget>
#include <memory>
#include "ui_softwidget.h"
#include "breaker_data.h"

class CNetManager;
class CSoftWidget : public QWidget
{
	Q_OBJECT

public:
	CSoftWidget(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CSoftWidget();
	//初始化数据
	void InitData();
	QList<int> GetSelectRows();
	int ParseBinary(char const * const binaryString, int nLength);

public slots:
	void Slot_RecvNewRealTimeData(DEG_SOFT_INFO &tMeas);
	void Slot_ContextMenuRequest(const QPoint &);
	//更新
	void Slot_UpdateData();
	void Slot_SetDevData();
private:
	Ui::CSoftWidget ui;
	std::map<int, QTableWidgetItem*> m_IdItem;
	//网络
	CNetManager *m_pNetManager;
};
