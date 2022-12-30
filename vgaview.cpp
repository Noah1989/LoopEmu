#include "vgaview.h"

#include <QPainter>

VgaView::VgaView(std::array<uint8_t, 0x2000> &name,
                 std::array<uint8_t, 0x2000> &attr,
                 std::array<uint8_t, 0x2000> &patt,
                 std::array<uint8_t, 0x2000> &pale,
                 QWidget *parent)
    : QWidget{parent},
      name(name), attr(attr), patt(patt), pale(pale),
      pixels(new QRgb[640*480]),
      image((uchar*)pixels, 640, 480, QImage::Format_ARGB32),
      dirty(true)
{
    for(int n=0; n<0x100; n++) {
        int R = (n>>7) & 1;
        int r = (n>>6) & 1;
        int G = (n>>5) & 1;
        int g = (n>>4) & 1;
        int B = (n>>3) & 1;
        int b = (n>>2) & 1;
        int I = (n>>1) & 1;
        int i = (n>>0) & 1;
        int r4 = (R<<3)+(I<<2)+(r<<1)+(i<<0);
        int g4 = (G<<3)+(I<<2)+(g<<1)+(i<<0);
        int b4 = (B<<3)+(I<<2)+(b<<1)+(i<<0);
        colors[n] = qRgb(r4*0x11, g4*0x11, b4*0x11);
    }
}

QSize VgaView::minimumSizeHint() const
{
    return QSize(640, 480);
}

void VgaView::update()
{
    dirty = true;
    QWidget::update();
}

void VgaView::paintEvent(QPaintEvent*) {
    if (dirty) {
        render();
        dirty = false;
    }
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), Qt::black);
    painter.drawImage((width()-640)/2, (height()-480)/2, image);
}

void VgaView::render()
{
    static uint8_t t = 0;
    for (int x = 0; x < 640; ++x) {
        for (int y = 0; y < 480; ++y) {
            int tx = x/8;
            int ty = y/8;
            int px = x%8;
            int py = y%8;
            int t = 128*ty + tx;
            int n = name[t];
            int a = attr[t];
            int p = patt[32*n + 8*(py/2) + px];
            int i = py%2 ? (p>>4)&0xf : p&0xf;
            int c = pale[16*a + i];
            pixels[640*y + x] = colors[c];
        }
    }
    t++;
}
