#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    executablePath("./exec/process.exe")
{
    ui->setupUi(this);
/*
    //Create processes
    for(int i=0;i<10;i++){
        processes.push_back(new QProcess(this));
        connect(processes[i], SIGNAL(finished(int)), this, SLOT(processReturned()));
        QStringList list;
        list << QString::number(i) << QString::number(2000+i);
        processes[i]->start(executablePath, list);
        processes[i]->closeWriteChannel();


        //if (!processes[0]->waitForFinished())
        //   exit(i);
        //auto result = processes[0]->readAllStandardOutput();
        //qDebug() << result;
    }
*/

}

Widget::~Widget()
{
    delete ui;
}

void Widget::processReturned()
{
    nbProcessesfinished++;
    qDebug() << nbProcessesfinished << "th processes finished!!";

    if(nbProcessesfinished==10)
    {
        for(int i=0;i<10;i++)
        {
            auto result = processes[i]->readAllStandardOutput();
            qDebug() << result;

        }
        processes.clear();
    }
}




void Widget::on_pushButton_clicked()
{
    //Create processes
    for(int i=0;i<10;i++){
        processes.push_back(new QProcess(this));
        connect(processes[i], SIGNAL(finished(int)), this, SLOT(processReturned()));
        QStringList list;
        list << QString::number(i) << QString::number(2000+i);
        processes[i]->start(executablePath, list);
        processes[i]->closeWriteChannel();


        //if (!processes[0]->waitForFinished())
        //   exit(i);
        //auto result = processes[0]->readAllStandardOutput();
        //qDebug() << result;
    }
}
