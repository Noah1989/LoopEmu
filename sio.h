#ifndef SIO_H
#define SIO_H

#include <QObject>

class Sio : public QObject
{
    Q_OBJECT

public:
    Sio(QObject *parent = nullptr);
    ~Sio();
};

#endif // SIO_H
