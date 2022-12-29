#include "mainwindow.h"
#include "maincli.h"
#include "system.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    std::srand(unsigned(std::time(nullptr)));
    System s;

    std::vector<uint8_t> test = {
        0x21, 0x06, 0x00,
        0x34,
        0x18, 0xfd,
        0x00
    };
    std::copy(test.begin(), test.end(), s.lowerMemory.begin());

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
        a.setStyle("Fusion");
        QObject::connect(&a, &QApplication::lastWindowClosed, &s, &System::stop);
        MainWindow w(&s);
        w.show();
        return a.exec();
    }
}
