#include "cpuview.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

CpuView::CpuView(Z80 &cpu, QWidget *parent)
    : QWidget{parent}
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(new QLabel(QString("A")), 0, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.A), 0, 1);
    layout->addWidget(new QLabel(QString("F")), 0, 2);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.F), 0, 3);
    layout->addWidget(new QLabel(QString("A'")), 0, 4);
    layout->addWidget(makeRegisterView(&cpu.reg.back.A), 0, 5);
    layout->addWidget(new QLabel(QString("F'")), 0, 6);
    layout->addWidget(makeRegisterView(&cpu.reg.back.F), 0, 7);

    layout->addWidget(new QLabel(QString("B")), 1, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.B), 1, 1);
    layout->addWidget(new QLabel(QString("C")), 1, 2);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.C), 1, 3);
    layout->addWidget(new QLabel(QString("B'")), 1, 4);
    layout->addWidget(makeRegisterView(&cpu.reg.back.B), 1, 5);
    layout->addWidget(new QLabel(QString("C'")), 1, 6);
    layout->addWidget(makeRegisterView(&cpu.reg.back.C), 1, 7);

    layout->addWidget(new QLabel(QString("D")), 2, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.D), 2, 1);
    layout->addWidget(new QLabel(QString("E")), 2, 2);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.E), 2, 3);
    layout->addWidget(new QLabel(QString("D'")), 2, 4);
    layout->addWidget(makeRegisterView(&cpu.reg.back.D), 2, 5);
    layout->addWidget(new QLabel(QString("E'")), 2, 6);
    layout->addWidget(makeRegisterView(&cpu.reg.back.E), 2, 7);

    layout->addWidget(new QLabel(QString("H")), 3, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.H), 3, 1);
    layout->addWidget(new QLabel(QString("L")), 3, 2);
    layout->addWidget(makeRegisterView(&cpu.reg.pair.L), 3, 3);
    layout->addWidget(new QLabel(QString("H'")), 3, 4);
    layout->addWidget(makeRegisterView(&cpu.reg.back.H), 3, 5);
    layout->addWidget(new QLabel(QString("L'")), 3, 6);
    layout->addWidget(makeRegisterView(&cpu.reg.back.L), 3, 7);

    layout->addWidget(new QLabel(QString("IX")), 4, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.IX), 4, 1, 1, 3);
    layout->addWidget(new QLabel(QString("IY")), 4, 4);
    layout->addWidget(makeRegisterView(&cpu.reg.IY), 4, 5, 1, 3);

    layout->addWidget(new QLabel(QString("SP")), 5, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.SP), 5, 1, 1, 3);
    layout->addWidget(new QLabel(QString("PC")), 5, 4);
    layout->addWidget(makeRegisterView(&cpu.reg.PC), 5, 5, 1, 3);

    layout->addWidget(new QLabel(QString("I")), 6, 0);
    layout->addWidget(makeRegisterView(&cpu.reg.I), 6, 1);
    layout->addWidget(new QLabel(QString("R")), 6, 2);
    layout->addWidget(makeRegisterView(&cpu.reg.R), 6, 3);

}

QWidget* CpuView::makeRegisterView(uint8_t *data)
{
    QLineEdit *widget = new QLineEdit();
    widget->setInputMask(QString("HH;0"));
    widget->setMinimumWidth(widget->fontMetrics().boundingRect(QChar('0')).width()*4);
    widget->setReadOnly(true);
    return widget;
}

QWidget* CpuView::makeRegisterView(uint16_t *data)
{
    QLineEdit *widget = new QLineEdit();
    widget->setInputMask(QString("HHHH;0"));
    widget->setMinimumWidth(widget->fontMetrics().boundingRect(QChar('0')).width()*6);
    widget->setReadOnly(true);
    return widget;
}
