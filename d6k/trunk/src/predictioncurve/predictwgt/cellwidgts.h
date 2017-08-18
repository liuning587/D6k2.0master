#pragma once

#include <QWidget>
#include <QDateTime>

class QDateEdit;
class QCalendarWidget;
class QPushButton;
class QLabel;
class QRadioButton;
class QCheckBox;

class CCellTimeWgt : public QWidget
{
	Q_OBJECT
public:
	CCellTimeWgt(QWidget *parent = nullptr);
	~CCellTimeWgt();
public:
	//获取时间
	QDate GetCellData();
	//设置显示名称
	void SetLabelText(const QString &strName);
signals:
	//修改事件发出信号
	void Signal_UpdateTime(const QDate &date);

private:
	QDateEdit *m_pDateEdit;
	QCalendarWidget *m_pCalendarWgt;
	QPushButton *m_pPushWgt;
	QLabel *m_pLableWgt;
};

class CCellRadioWgt : public QWidget
{
	Q_OBJECT
public:
	CCellRadioWgt(QWidget *parent = nullptr);
	~CCellRadioWgt();
signals:
	// 1 折线  2柱状图
	void Signal_ClickItem(int nFlag);
public slots:
    //曲线槽
    void Slot_ClickCurveItem();
	//柱状槽
	void Slot_ClickAreaItem();
private:
	QRadioButton *m_pRaidoCurve;
	QRadioButton *m_pRadioRechantle;

};

class CCellCheckWgt : public QWidget
{
	Q_OBJECT
public:
	CCellCheckWgt(QWidget *parent = nullptr);
	~CCellCheckWgt();
private:
	//目前预测
	QCheckBox *m_pDayPre;
	//草短期预测
	QCheckBox *m_pShortPre;
	//今日实测
	QCheckBox *m_pCurrentData;

};