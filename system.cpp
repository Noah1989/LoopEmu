#include "system.h"

System::System(QObject *parent)
    : QObject{parent},
      lowerMemory(0x8000),
      upperMemory(0x8000),
      cpu(this)
{
    cpu.setupCallbackFP(readByte, writeByte, inPort, outPort);
}

unsigned char System::readByte(void *arg, unsigned short addr)
{
    System *self = static_cast<System*>(arg);
    if (addr < 0x8000) {
        return self->lowerMemory[addr];
    } else {
        return self->upperMemory[addr-0x8000];
    }
}

void System::writeByte(void *arg, unsigned short addr, unsigned char value)
{
    System *self = static_cast<System*>(arg);
    if (addr < 0x8000) {
        self->lowerMemory[addr] = value;
    } else {
        self->upperMemory[addr-0x8000] = value;
    }
}

unsigned char System::inPort(void *arg, unsigned short port)
{
    System *self = static_cast<System*>(arg);
    return 0;
}

void System::outPort(void *arg, unsigned short port, unsigned char value)
{
    System *self = static_cast<System*>(arg);
}
