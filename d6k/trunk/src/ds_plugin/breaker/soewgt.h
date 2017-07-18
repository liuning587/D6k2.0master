#pragma once

#include <QWidget>
#include "ui_soewgt.h"
#include <memory>
#include "breaker_data.h"

class CNetManager;
class SOE_ACTION_INFO;
class CSoeWgt : public QWidget
{
	Q_OBJECT

public:
	CSoeWgt(CNetManager *pNetManager, QWidget *parent = Q_NULLPTR);
	~CSoeWgt();
public:
	//动作soe
	void AnaylseActionData(SOE_ACTION_INFO *pAction);
	//变位soe
	void AnayseIoData(SOE_IO_INFO *pIO);
	//异常SOE
	void AnalyseAbnormalData(SOE_ID_INFO *pSoeInfo);
	//运行SOE
	void AnalyseRuningData(SOE_ID_INFO *pSoeInfo);
	//录波SOE
	void AnalyseRecordData(SOE_ID_INFO *pSoeInfo);

public slots:
    //收取详细信息
    void Slot_RecvNewRealTimeData(DEG_SOE_DETAIL &tSoeDetail);

    // 获取soe信息
    void Slot_GetSoeEventInfo();
	//获取soe条目信息
	void Slot_GetSoeEventNum();
	//更新事件
	void Slot_SoeUpdate(int nType);
	//清除
	void Slot_ClearTable();
	//右击菜单
	void Slot_ContextMenuRequest(const QPoint &point);
private:
	Ui::CSoeWgt ui;
	//网络
	CNetManager *m_pNetManager;
};
