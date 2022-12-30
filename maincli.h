#ifndef MAINCLI_H
#define MAINCLI_H

#include "system.h"
#include "consolelistener.h"

#include <QObject>

class MainCli : public QObject
{
    Q_OBJECT

public:
    MainCli(System *system, QObject *parent = nullptr);
    ~MainCli();
    void run();

private:
    ConsoleListener *consoleListener;

};
#endif // MAINCLI_H
