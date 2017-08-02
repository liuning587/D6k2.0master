#pragma once

#include <QDialog>
#include "ui_logic_manager.h"


#define LOGIC_MANAGER_FILE "./ini/breaker/user.cfg"

class CLogicManager : public QDialog
{
	Q_OBJECT

public:
	CLogicManager(QWidget *parent = Q_NULLPTR);
	//��ʼ������
	void InitData();
	~CLogicManager();
public slots:
    void Slot_Conform();
private:
	Ui::CLogicManager ui;
	//ԭ�˻���Ϣ
	QMap<QString,QString> m_mapUserPassWd;
};
