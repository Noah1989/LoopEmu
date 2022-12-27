#ifndef MEMORYTABLEMODEL_H
#define MEMORYTABLEMODEL_H

#include <QAbstractTableModel>

class MemoryTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryTableModel(std::vector<uint8_t> &memory, int offset, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    std::vector<uint8_t> &memory;
    int offset;
};

#endif // MEMORYTABLEMODEL_H
