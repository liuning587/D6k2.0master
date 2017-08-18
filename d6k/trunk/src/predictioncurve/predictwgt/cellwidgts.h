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
	//��ȡʱ��
	QDate GetCellData();
	//������ʾ����
	void SetLabelText(const QString &strName);
signals:
	//�޸��¼������ź�
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
	// 1 ����  2��״ͼ
	void Signal_ClickItem(int nFlag);
public slots:
    //���߲�
    void Slot_ClickCurveItem();
	//��״��
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
	//ĿǰԤ��
	QCheckBox *m_pDayPre;
	//�ݶ���Ԥ��
	QCheckBox *m_pShortPre;
	//����ʵ��
	QCheckBox *m_pCurrentData;

};