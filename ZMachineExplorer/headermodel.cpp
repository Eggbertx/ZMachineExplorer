#include <QColor>
#include <enums.h>
#include "headermodel.h"

HeaderModel::HeaderModel(ZMachineCore::ZMachineVM *vm, TableMode mode, QObject *parent)
    : QAbstractTableModel{parent}
{
    m_vm = vm;
    m_mode = mode;
}

QVariant HeaderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    using namespace ZMachineCore;
    if(role == Qt::DisplayRole && orientation == Qt::Vertical) {
        switch(m_mode) {
        case TableMode::Header:
            return headerDataModeHeader(section);
        case TableMode::Flags1:
            return headerDataModeFlags1(section);
        case TableMode::Flags2:
            return headerDataModeFlags2(section);
        default:
            break;
        }
    }
    return QVariant();
}

int HeaderModel::rowCount(const QModelIndex &parent) const
{
    switch(m_mode) {
    case TableMode::Header:
        return 29;
    case TableMode::Flags1:
        return 8;
    case TableMode::Flags2:
        return 16;
    default:
        return 0;
    }
}

int HeaderModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void HeaderModel::refresh(int r1, int c1, int r2, int c2) {
    emit dataChanged(index(r1, c1), index(r2, c2));
}

void HeaderModel::refresh() {
    refresh(0, 0, rowCount() - 1, columnCount() - 1);
}

QVariant HeaderModel::headerDataModeHeader(int section) const {
    switch (section) {
    case 0:
        return "Version";
    case 1:
        return "Flags 1";
    case 2:
        return "High Memory Base";
    case 3:
        return "Initial PC";
    case 4:
        return "Dictionary Address";
    case 5:
        return "Objects Address";
    case 6:
        return "Globals Address";
    case 7:
        return "Static Address";
    case 8:
        return "Flags 2";
    case 9:
        return "Abbreviations Address";
    case 10:
        return "File Length";
    case 11:
        return "Checksum";
    case 12:
        return "Interpreter Number";
    case 13:
        return "Interpreter Revision";
    case 14:
        return "Screen Height Lines";
    case 15:
        return "Screen Width Chars";
    case 16:
        return "Screen Width Units";
    case 17:
        return "Screen Height Units";
    case 18:
        return "Font Width";
    case 19:
        return "Font Height";
    case 20:
        return "Routines Offset";
    case 21:
        return "Strings Offset";
    case 22:
        return "Default Background";
    case 23:
        return "Default Foreground";
    case 24:
        return "Terminating Chars Table Address";
    case 25:
        return "Stream 3 Width";
    case 26:
        return "Standard Revision Number";
    case 27:
        return "Alphabet Table Address";
    case 28:
        return "Header Extension Table Address";
    default:
        return QVariant();
    }
}

QVariant HeaderModel::headerDataModeFlags1(int section) const {
    switch(section) {

    default:
        return QVariant();
    }
}

QVariant HeaderModel::headerDataModeFlags2(int section) const {
    return QVariant();
}

QVariant HeaderModel::dataModeHeader(int row) const {
    using namespace ZMachineCore;
    switch (row) {
    case 0:
        return m_vm->getInt<quint8>(HeaderAddress::Version);
    case 1:
        return QString("%1").arg(m_vm->getInt<quint8>(HeaderAddress::Flags1), 8, 2, '0');
    case 2:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::HighMemoryBase), 4, 16, '0');
    case 3:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::InitialPC), 4, 16, '0');
    case 4:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::DictionaryAddr), 4, 16, '0');
    case 5:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::ObjectsAddr), 4, 16, '0');
    case 6:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::GlobalsAddr), 4, 16, '0');
    case 7:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::StaticAddr), 4, 16, '0');
    case 8:
        return QString("%1").arg(m_vm->getInt<quint16>(HeaderAddress::Flags2), 16, 2, '0');
    case 9:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::AbbreviationsAddr), 4, 16, '0');
    case 10:
        return QString("%1 B").arg(m_vm->getInt<quint16>(HeaderAddress::FileLength));
    case 11:
        return QString("%1").arg(m_vm->getInt<quint16>(HeaderAddress::Checksum), 4, 16, '0');
    case 12:
        return m_vm->getInt<quint8>(HeaderAddress::InterpreterNum);
    case 13:
        return m_vm->getInt<quint8>(HeaderAddress::InterpreterRev);
    case 14:
        return m_vm->getInt<quint8>(HeaderAddress::ScreenHeightLines);
    case 15:
        return m_vm->getInt<quint8>(HeaderAddress::ScreenWidthChars);
    case 16:
        return m_vm->getInt<quint16>(HeaderAddress::ScreenWidthUnits);
    case 17:
        return m_vm->getInt<quint16>(HeaderAddress::ScreenHeightUnits);
    case 18:
        return m_vm->getInt<quint8>(HeaderAddress::FontWidth);
    case 19:
        return m_vm->getInt<quint8>(HeaderAddress::FontHeight);
    case 20:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::RoutinesOffset), 4, 16, '0');
    case 21:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::StringsOffset), 4, 16, '0');
    case 22:
        return QString("$%1").arg(m_vm->getInt<quint8>(HeaderAddress::DefaultBG), 2, 16, '0');
    case 23:
        return QString("$%1").arg(m_vm->getInt<quint8>(HeaderAddress::DefaultFG), 2, 16, '0');
    case 24:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::TerminatingCharsTableAddr), 4, 16, '0');
    case 25:
        return m_vm->getInt<quint16>(HeaderAddress::Stream3Width);
    case 26:
        return m_vm->getInt<quint16>(HeaderAddress::StandardRevNum);
    case 27:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::AlphabetTableAddr), 4, 16, '0');
    case 28:
        return QString("$%1").arg(m_vm->getInt<quint16>(HeaderAddress::HeaderExtTableAddr), 4, 16, '0');
    default:
        return QVariant();
    }
}

QVariant HeaderModel::dataModeFlags1(int row) const {
    return QVariant();
}

QVariant HeaderModel::dataModeFlags2(int row) const {
    return QVariant();
}

QVariant HeaderModel::data(const QModelIndex &index, int role) const
{
    using namespace ZMachineCore;
    if(role == Qt::DisplayRole && m_vm->fileSize() > 0) {
        switch(m_mode) {
        case TableMode::Header:
            return dataModeHeader(index.row());
        case TableMode::Flags1:
            return dataModeFlags1(index.row());
        case TableMode::Flags2:
            return dataModeFlags2(index.row());
        default:
            break;
        }
    }
    return QVariant();
}
