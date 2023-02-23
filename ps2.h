#ifndef PS2_H
#define PS2_H

#include <QKeyEvent>

struct ps2Code {
    size_t nbytes;
    uint8_t bytes[8];
};

ps2Code ps2CodeFromEvent(QKeyEvent *ev);

#endif // PS2_H
