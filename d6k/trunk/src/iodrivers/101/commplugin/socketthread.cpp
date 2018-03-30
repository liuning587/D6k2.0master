#include "ftu_module.h"
#include "socketthread.h"
#include "101recver.h"
#include "fcifile.h"
#include <QTcpSocket>
#include <QMutex>
#include <QHostAddress>
#include <QNetworkProxy>
#include <QSerialPortInfo>


#define MAX_MSG_SOCKETBUFF    1024*1024    //SOCKET数据缓存大小

/*********************************************************************************************************
** \brief CSocketThread(QObject *parent)
** \details 构造函数
** \param argl  父类指针
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
**********************************************************************************************************/
CSocketThread::CSocketThread(QObject *parent):
    QObject(parent),
    m_pMutex(new QMutex)
{
	m_pApduRecver = NULL;

	m_pSerial = std::make_shared<QSerialPort>();
	
	m_pSerial->close();

	qRegisterMetaType<QSerialPort::SerialPortError>();

	connect(m_pSerial.get(), SIGNAL(errorOccurred(QSerialPort::SerialPortError )),this,SLOT(Slot_SocketError(QSerialPort::SerialPortError)), Qt::QueuedConnection);
	
	connect (m_pSerial.get(),SIGNAL(readyRead()),this,SLOT(Slot_recvNewData()), Qt::QueuedConnection);
}

CSocketThread::~CSocketThread()
{ 
	m_pSerial->close();

	m_pSerial->deleteLater();

    delete m_pMutex;   
}


