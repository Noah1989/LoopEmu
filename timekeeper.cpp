#include "timekeeper.h"
#include <ctime>

Timekeeper::Timekeeper(QObject *parent)
    : QObject{parent}
{
}

Timekeeper::~Timekeeper()
{
}

uint8_t bcd(uint8_t value)
{
    return value%100/10*0x10 + value%10;
}

uint8_t Timekeeper::getByte(uint16_t address)
{
    time_t now;
    time(&now);
    tm localNow = *localtime(&now);

    switch (address) {
    case 0x7fff:
        return bcd(localNow.tm_year);
    case 0x7ffe:
        return bcd(localNow.tm_mon+1);
    case 0x7ffd:
        return bcd(localNow.tm_mday);
    case 0x7ffc:
        return localNow.tm_wday == 0 ? 7 : localNow.tm_wday;
    case 0x7ffb:
        return bcd(localNow.tm_hour);
    case 0x7ffa:
        return bcd(localNow.tm_min);
    case 0x7ff9:
        return bcd(localNow.tm_sec);
    default:
        return 0;
    }
}
