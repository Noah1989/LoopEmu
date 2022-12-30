#include "maincli.h"

MainCli::MainCli(System *system, QObject *parent)
    : QObject(parent),
      consoleListener(new ConsoleListener)
{
    std::cout << std::unitbuf;
    connect(consoleListener, &ConsoleListener::newChar,
            system, &System::serialIn);
    connect(system, &System::serialOut,
            [](uint8_t c){ std::cout << (char)c; });
} 

MainCli::~MainCli()
{
}

void MainCli::run()
{
}
