#include "video.h"

Video::Video(QObject *parent)
    : QObject{parent}
{
    std::generate(nameMemory.begin(), nameMemory.end(), std::rand);
    std::generate(attrMemory.begin(), attrMemory.end(), std::rand);
    std::generate(pattMemory.begin(), pattMemory.end(), std::rand);
    std::generate(paleMemory.begin(), paleMemory.end(), std::rand);
}

Video::~Video()
{
}

uint8_t Video::inPort(uint16_t port)
{
    uint8_t result = 0;
    switch (port&0x0f) {
    case 0x08:
        result = nameMemory[vaddr];
        break;
    case 0x09:
        result = attrMemory[vaddr];
        break;
    case 0x0a:
        result = pattMemory[vaddr];
        break;
    case 0x0b:
        result = paleMemory[vaddr];
        break;
    case 0x0c:
        result = nameMemory[vaddr++];
        vaddr &= 0x1fff;
        break;
    case 0x0d:
        result = attrMemory[vaddr++];
        vaddr &= 0x1fff;
        break;
    case 0x0e:
        result = pattMemory[vaddr++];
        vaddr &= 0x1fff;
        break;
    case 0x0f:
        result = paleMemory[vaddr++];
        vaddr &= 0x1fff;
        break;
    }
    return result;
}

void Video::outPort(uint16_t port, uint8_t value)
{
    switch (port&0x0f) {
    case 0x00:
        vscrx = value;
        break;
    case 0x01:
        vscry = value;
        break;
    case 0x02:
        vscrh = value;
        break;
    case 0x03:
        vaddr &= 0xff00;
        vaddr |= value;
        vaddr &= 0x1fff;
        break;
    case 0x04:
        vaddr &= 0xff;
        vaddr |= value<<8;
        vaddr &= 0x1fff;
        break;
    case 0x08:
        nameMemory[vaddr] = value;
        break;
    case 0x09:
        attrMemory[vaddr] = value;
        break;
    case 0x0a:
        pattMemory[vaddr] = value;
        break;
    case 0x0b:
        paleMemory[vaddr] = value;
        break;
    case 0x0c:
        nameMemory[vaddr++] = value;
        vaddr &= 0x1fff;
        break;
    case 0x0d:
        attrMemory[vaddr++] = value;
        vaddr &= 0x1fff;
        break;
    case 0x0e:
        pattMemory[vaddr++] = value;
        vaddr &= 0x1fff;
        break;
    case 0x0f:
        paleMemory[vaddr++] = value;
        vaddr &= 0x1fff;
        break;
    }
}
