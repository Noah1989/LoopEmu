#ifndef CPUVIEW_H
#define CPUVIEW_H

#include "z80.hpp"

#include <QWidget>
#include <QLabel>

class CpuView : public QWidget
{
    Q_OBJECT
public:
    CpuView(Z80 &cpu, QWidget *parent = nullptr);

public slots:
    void update(long cycles);

private:
    QWidget* makeRegisterView(uint8_t *data);
    QWidget* makeRegisterView(uint16_t *data);    
    std::vector<std::function<void()>> updaters;
    QLabel *freqLabel;
};

#endif // CPUVIEW_H
