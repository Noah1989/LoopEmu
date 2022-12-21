#ifndef SYSTEM_H
#define SYSTEM_H

#include "z80.hpp"
#include <QObject>

class System : public QObject
{
    Q_OBJECT
public:
    explicit System(QObject *parent = nullptr);
    std::vector<uint8_t> lowerMemory;
    std::vector<uint8_t> upperMemory;
    Z80 cpu;

private:
    static unsigned char readByte(void* arg, unsigned short addr);
    static void writeByte(void* arg, unsigned short addr, unsigned char value);
    static unsigned char inPort(void* arg, unsigned short port);
    static void outPort(void* arg, unsigned short port, unsigned char value);
};

#endif // SYSTEM_H
