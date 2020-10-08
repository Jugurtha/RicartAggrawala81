#include "process.h"
#include "ui_process.h"
#include <iostream>
#include <QStringListModel>


Process::Process(int argc, char *argv[], QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Process)
{
    ui->setupUi(this);
    model =new QStringListModel(this);
    ui->listView->setModel(model);

    qsrand(QTime::currentTime().msec());//Seeding qrand

    if(argc < 2)
    {
        id=0;port=1000;//exit(EXIT_FAILURE);
    }
    else{
        id = QString(argv[1]).toInt();
        port = QString(argv[2]).toInt();
    }


    std::cout << "Process " << id << ", port : " <<  port << std::endl;
    ui->label->setText(QString("Process %1, port %2 ").arg(id).arg(port));

    pServer = new QTcpServer(this);

    connect(pServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));

    if(!pServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server could not start !!!";
        exit(EXIT_FAILURE);
    }
    else
        qDebug() << "Server started !!!";

}

Process::~Process()
{
    delete ui;
}

void Process::executeCriticalSection()
{

    //Exclusion

    mutex.lock();
    scdemandee = true;
    osn = hsn;
    nbrepattendues = NBR_PROCESSES - 1;
    mutex.unlock();

    for(quint32 j=0;j<NBR_PROCESSES;j++)
        if(j!=id)
            send(req,osn,id,j);
    
    while(nbrepattendues!=0)
    {
qDebug() << id << " nbrepattendues = " << nbrepattendues;
appendRowToView(QString("%1 nbrepattendues = %2").arg(id).arg(nbrepattendues));
       delay(1);
    }

    //Start of CS
    int waitTime = qrand()%(2000 - 1000 +1) + 1000;
qDebug() << id << " entering cs for " << waitTime;
appendRowToView(QString("%1 entering cs for %2").arg(id).arg(waitTime));
    delay(waitTime);// Wait for 1 to 2 seconds
    //end od CS

    mutex.lock();
    scdemandee = false;

    for(quint32 j=0; j < NBR_PROCESSES; j++)
        if(repdifferee[j])
        {
            repdifferee[j] = false;
            send(rep,osn,id,j);
        }
    mutex.unlock();

}

void Process::send(quint32 message, quint32 osn, quint32 id, quint32 j)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost,2000+j);
    if(socket->waitForConnected())
    {
        QString str;
        if(message==req)
            str = QString("req,%1,%2").arg(osn).arg(id);
        else if(message==rep)
            str = QString("rep,%1,%2").arg(osn).arg(id);

        socket->write(str.toStdString().c_str());
        socket->flush();
        socket->waitForBytesWritten();
        socket->close();

qDebug() << id << " sent : " << str;
appendRowToView(QString("%1 sent : %2 to %3").arg(id).arg(str).arg(j));
    }
    else
        qDebug() << id << ": message not sent to process " << j;
}

void Process::run()
{
    int waitTime = qrand()%(10000 - 1000 +1) + 1000;
qDebug() << id << " waiting " << waitTime << " before cs.";
appendRowToView(QString("%1 waiting %2 before cs.").arg(id).arg(waitTime));

    delay(waitTime);// Wait for 1 to 10 seconds must be changed to handle craches. And execute in defferent thread
    
    //execute this in different thread
    executeCriticalSection();
}

void Process::receiveMessage()
{
    QTcpSocket *pSocket = pServer->nextPendingConnection();

    pSocket->waitForReadyRead();

    QString str(pSocket->readAll());
    QStringList list = str.split(',');

    quint32 message, k, j;
    if(list[0]=="req")
       message = req;
    else if(list[0]=="rep")
        message = rep;
    k = list[1].toInt();
    j = list[2].toInt();

qDebug() << id << " received : " << list[0] << ", " << k << ", " << j;
appendRowToView(QString("%1 received : %2,%3,%4").arg(id).arg(list[0]).arg(k).arg(j));

    if(message==req)
    {
        mutex.lock();
        hsn = (hsn> k)?hsn:k + 1;
        priorite = scdemandee && ( (k>osn) || ((k==osn) && (id<j) ) );
        if(priorite)
            repdifferee[j] = true;
        else
            send(rep,osn,id,j);
        mutex.unlock();
    }
    else if(message==rep)
    {
        mutex.lock();
        nbrepattendues--;
        mutex.unlock();
    }

}

void Process::appendRowToView(QString str)
{
    model->insertRow(model->rowCount());
    QModelIndex index = model->index(model->rowCount()-1);
    model->setData(index, str);
}


