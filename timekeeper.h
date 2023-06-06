#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include <QObject>

class Timekeeper : public QObject
{
    Q_OBJECT
public:
    Timekeeper(QObject *parent = nullptr);
    ~Timekeeper();

    uint8_t getByte(uint16_t address);

};

#endif // TIMEKEEPER_H
