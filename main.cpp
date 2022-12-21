#include "mainwindow.h"
#include "system.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    System s;
    MainWindow w(&s);
    w.show();
    return a.exec();
}
