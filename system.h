#ifndef SYSTEM_H
#define SYSTEM_H

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
    std::array<uint8_t, 0x2000> nameMemory;
    std::array<uint8_t, 0x2000> attrMemory;
    std::array<uint8_t, 0x2000> pattMemory;
    std::array<uint8_t, 0x2000> paleMemory;

    Z80 cpu;
    double mhz;
    double fps;

signals:
    void frame(int cycles);
    void stopped();
    void serialOut(uint8_t c);

public slots:
    void start();
    void stop();
    void serialIn(uint8_t c);

private:    
    static unsigned char readByte(void* arg, unsigned short addr);
    static void writeByte(void* arg, unsigned short addr, unsigned char value);
    static unsigned char inPort(void* arg, unsigned short port);
    static void outPort(void* arg, unsigned short port, unsigned char value);
    QThread *thread;
    QTimer *timer;

private slots:
    void tick();

};

#endif // SYSTEM_H
