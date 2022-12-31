#include "system.h"
#include "intelhexclass.h"

#include <fstream>

System::System(QObject *parent)
    : QObject{parent},
      lowerMemory(0x8000),
      upperMemory(0x8000),
      cpu(this), mhz(9.830400), fps(60.0),
      thread(new QThread(this)),
      timer(nullptr)
{    
    std::generate(lowerMemory.begin(), lowerMemory.end(), std::rand);
    std::generate(upperMemory.begin(), upperMemory.end(), std::rand);
    std::generate(nameMemory.begin(), nameMemory.end(), std::rand);
    std::generate(attrMemory.begin(), attrMemory.end(), std::rand);
    std::generate(pattMemory.begin(), pattMemory.end(), std::rand);
    std::generate(paleMemory.begin(), paleMemory.end(), std::rand);

    load("disk/bios.hex");
    load("disk/charmap.hex");

    cpu.setupCallbackFP(readByte, writeByte, inPort, outPort);
    moveToThread(thread);
    connect(thread, &QThread::started, this, &System::start);
    thread->start();
}

System::~System()
{
    thread->quit();
    thread->wait();
}

void System::load(const std::string &hexFileName)
{
    std::ifstream intelHexInput;
    intelHexInput.open(hexFileName, ifstream::in);
    intelhex data;
    intelHexInput >> data;
    data.begin();
    while (true) {
        uint8_t byte;
        data.getData(&byte);
        writeByte(this, data.currentAddress(), byte);
        if (data.endOfData()) {
            break;
        }
        data.incrementAddress();
    }
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
    switch (port&0xff) {
    case 0xb0:
        self->vscrx = value;
        break;
    case 0xb1:
        self->vscry = value;
        break;
    case 0xb2:
        self->vscrh = value;
        break;
    case 0xb3:
        self->vaddr &= 0xff00;
        self->vaddr |= value;
        break;
    case 0xb4:
        self->vaddr &= 0xff;
        self->vaddr |= value<<4;
        break;
    case 0xb8:
        self->nameMemory[self->vaddr] = value;
        break;
    case 0xb9:
        self->attrMemory[self->vaddr] = value;
        break;
    case 0xba:
        self->pattMemory[self->vaddr] = value;
        break;
    case 0xbb:
        self->paleMemory[self->vaddr] = value;
        break;
    case 0xbc:
        self->nameMemory[self->vaddr++] = value;
        self->vaddr &= 0x1fff;
        break;
    case 0xbd:
        self->attrMemory[self->vaddr++] = value;
        self->vaddr &= 0x1fff;
        break;
    case 0xbe:
        self->pattMemory[self->vaddr++] = value;
        self->vaddr &= 0x1fff;
        break;
    case 0xbf:
        self->paleMemory[self->vaddr++] = value;
        self->vaddr &= 0x1fff;
        break;
    }
}

void System::start()
{
    if (timer == nullptr) {
        timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        connect(timer, &QTimer::timeout, this, &System::tick);
        int interval = round(1000.0/fps);
        fps = 1000.0/interval;
        timer->setInterval(interval);
    }
    timer->start();
}

void System::tick()
{
    emit frame(cpu.execute(round(1e6*mhz/fps)));
}

void System::stop()
{
    if (timer != nullptr) {
        timer->stop();
    }
    emit stopped();
}

void System::serialIn(uint8_t c) {
    emit serialOut(c);
}
