#ifndef SYSTEM_H
#define SYSTEM_H

#include "sio.h"
#include "video.h"
#include "z80.hpp"
#include <QObject>
#include <QThread>
#include <QTimer>

class System : public QObject
{
    Q_OBJECT
public:
    System(QObject *parent = nullptr);
    ~System();
    std::vector<uint8_t> lowerMemory;
    std::vector<uint8_t> upperMemory;

    Z80 cpu;
    double mhz;
    double fps;

    Video video;

    Sio sio;

    void load(const std::string &hexFileName);

signals:
    void frame(int cycles);
    void stopped();

public slots:
    void start();
    void stop();

private:    
    static uint8_t readByte(void* arg, uint16_t addr);
    static void writeByte(void* arg, uint16_t addr, uint8_t value);
    static uint8_t inPort(void* arg, uint16_t port);
    static void outPort(void* arg, uint16_t port, uint8_t value);
    QThread *thread;
    QTimer *timer;

private slots:
    void tick();

};

#endif // SYSTEM_H
