#ifndef PREDICT_STATION_TABLE_WIDGET
#define PREDICT_STATION_TABLE_WIDGET

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "predict_datainfo.h"

#include <QWidget>
#include <QTableWidget>

class CStationStaticTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CStationStaticTableWidget(CPlantInfo* plantItem);
	~CStationStaticTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CPlantInfo* GetPlantItem() const;
	//获取绑点值
	QString GetBindValue();
public slots:
	//设置绑定值 
	void Slot_SetBindValue();
	void on_customContextMenuRequested(const QPoint &pos);
private:
	CPlantInfo*    m_pPlantItem;
};

class CPredictStationStaticWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictStationStaticWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictStationStaticWidget();

public:

	void AddToTableCtrl(CPlantInfo* pPlantItem, QString strName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};

class CStationDynaticTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CStationDynaticTableWidget(CPlantInfo* plantItem);
	~CStationDynaticTableWidget();

public slots:
	void itemChanged(QTableWidgetItem *item);

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CPlantInfo* GetPlantItem() const;
	//获取绑点值
	QString GetBindValue();
	public slots:
	//设置绑定值 
	void Slot_SetBindValue();

private:
	CPlantInfo*    m_pPlantItem;
};

class CPredictStationDynaticWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictStationDynaticWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictStationDynaticWidget();

public slots:
	void itemChanged(QTableWidgetItem *item);

public:

	void AddToTableCtrl(CPlantInfo* pPlantItem, QString strName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};

class CStationTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CStationTableWidget(CPlantInfo* plantItem);
	~CStationTableWidget();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CPlantInfo* GetPlantItem() const;
	//获取绑点值
	QString GetBindValue();
public slots:
    //设置绑定值 
    void Slot_SetBindValue();

private:
	CPlantInfo*    m_pPlantItem;
};


class CPredictStationWidget : public QWidget
{
	Q_OBJECT

public:

	CPredictStationWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictStationWidget();

public:

	void AddToTableCtrl(CPlantInfo* pPlantItem, QString strName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};





class CBasicStationTableWidget : public QTableWidget, public IBase
{
	Q_OBJECT

public:
	CBasicStationTableWidget(CStationData* pItem);
	~CBasicStationTableWidget();

	virtual void setData(int role, const QVariant &value);


public slots:
	void itemChanged(QTableWidgetItem *item);


public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CStationData* GetBasicStationItem() const;

private:
	CStationData*    m_pStationItem;
};



//基本场站信息station
class CBasicStationWidget : public QWidget
{
	Q_OBJECT

public:

	CBasicStationWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CBasicStationWidget();

public:

	void AddToTableCtrl(CStationData* pStationItem, QString strName);

	void ReleaseData();

	void SetTabViewHead();

private:

	CTabWidget *m_pTableWidget;

	IMainModuleInterface *m_pCore;

};

#endif  //PREDICT_STATION_TABLE_WIDGET
