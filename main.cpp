#include "mainwindow.h"
#include "maincli.h"
#include "system.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    System s;

    bool noGui = false;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--no-gui") == 0) {
            noGui = true;
            break;
        }
    }

    if (noGui) {
        QCoreApplication a(argc, argv);
        MainCli c(&s);
        c.run();
        return a.exec();
    } else {
        QApplication a(argc, argv);
        MainWindow w(&s);
        w.show();
        return a.exec();
    }
}