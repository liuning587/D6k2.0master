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
	//����soe
	void AnaylseActionData(SOE_ACTION_INFO *pAction);
	//��λsoe
	void AnayseIoData(SOE_IO_INFO *pIO);
	//�쳣SOE
	void AnalyseAbnormalData(SOE_ID_INFO *pSoeInfo);
	//����SOE
	void AnalyseRuningData(SOE_ID_INFO *pSoeInfo);
	//¼��SOE
	void AnalyseRecordData(SOE_ID_INFO *pSoeInfo);

public slots:
    //��ȡ��ϸ��Ϣ
    void Slot_RecvNewRealTimeData(DEG_SOE_DETAIL &tSoeDetail);

    // ��ȡsoe��Ϣ
    void Slot_GetSoeEventInfo();
	//��ȡsoe��Ŀ��Ϣ
	void Slot_GetSoeEventNum();
	//�����¼�
	void Slot_SoeUpdate(int nType);
	//���
	void Slot_ClearTable();
	//�һ��˵�
	void Slot_ContextMenuRequest(const QPoint &point);
private:
	Ui::CSoeWgt ui;
	//����
	CNetManager *m_pNetManager;
};
