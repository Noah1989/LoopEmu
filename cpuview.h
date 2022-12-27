#ifndef CPUVIEW_H
#define CPUVIEW_H

#include "z80.hpp"

#include <QWidget>

class CpuView : public QWidget
{
    Q_OBJECT
public:
    explicit CpuView(Z80 &cpu, QWidget *parent = nullptr);

private:
    QWidget* makeRegisterView(uint8_t *data);
    QWidget* makeRegisterView(uint16_t *data);

};

#endif // CPUVIEW_H
