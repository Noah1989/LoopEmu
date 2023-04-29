#include "mainwindow.h"
#include "system.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    std::srand(unsigned(std::time(nullptr)));

    QApplication a(argc, argv);
    a.setStyle("Fusion");
    System s;
    MainWindow w(&s);
    QObject::connect(&a, &QApplication::lastWindowClosed, &s, &System::stop);
    w.show();
    return a.exec();
}
