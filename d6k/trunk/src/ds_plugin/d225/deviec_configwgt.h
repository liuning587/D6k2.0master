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

	//初始化界面
	bool InitWgt(const QString &strFileName);
	void AnalyseXmlData(QIODevice *pDevice);

	void InitIecInfo();
	//初始化101配置
	bool Init101IecInfo();
	//初始化104配置
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
	//更新读取的数据
	void Slot_updateFixData(QMap<int, float>);
private:
	Ui::CDevIecConfigWgt ui;
	CCommThread *m_pCommunicate;
	IMainModule *m_pCore;
	QString m_strDeviceName;
	//数据类型映射
	QMap<int, QString> m_mapTypeCode;
	//
	int m_nBeginPointNum;
	//
	QTimer *m_pFixReadTimeOut;
	//编号 对应的item
	QMap<int, QTableWidgetItem*> m_pIdToItem;
};
