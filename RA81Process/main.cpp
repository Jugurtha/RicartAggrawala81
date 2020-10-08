#include "process.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Process w(argc, argv);
    w.show();
    //for(int n = 0; n< 2;n++)
        w.run();

    return a.exec();
}
