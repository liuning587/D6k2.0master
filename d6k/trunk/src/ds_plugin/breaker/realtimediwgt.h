#pragma once

#include <QWidget>
#include "ui_realtimediwgt.h"

class CRealTimeDiWgt : public QWidget
{
	Q_OBJECT

public:
	CRealTimeDiWgt(QWidget *parent = Q_NULLPTR);
	~CRealTimeDiWgt();
	//初始化数据
	void InitData();
	//更新值
	void UpdateItem(int nValue);

// 	const std::map<int, QTableWidgetItem*>&GetIdItem()
// 	{
// 		return m_IdItem;
// 	}

private:
	Ui::CRealTimeDiWgt ui;
//	std::map<int, QTableWidgetItem*> m_IdItem;
};
