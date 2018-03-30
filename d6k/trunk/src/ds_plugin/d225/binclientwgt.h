/*************************************************
Copyright:1.4
Author:wangwei
Date:2016-04-25
Description:bin文件的发送工具
**************************************************/
#ifndef BINCLIENTWGT_H
#define BINCLIENTWGT_H

#include <QWidget>
#include <QAbstractSocket>
#include <QTcpServer>

#pragma execution_character_set("utf-8");


namespace Ui {
class CBinClientWgt;
}

//srcollbar每次加载的字符数量
const int g_ConstSrocllDataLength = 5000;

class QTcpSocket;
class QPixmap;
class QSerialPort;
class QTimer;
class CBinClientWgt : public QWidget
{
    Q_OBJECT

public:
    explicit CBinClientWgt(QWidget *parent = 0);
    ~CBinClientWgt();

	void updateSerialPort();

private slots:

    //串接口发送的文件  加载
    void slot_dynamicSeralLoadData(int iIndex);

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    //串口接收数据
    void slot_RecvSerialData();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();
    //seral error
    void slot_SeralError();

    void on_pushButton_7_clicked();

private:
    Ui::CBinClientWgt *ui;

    //需要通过串口上传的文件名
    QString m_strSeralBinFile;

    //发送串接口数据
    QByteArray m_ByArraySendSeralData;

    //动态显示串口数据
    QByteArray m_byArraySeralScrollInfo;
    //串口类
    QSerialPort *m_pSeralPort;
	//
	int m_iSSeralDataLength;
};

#endif // BINCLIENTWGT_H
