#pragma once

#include <QWidget>
#include "ui_deviec_configwgt.h"

#define IEC_101_FILE  "/ini/d225/iecconfig/iec101s.cfg"
#define IEC_104_FILE  "/ini/d225/iecconfig/iec104s.cfg"

enum 
{
	IEC_START_POINT_NUM = 25089,
};
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class CCommThread;
class IMainModule;
class CDevIecConfigWgt : public QWidget
{
	Q_OBJECT

public:
	CDevIecConfigWgt(CCommThread *pCommunicate, IMainModule *pCore, const QString &deviceName, QWidget *parent = 0);
	~CDevIecConfigWgt();

	//��ʼ������
	bool InitWgt(const QString &strFileName);
	void AnalyseXmlData(QIODevice *pDevice);

	void InitIecInfo();
	//��ʼ��101����
	bool Init101IecInfo();
	//��ʼ��104����
	bool Init104IecInfo();
public slots:
	void Slot_DoubleClickTableItem(QTableWidgetItem *item);
	//
	void Slot_ContextMenuRequest(const QPoint &point);
	//
	void Slot_GetAllPoints();
	//
	void Slot_SendCurrentAllPoints();
	//
	void Slot_FixReadTimeOut();
	//���¶�ȡ������
	void Slot_updateFixData(QMap<int, float>);
private:
	Ui::CDevIecConfigWgt ui;
	CCommThread *m_pCommunicate;
	IMainModule *m_pCore;
	QString m_strDeviceName;
	//��������ӳ��
	QMap<int, QString> m_mapTypeCode;
	//
	int m_nBeginPointNum;
	//
	QTimer *m_pFixReadTimeOut;
	//��� ��Ӧ��item
	QMap<int, QTableWidgetItem*> m_pIdToItem;
};
