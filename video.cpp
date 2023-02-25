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

void Video::outPort(unsigned short port, unsigned char value)
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
