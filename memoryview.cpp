#include "memoryview.h"

#include <QFontDatabase>
#include <QPainter>
#include <QStyle>
#include <QScrollBar>

MemoryView::MemoryView(std::vector<uint8_t> &memory, int offset, QWidget *parent)
    : QAbstractScrollArea{parent}, memory(memory), offset(offset),
      digits {
          QStaticText("0"), QStaticText("1"), QStaticText("2"), QStaticText("3"),
          QStaticText("4"), QStaticText("5"), QStaticText("6"), QStaticText("7"),
          QStaticText("8"), QStaticText("9"), QStaticText("A"), QStaticText("B"),
          QStaticText("C"), QStaticText("D"), QStaticText("E"), QStaticText("F")
      },
      gap(3),
      nrows(1)
{
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    digitSize = fontMetrics().size(0, QString('0'));
    int w = 0;
    w += (4+16*2)*digitSize.width();
    w += 16 * gap + 1;
    viewport()->setMinimumWidth(w);
    //viewport()->setMaximumWidth(w);
    w += 2*frameWidth();
    w += style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    setMinimumWidth(w);
}

void MemoryView::resizeEvent(QResizeEvent *event)
{
    nrows = viewport()->height()/digitSize.height()-1;
    if (nrows < 1) {
        nrows = 1;
    }
    verticalScrollBar()->setPageStep(nrows);
    verticalScrollBar()->setRange(0, memory.size()/16 - nrows);
}

void MemoryView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.fillRect(0, 0,
                     viewport()->width(), viewport()->height(),
                     Qt::white);
    int x = 4*digitSize.width() + gap + (digitSize.width()/2);
    int y = 0;
    for (int i=0; i<16; i++) {
        painter.drawStaticText(x, y, digits[i]);
        x += 2*digitSize.width() + gap;
    }
    y += digitSize.height();
    for (int row=0; row<nrows; row++) {
        x = 0;
        int addr = offset + (verticalScrollBar()->value()+row)*16;
        for(int i=3; i>=0; i--) {
            painter.drawStaticText(x, y, digits[(addr>>i*4)&0xf]);
            x += digitSize.width();
        }
        x += gap;
        addr -= offset;
        for(int col=0; col<16; col++) {
            int data = memory[addr++];
            painter.drawStaticText(x, y, digits[(data>>4)&0xf]);
            x += digitSize.width();
            painter.drawStaticText(x, y, digits[data&0xf]);
            x += digitSize.width();
            x += gap;
        }
        y += digitSize.height();
    }
 }

void MemoryView::update()
{
    viewport()->update();
}
