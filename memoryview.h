#ifndef MEMORYVIEW_H
#define MEMORYVIEW_H

#include <QAbstractScrollArea>
#include <QStaticText>

class MemoryView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    MemoryView(std::vector<uint8_t> &memory, int offset, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public slots:
    void update();

private:
    std::vector<uint8_t> &memory;
    int offset;
    std::array<QStaticText, 16> digits;
    QSize digitSize;
    int gap;
    int nrows;
};

#endif // MEMORYVIEW_H
