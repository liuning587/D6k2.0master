#pragma once

#include <QDialog>
#include "ui_logic_manager.h"


#define LOGIC_MANAGER_FILE "./ini/breaker/user.cfg"

class CLogicManager : public QDialog
{
	Q_OBJECT

public:
	CLogicManager(QWidget *parent = Q_NULLPTR);
	//初始化数据
	void InitData();
	~CLogicManager();
public slots:
    void Slot_Conform();
private:
	Ui::CLogicManager ui;
	//原账户信息
	QMap<QString,QString> m_mapUserPassWd;
};
