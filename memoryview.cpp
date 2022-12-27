#include "memorytablemodel.h"
#include "memoryview.h"

#include <QFontDatabase>
#include <QHBoxLayout>
#include <QHeaderView>

MemoryView::MemoryView(std::vector<uint8_t> &memory, int offset, QWidget *parent)
    : QWidget{parent},
      tableView(new QTableView)
{
    tableView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    tableView->setModel(new MemoryTableModel(memory, offset));
    tableView->horizontalHeader()->setMinimumSectionSize(1);
    tableView->verticalHeader()->setMinimumSectionSize(1);
    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    int w = 0;
    w += tableView->contentsMargins().left();
    w += tableView->contentsMargins().right();
    w += tableView->verticalHeader()->width();
    for (int i=0; i<tableView->model()->columnCount(); ++i) {
        w += tableView->columnWidth(i);
    }
    w += tableView->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    tableView->setMinimumWidth(w);
    //tableView->setMaximumWidth(w);
    //setMaximumWidth(w);
    QHBoxLayout *layout = new QHBoxLayout(this);    
    layout->addWidget(tableView);
}

void MemoryView::update()
{
    tableView->update();
}

