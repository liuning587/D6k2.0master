#ifndef CSOCKETTHREAD_H
#define CSOCKETTHREAD_H
#include <QAbstractSocket>

class QTcpSocket;
class QMutex;
class CBreakRecver;

class CSocketConnect : public QObject
{
    Q_OBJECT
public:
    CSocketConnect(QObject* parent=0);
    ~CSocketConnect();
    //初始化socket
    void  ConnectSocket(const QString &strIP, int iPort);
    //关闭socket
    void CloseSocket();
    //获取数据
    QByteArray GetNetworkData();
    //写入数据
    bool WriteSocketData(const char *pSendData, int iLength);
	void SetBreakerRecver(CBreakRecver *pRecver);

signals:
    //错误信号
    void Signal_SocketError(QString strError);
    //连接成功信号
    void Signal_ConnectSuccess();
	//发送出去的数据
	void Signal_Write16Data(QByteArray btbuff);

private slots:
    void Slot_recvNewData();
    void Slot_SocketError(QAbstractSocket::SocketError error);
	//连接成功
	void Slot_ConnectSuccess();

private:
    QTcpSocket *m_pTcpScoket;
    //接收到的数据
    QByteArray m_BtayRecvData;
    //锁
    QMutex *m_pMutex;
	//处理类
	CBreakRecver *m_pRecver;
};

#endif // CSOCKETTHREAD_H
