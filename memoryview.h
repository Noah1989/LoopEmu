#ifndef MEMORYVIEW_H
#define MEMORYVIEW_H

#include <QTableView>
#include <QWidget>

class MemoryView : public QWidget
{
    Q_OBJECT
public:
    MemoryView(std::vector<uint8_t> &memory, int offset, QWidget *parent = nullptr);

public slots:
    void update();

private:
    QTableView *tableView;
};

#endif // MEMORYVIEW_H
