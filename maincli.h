#ifndef MAINCLI_H
#define MAINCLI_H

#include "system.h"

#include <QObject>

class MainCli : public QObject
{
    Q_OBJECT

public:
    MainCli(System *system, QObject *parent = nullptr);
    ~MainCli();
    void run();

};
#endif // MAINCLI_H
