#ifndef ENCRYCONTROLWGT_H
#define ENCRYCONTROLWGT_H

#include <QWidget>

namespace Ui {
class CEncryControlWgt;
}

class CApduRecver;
class CApduSender;
class QTimer;
class CEncryControlWgt : public QWidget
{
    Q_OBJECT

public:
    explicit CEncryControlWgt(QWidget *parent = 0);
    ~CEncryControlWgt();
    //写日志
    void WriteLog(const QString &strLog);

private slots:
    void on_pushButton_clicked();
	//更新日志
	void Slot_UpDateLog();

private:
    Ui::CEncryControlWgt *ui;

	//服务度接收主站数据
	CApduRecver *m_pRecverServer;
	//客户端发送数据到装置
	CApduSender *m_pSenderClient;
	//定时器  定时更新日志
	QTimer *m_pTimerLog;

};

#endif // ENCRYCONTROLWGT_H
