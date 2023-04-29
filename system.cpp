#include "system.h"
#include "intelhexclass.h"

#include <fstream>
#include <cmath>

System::System(QObject *parent)
    : QObject{parent},
      lowerMemory(0x8000),
      upperMemory(0x8000),
      cpu(this), mhz(9.830400), fps(60.0),
      video(this), sio(this),
      thread(new QThread(this)),
      timer(nullptr)
{    
    std::generate(lowerMemory.begin(), lowerMemory.end(), std::rand);
    std::generate(upperMemory.begin(), upperMemory.end(), std::rand);

    load("disk/bios.hex");
    load("disk/shell.hex");
    load("disk/charmap.hex");
    load("disk/test.hex");

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

uint8_t System::readByte(void *arg, uint16_t addr)
{
    System *self = static_cast<System*>(arg);
    if (addr < 0x8000) {
        return self->lowerMemory[addr];
    } else {
        return self->upperMemory[addr-0x8000];
    }
}

void System::writeByte(void *arg, uint16_t addr, uint8_t value)
{
    System *self = static_cast<System*>(arg);
    if (addr < 0x8000) {
        self->lowerMemory[addr] = value;
    } else {
        self->upperMemory[addr-0x8000] = value;
    }
}

uint8_t System::inPort(void *arg, uint16_t port)
{
    System *self = static_cast<System*>(arg);
    switch (port&0xf0) {
    case 0xb0:
        return self->video.inPort(port);
    case 0xc0:
        return self->sio.inPort(port);
    default:
        return 0;
    }
}

void System::outPort(void *arg, uint16_t port, uint8_t value)
{
    System *self = static_cast<System*>(arg);
    switch (port&0xf0) {
    case 0xb0:        
        self->video.outPort(port, value);
        break;
    case 0xc0:
        self->sio.outPort(port, value);
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
