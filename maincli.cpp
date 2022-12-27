#include "maincli.h"

MainCli::MainCli(System *system, QObject *parent)
    : QObject(parent),
      consoleListener(new QConsoleListener)
{
}

MainCli::~MainCli()
{
}

void MainCli::run()
{
}
