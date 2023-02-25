#ifndef SIO_H
#define SIO_H

#include <QObject>
#include <queue>

class Sio : public QObject
{
    Q_OBJECT

public:
    Sio(QObject *parent = nullptr);
    ~Sio();

    uint8_t inPort(uint16_t port);
    void outPort(uint16_t port, uint8_t value);

    enum Channel { Channel_A, Channel_B };

signals:
    void transmit(Sio::Channel ch, uint8_t byte);

public slots:
    void receive(Sio::Channel ch, uint8_t byte);

private:
    std::queue<uint8_t> fifoA;
    std::queue<uint8_t> fifoB;
    std::queue<uint8_t> *fifo(Channel ch);
    uint8_t status(Channel ch);
    uint8_t nextByte(Channel ch);
};

#endif // SIO_H
