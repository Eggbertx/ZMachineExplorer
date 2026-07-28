#ifndef HEADERMODEL_H
#define HEADERMODEL_H

#include <zmachinevm.h>
#include <QAbstractTableModel>

class HeaderModel : public QAbstractTableModel
{
public:
    enum TableMode {
        Unknown, Header, Flags1, Flags2
    };

    explicit HeaderModel(ZMachineCore::ZMachineVM* vm, TableMode mode, QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void refresh(int r1, int c1, int r2, int c2);
    void refresh();

private:
    QVariant headerDataModeHeader(int section) const;
    QVariant headerDataModeFlags1(int section) const;
    QVariant headerDataModeFlags2(int section) const;
    QVariant dataModeHeader(int row) const;
    QVariant dataModeFlags1(int row) const;
    QVariant dataModeFlags2(int row) const;
    ZMachineCore::ZMachineVM* m_vm;
    TableMode m_mode;
};

#endif // HEADERMODEL_H
