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
	//初始化界面
	void InitWidget();
	//测试  可删
	void MakeData();
public slots:
	//选择折线图或柱状图 1折线  2柱状图
	void Slot_ChooseCureType(int nFlag);
private:
	Ui::CPredicitionWgtClass ui;
	//开始时间
	CCellTimeWgt *m_pStartTime;
	//结束时间
	CCellTimeWgt *m_pEndTime;
	//图形属性设置
	CCellRadioWgt *mj_pAttrWgt;
	//类型选测
	CCellCheckWgt *m_pTypeChooseWgt;//
	//曲线图
	CChartWgt *m_CurveWgt;
};
