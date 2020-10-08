#ifndef PROCESS_H
#define PROCESS_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <QTime>
#include <QMutex>
#include <QStringListModel>
#include <QCoreApplication>
#include <QEventLoop>

#define NBR_PROCESSES 10


#include <QTime>


enum{req,rep};

namespace Ui {
class Process;
}

class Process : public QWidget
{
    Q_OBJECT

public:
    explicit Process(int argc, char *argv[], QWidget *parent = 0);
    ~Process();

    void executeCriticalSection();
    void send(quint32 message, quint32 osn, quint32 id, quint32 j);
    void run();

public slots :
    void receiveMessage();

private:
    Ui::Process *ui;
    quint32 id;
    quint32 port;
    QTcpServer *pServer;

    quint32 osn = 0;
    quint32 hsn = 0;
    bool scdemandee = false;
    bool priorite = false;
    bool repdifferee[NBR_PROCESSES] = {false};
    quint32 nbrepattendues = 0;

    QMutex mutex;
    QStringListModel *model;
    void appendRowToView(QString str);

    void delay( int millisecondsToWait )
    {
        QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
        while( QTime::currentTime() < dieTime )
        {
            QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        }
    }

};

#endif // PROCESS_H
