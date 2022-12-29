#include "system.h"

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
