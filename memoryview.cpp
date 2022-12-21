#include "memorytablemodel.h"
#include "memoryview.h"

#include <QHBoxLayout>

MemoryView::MemoryView(std::vector<uint8_t> &memory, QWidget *parent)
    : QWidget{parent},
      tableView(new QTableView)
{
    tableView->setModel(new MemoryTableModel(memory));
    tableView->resizeColumnsToContents();
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(tableView);
}
