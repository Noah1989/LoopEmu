#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>

class Video : public QObject
{
    Q_OBJECT
public:
    Video(QObject *parent = nullptr);
    ~Video();

    std::array<uint8_t, 0x2000> nameMemory;
    std::array<uint8_t, 0x2000> attrMemory;
    std::array<uint8_t, 0x2000> pattMemory;
    std::array<uint8_t, 0x2000> paleMemory;
    uint16_t vaddr;
    uint8_t vscrx;
    uint8_t vscry;
    uint8_t vscrh;

    uint8_t inPort(uint16_t port);
    void outPort(uint16_t port, uint8_t value);

};

#endif // VIDEO_H
