#include "sio.h"
#include <iostream>

Sio::Sio(QObject *parent)
    : QObject{parent},
      fifoA(),
      fifoB()
{
}

Sio::~Sio()
{
}

std::queue<uint8_t> *Sio::fifo(Channel ch)
{
    switch(ch) {
    case Channel_A:
        return &fifoA;
        break;
    case Channel_B:
        return &fifoB;
        break;
    default:
        throw std::invalid_argument("invalid channel");
    }
}

uint8_t Sio::status(Channel ch)
{
    std::queue<uint8_t> *f = fifo(ch);
    return f->empty() ? 0x00 : 0x01;
}

uint8_t Sio::nextByte(Channel ch)
{
    std::queue<uint8_t> *f = fifo(ch);
    if (f->empty()) return 0;
    uint8_t result = f->front();
    f->pop();
    return result;
}

uint8_t Sio::inPort(uint16_t port)
{
    switch (port&0x0f) {
    case 0x00:
        return nextByte(Channel_A);
    case 0x01:
        return nextByte(Channel_B);
    case 0x02:
        return status(Channel_A);
    case 0x03:
        return status(Channel_B);
    default:
        return 0;
    }
}


void Sio::outPort(uint16_t port, uint8_t value)
{
    switch (port&0x0f) {
    case 0x00:
        emit transmit(Channel_A, value);
        break;
    case 0x01:
        emit transmit(Channel_B, value);
        break;
    }
}


void Sio::receive(Sio::Channel ch, uint8_t byte)
{
    std::queue<uint8_t> *f = fifo(ch);
    if (f->size() >= 3) {
        std::cerr << "SIO channel " << (char)('A'+ch) << ": FIFO is full" << std::endl;
        // return; // full
    }
    f->push(byte);
}