/*********************************************************************************************************
** \brief ConnectSocket(const QString &strIP, int iPort)
** \details socket连接
** \param argl  IP地址
** \param arg2  端口号
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CSocketThread::ConnectSocket(const QString &strIP, int iPort)
{
	Q_UNUSED(strIP);

	Q_UNUSED(iPort);

	UART_DEF pUart;

	CFciFile::GetInstance().GetUartInfo(0, pUart);

	QString strComName = QString("COM%1").arg(pUart->ComID);
	
	if (m_pSerial==nullptr)
	{
		m_pSerial = std::make_shared<QSerialPort>();

		m_pSerial->close();

		connect(m_pSerial.get(), SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(Slot_SocketError(QSerialPort::SerialPortError)), Qt::QueuedConnection);

		connect(m_pSerial.get(), SIGNAL(readyRead()), this, SLOT(Slot_recvNewData()), Qt::QueuedConnection);
	}

	m_pSerial->setPortName(strComName);

	m_pSerial->setBaudRate(pUart->Band);

	switch (pUart->ParityBit)
	{
	case 0:
		m_pSerial->setParity(QSerialPort::NoParity);
		break;
	case 2:
		m_pSerial->setParity(QSerialPort::EvenParity);
		break;
	case 3:
		m_pSerial->setParity(QSerialPort::OddParity);
	default:
		break;
	}

	m_pSerial->setStopBits(QSerialPort::OneStop);
	m_pSerial->setDataBits(QSerialPort::Data8);
	m_pSerial->setFlowControl(QSerialPort::NoFlowControl);
	

	QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
	
	for each (QSerialPortInfo var in infos)
	{
		if (var.portName()== strComName)
		{
			if (m_pSerial->isOpen())
			{
				emit Signal_SocketError(tr("COM is busy!!"));
				return;
			}
			else 
			{ 
				if (m_pSerial->open(QIODevice::ReadWrite))
				{		
					m_pMutex->lock();

					m_BtayRecvData.clear();

					m_pMutex->unlock();

					emit Signal_ConnectSuccess(QString("COM OPEN SUCCESS!"));
					
					return;
				}
			}		
		}
	}

	emit Signal_SocketError(tr("Can not find this COM info"));
}

/*********************************************************************************************************
** \brief setApduRecver(CApduRecver *pRecver)
** \details recver类
** \param argl  
** \param arg2  
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/

void CSocketThread::setApduRecver(C101Recver *pRecver)
{
	Q_ASSERT(pRecver);
	if (!pRecver)
	{
		return;
	}
	m_pApduRecver = pRecver;
}


void CSocketThread::setSender(C101Sender * pSender)
{
	Q_ASSERT(pSender);
	if (!pSender)
	{
		return;
	}
	m_pSender = pSender;
}

void CSocketThread::SetModule(CFtuModule* pMudule)
{
	Q_ASSERT(pMudule);
	if (!pMudule)
	{
		return;
	}
	m_pMudule = pMudule;
}

void CSocketThread::CloseSocket()
{
	if(m_pSerial->isOpen())
	{
		m_pMutex->lock();
		m_pSerial->close();
		m_pMutex->unlock();
	}	
}

/*********************************************************************************************************
** \brief GetNetworkData()
** \details 获取网络数据
** \param argl  
** \param arg2  
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
QByteArray CSocketThread::GetNetworkData()
{
	return "";
}

/*********************************************************************************************************
** \brief WriteSocketData(const char *pSendData, int iLength)
** \details 写入socket数据
** \param argl  需要写入的数据
** \param arg2  需要写入数据的长度
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CSocketThread::WriteSocketData(const char *pSendData, int iLength)
{
    if ((pSendData == NULL) || (iLength < 0))
    {
        return false;
    }
  
    m_pMutex->lock();
    qint64 sendLength =  m_pSerial->write(pSendData,iLength);
    m_pMutex->unlock();

    if (sendLength < iLength)
    {
        return false;
    }

	emit Signal_Write16Data(QByteArray(pSendData,iLength));

    return true;
}

void CSocketThread::RecvData()
{
	CDeviceData * pDev = m_pMudule->GetDeviceInfo();
	Q_ASSERT(pDev);
	if (!pDev)
	{
		return;
	}
	if (pDev->canRecv())
	{
		while (m_pSerial->bytesAvailable())
		{
			QByteArray btArray = m_pSerial->readAll();

			if ((unsigned)btArray.length() <= 0)
			{
				return;
			}

			qDebug() << btArray;

			bool bRet = m_pApduRecver->OnReceive(btArray.data(), btArray.length());
			if (bRet)
			{
				pDev->m_nRunState = DEVICE_RUN_STATE_RECV;
			}
		}
	}
}

/*********************************************************************************************************
** \brief Slot_recvNewData()
** \details 接收网络数据槽函数
** \param argl  
** \param arg2  
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CSocketThread::Slot_recvNewData()
{
	CDeviceData * pDev = m_pMudule->GetDeviceInfo();
	Q_ASSERT(pDev);
	if (!pDev)
	{
		return;
	}

	{
		while (m_pSerial->bytesAvailable())
		{
			QByteArray btArray = m_pSerial->readAll();

			if ((unsigned)btArray.length() <= 0)
			{
				return;
			}
		
			bool bRet=m_pApduRecver->OnReceive(btArray.data(), btArray.length());
			if (bRet)
			{
				pDev->m_nRunState = DEVICE_RUN_STATE_RECV;
			}
		}
	}	
}

/*********************************************************************************************************
** \brief Slot_SocketError(QAbstractSocket::SocketError error)
** \details socket错误槽函数
** \param argl   socket错误码
** \param arg2
** \return void
** \author xingzhibing
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CSocketThread::Slot_SocketError(QSerialPort::SerialPortError error)
{
	switch (error)
	{
	case QSerialPort::NoError:
		break;
	case QSerialPort::DeviceNotFoundError:
		emit Signal_SocketError(tr("DeviceNotFoundError"));
		break;
	case QSerialPort::PermissionError:
		emit Signal_SocketError(tr("PermissionError"));
		break;
	case QSerialPort::OpenError:
		emit Signal_SocketError(tr("OpenError"));
		break;
	case QSerialPort::ParityError:
		emit Signal_SocketError(tr("ParityError"));
		break;
	case QSerialPort::FramingError:
		emit Signal_SocketError(tr("FramingError"));
		break;
	case QSerialPort::BreakConditionError:
		emit Signal_SocketError(tr("BreakConditionError"));
		break;
	case QSerialPort::WriteError:
		emit Signal_SocketError(tr("WriteError"));
		break;
	case QSerialPort::ReadError:
		emit Signal_SocketError(tr("ReadError"));
		break;
	case QSerialPort::ResourceError:
		emit Signal_SocketError(tr("ResourceError"));
		break;
	case QSerialPort::UnsupportedOperationError:
		emit Signal_SocketError(tr("UnsupportedOperationError"));
		break;
	case QSerialPort::UnknownError:
		emit Signal_SocketError(tr("UnknownError"));
		break;
	case QSerialPort::TimeoutError:
		emit Signal_SocketError(tr("TimeoutError"));
		break;
	case QSerialPort::NotOpenError:
		emit Signal_SocketError(tr("NotOpenError"));
		break;
	default:
		break;
	}
}

//连接成功
void CSocketThread::Slot_ConnectSuccess()
{

}
