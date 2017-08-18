#pragma once

#include <QtWidgets/QWidget>
#include "ui_predicition_wgt.h"

class CCellTimeWgt;
class CCellRadioWgt;
class CCellCheckWgt;
class CChartWgt;
class CPredicitionWgt : public QWidget
{
	Q_OBJECT

public:
	CPredicitionWgt(QWidget *parent = Q_NULLPTR);
	//��ʼ������
	void InitWidget();
	//����  ��ɾ
	void MakeData();
public slots:
	//ѡ������ͼ����״ͼ 1����  2��״ͼ
	void Slot_ChooseCureType(int nFlag);
private:
	Ui::CPredicitionWgtClass ui;
	//��ʼʱ��
	CCellTimeWgt *m_pStartTime;
	//����ʱ��
	CCellTimeWgt *m_pEndTime;
	//ͼ����������
	CCellRadioWgt *mj_pAttrWgt;
	//����ѡ��
	CCellCheckWgt *m_pTypeChooseWgt;//
	//����ͼ
	CChartWgt *m_CurveWgt;
};
