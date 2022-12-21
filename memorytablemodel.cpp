#include "memorytablemodel.h"

MemoryTableModel::MemoryTableModel(std::vector<uint8_t> &memory, QObject *parent)
    : QAbstractTableModel{parent},
      memory(memory)
{

}

int MemoryTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return memory.size() / columnCount();
}

int MemoryTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 16;
}

QVariant MemoryTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        uint16_t addr = index.row() * columnCount() + index.column();
        return QString("%1").arg(memory[addr], 2, 16, QChar('0'));
    }

    return QVariant();
}
