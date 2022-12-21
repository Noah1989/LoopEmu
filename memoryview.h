#ifndef MEMORYVIEW_H
#define MEMORYVIEW_H

#include <QTableView>
#include <QWidget>

class MemoryView : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryView(std::vector<uint8_t> &memory, QWidget *parent = nullptr);

private:
    QTableView *tableView;
};

#endif // MEMORYVIEW_H
