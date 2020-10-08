#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QProcess>
#include <QVector>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
 /*
    void connected();
    void disconnected();
    void bytesWriten(quint64 bytes);
    void readyRead();
*/
    void processReturned();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *pSocket;
    QVector<QProcess*> processes;
    QString executablePath;
    int nbProcessesfinished = 0;
};

#endif // WIDGET_H
