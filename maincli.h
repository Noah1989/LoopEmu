#ifndef MAINCLI_H
#define MAINCLI_H

#include "system.h"
#include "QConsoleListener/src/qconsolelistener.h"

#include <QObject>

class MainCli : public QObject
{
    Q_OBJECT

public:
    MainCli(System *system, QObject *parent = nullptr);
    ~MainCli();
    void run();

private:
    QConsoleListener *consoleListener;

};
#endif // MAINCLI_H
