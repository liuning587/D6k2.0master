#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QTcpSocket>
#include <QMessageBox>
#include <QBuffer>
#include <QScrollBar>
#include <QPixmap>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "binclientwgt.h"
#include "ui_binclientwgt.h"


CBinClientWgt::CBinClientWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CBinClientWgt),
    m_pSeralPort(new QSerialPort)
{
    ui->setupUi(this);

    m_strSeralBinFile = "";

	m_iSSeralDataLength = 0;

    setWindowTitle("bin文件发送工具");

    //动态加载
    connect (ui->textEdit_4->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(slot_dynamicSeralLoadData(int)));
    //串口
    connect (m_pSeralPort,SIGNAL(readyRead()),this,SLOT(slot_RecvSerialData()));
    connect (m_pSeralPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(slot_SeralError()));

	updateSerialPort();

}

CBinClientWgt::~CBinClientWgt()
{
    m_pSeralPort->close();
    delete m_pSeralPort;
    delete ui;
}

void CBinClientWgt::updateSerialPort()
{
	ui->comboBox->clear();
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		qDebug() << "Name        : " << info.portName();
		qDebug() << "Description : " << info.description();
		qDebug() << "Manufacturer: " << info.manufacturer();

		QSerialPort serial;
		serial.setPort(info);
		if (serial.open(QIODevice::ReadWrite))
		{
			ui->comboBox->addItem(info.portName());
			serial.close();
		}
	}
}


void CBinClientWgt::slot_dynamicSeralLoadData(int iIndex)
{
	
    qDebug()<<iIndex;
    if(iIndex == ui->textEdit_4->verticalScrollBar()->maximum())
    {
        qDebug()<<"begin loading";
        if (m_byArraySeralScrollInfo.length()-m_iSSeralDataLength > g_ConstSrocllDataLength)
        {
            //继续加载
            ui->textEdit_4->insertPlainText(m_byArraySeralScrollInfo.mid(m_iSSeralDataLength,g_ConstSrocllDataLength).toHex());
            m_iSSeralDataLength += g_ConstSrocllDataLength;
        }
        else if (m_byArraySeralScrollInfo.length()-m_iSSeralDataLength == 0)
        {
            return;
        }
        else
        {
            ui->textEdit_4->moveCursor(QTextCursor::End);
            ui->textEdit_4->insertPlainText(m_byArraySeralScrollInfo.mid(m_iSSeralDataLength,m_byArraySeralScrollInfo.length()-m_iSSeralDataLength).toHex());
            m_iSSeralDataLength += m_byArraySeralScrollInfo.length()-m_iSSeralDataLength;
        }
    }
	
}


/******************************************************************************
Function:       on_pushButton_6_clicked
Description:    点击打开文件
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::on_pushButton_6_clicked()
{
    m_strSeralBinFile = QFileDialog::getOpenFileName(this);
    qDebug()<<m_strSeralBinFile;
    if (!m_strSeralBinFile.isEmpty())
    {
        qDebug()<<"fjdkjshfkjds";
        QFile file(m_strSeralBinFile);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug()<<"fjdkjshfefekjds";
            m_byArraySeralScrollInfo.clear();
            //操作文件数据
            while (!file.atEnd())
            {
                m_byArraySeralScrollInfo.append(file.readLine());
            }
            //记录当前已经加载的数据

            if (m_byArraySeralScrollInfo.length() < g_ConstSrocllDataLength)
            {
                 ui->textEdit_4->setText(m_byArraySeralScrollInfo.toHex());
                 m_iSSeralDataLength = m_byArraySeralScrollInfo.length();
            }
            else
            {
                m_iSSeralDataLength = g_ConstSrocllDataLength;
                ui->textEdit_4->setText(m_byArraySeralScrollInfo.mid(0,g_ConstSrocllDataLength).toHex());
            }

            file.close();

        }
    }
    else
    {
        qDebug()<<"eeege";
        ui->textEdit_4->clear();
        QMessageBox::warning(this,tr("告警"),tr("请选择文件后进行操作!"));
    }
}

/******************************************************************************
Function:       on_pushButton_8_clicked
Description:    刷新串口
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::on_pushButton_8_clicked()
{
    updateSerialPort();
}

/******************************************************************************
Function:       slot_RecvSerialData
Description:    串口接收数据
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::slot_RecvSerialData()
{
	//接收到的数据
    QByteArray  data = m_pSeralPort->readAll();
	ui->textEdit_5->setText(data);
}

/******************************************************************************
Function:       on_pushButton_9_clicked
Description:    打开串口
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::on_pushButton_9_clicked()
{
    m_pSeralPort->close();
    m_pSeralPort->setPortName(ui->comboBox->currentText());
    if (!m_pSeralPort->open(QIODevice::ReadWrite))
    {
        m_pSeralPort->clear();
        ui->label_15->setText(tr("当前状态:串口打开失败"));
        QMessageBox::warning(this,tr("告警"),tr("串口打开失败:%1").arg(m_pSeralPort->errorString()));
        return;
    }
    else
    {
        ui->label_15->setText(tr("当前状态:串口打开成功"));
    }
    m_pSeralPort->setBaudRate(ui->comboBox_2->currentText().toInt());
    m_pSeralPort->setDataBits(QSerialPort::Data8);
    m_pSeralPort->setParity(QSerialPort::NoParity);
    m_pSeralPort->setStopBits(QSerialPort::OneStop);
    m_pSeralPort->setFlowControl(QSerialPort::NoFlowControl);
}

/******************************************************************************
Function:       on_pushButton_10_clicked
Description:    关闭串口
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::on_pushButton_10_clicked()
{
    m_pSeralPort->close();
    ui->label_15->setText(tr("当前状态:串口关闭成功"));
}

/******************************************************************************
Function:       slot_SeralError
Description:    窗口错误
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::slot_SeralError()
{
    qDebug()<<m_pSeralPort->errorString();

}

/******************************************************************************
Function:       on_pushButton_7_clicked
Description:    串口发送数据
Input:

Output:         无
Return:
Others:         无
******************************************************************************/
void CBinClientWgt::on_pushButton_7_clicked()
{
    m_ByArraySendSeralData.clear();
    //发送信息
    //QByteArray dataArray;
    if (!m_strSeralBinFile.isEmpty())
    {
        QFile file(m_strSeralBinFile);
        if (file.open(QFile::ReadOnly))
        {
             m_ByArraySendSeralData.append(file.readAll());
        }
        file.close();
    }
    else
    {
        QMessageBox::warning(this,tr("告警"),tr("请先选择文件再进行发送!"));
        return;
    }

    int ret = m_pSeralPort->write(m_ByArraySendSeralData);
    if (ret == -1)
    {
        QMessageBox::warning(this,tr("告警"),tr("发送失败,错误:%1").arg(m_pSeralPort->errorString()));
    }
	else
	{
		QMessageBox::information(this, tr("成功"), tr("数据发送成功"));
		ui->label_15->setText(tr("当前状态:数据发送成功"));
	}
}
