#ifndef LOADFILETHREAD_H
#define LOADFILETHREAD_H

#include <QObject>


#define RECORDFILEPATH "/ini/d225/lbfile/"
#define HISTORY_DATA "/ini/d225/history/"
#define REMOTE_TABLE "/ini/d225/remotetable/"
#define CMD_HISTORY "./ini/d225/cmdhistory/"



#define RECORD_FIlE "tffs0"

class CLoadFileThread : public QObject
{
    Q_OBJECT

public:
    CLoadFileThread(QObject *parent=0);
    ~CLoadFileThread();
    void CreateNewFile(QString &strFileName, QByteArray &byData);
signals:
    void Signal_CreateNewFile(QString &strFileName,QByteArray &byData);

public slots:

void Slot_CreateNewFile(QString strFileName, QByteArray byData);

private:
    
};

#endif // LOADFILETHREAD_H
