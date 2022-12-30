#include "mainwindow.h"
#include "maincli.h"
#include "system.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    std::srand(unsigned(std::time(nullptr)));

    bool noGui = false;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--no-gui") == 0) {
            noGui = true;
            break;
        }
    }

    if (noGui) {
        QCoreApplication a(argc, argv);
        System s;
        MainCli c(&s);
        c.run();
        return a.exec();
    } else {
        QApplication a(argc, argv);
        a.setStyle("Fusion");
        System s;
        MainWindow w(&s);
        QObject::connect(&a, &QApplication::lastWindowClosed, &s, &System::stop);
        w.show();
        return a.exec();
    }
}
