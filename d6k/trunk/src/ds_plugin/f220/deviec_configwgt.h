#pragma once

#include <QWidget>
#include "ui_deviec_configwgt.h"

#define IEC_101_FILE  "ini/f220/iecconfig/iec101s.cfg"
#define IEC_104_FILE  "ini/f220/iecconfig/iec101s.cfg"


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

public slots:
	void Slot_DoubleClickTableItem(QTableWidgetItem *item);
	//
	void Slot_ContextMenuRequest(const QPoint &point);

private:
	Ui::CDevIecConfigWgt ui;
	CCommThread *m_pCommunicate;
	IMainModule *m_pCore;
	QString m_strDeviceName;
	//数据类型映射
	QMap<int, QString> m_mapTypeCode;

};
