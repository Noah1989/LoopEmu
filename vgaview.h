#ifndef VGAVIEW_H
#define VGAVIEW_H

#include <QWidget>

class VgaView : public QWidget
{
    Q_OBJECT
public:
    VgaView(std::array<uint8_t, 0x2000> &name,
            std::array<uint8_t, 0x2000> &attr,
            std::array<uint8_t, 0x2000> &patt,
            std::array<uint8_t, 0x2000> &pale,
            uint8_t &vscrx, uint8_t &vscry, uint8_t &vscrh,
            QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;

public slots:
    void update();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    std::array<uint8_t, 0x2000> &name;
    std::array<uint8_t, 0x2000> &attr;
    std::array<uint8_t, 0x2000> &patt;
    std::array<uint8_t, 0x2000> &pale;
    uint8_t &vscrx;
    uint8_t &vscry;
    uint8_t &vscrh;
    QRgb* pixels;
    QImage image;
    bool dirty;
    std::array<QRgb, 0x100> colors;
    void render();
};

#endif // VGAVIEW_H
