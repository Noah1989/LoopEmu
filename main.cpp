#include "mainwindow.h"
#include "system.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    std::srand(unsigned(std::time(nullptr)));

    QApplication a(argc, argv);
    a.setStyle("Fusion");

    // Any arguments are Intel-hex images to load on top of the disk/ defaults.
    std::vector<std::string> extraHexFiles;
    for (int i = 1; i < argc; ++i) {
        extraHexFiles.push_back(argv[i]);
    }

    System s(extraHexFiles);
    MainWindow w(&s);
    QObject::connect(&a, &QApplication::lastWindowClosed, &s, &System::stop);
    w.show();
    return a.exec();
}
