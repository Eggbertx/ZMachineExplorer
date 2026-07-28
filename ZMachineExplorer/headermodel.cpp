#include "headermodel.h"
#include <QColor>
#include <enums.h>

HeaderModel::HeaderModel(ZMachineCore::ZMachineVM *vm, TableMode mode, QObject *parent)
    : QAbstractTableModel{parent}
{
    m_vm = vm;
    m_mode = mode;
}

QVariant HeaderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    using namespace ZMachineCore;
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        switch (m_mode) {
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
    quint8 version = m_vm->zMachineVersion();
    switch (m_mode) {
    case TableMode::Header:
        return 29;
    case TableMode::Flags1:
        switch (version) {
        case 0:
            return 5;
        case 1:
        case 2:
        case 3:
            return 5;
        case 4:
            return 4;
        case 5:
            return 5;
        default:
            return 7;
        }
    case TableMode::Flags2:
        return 9;
    default:
        return 0;
    }
}

int HeaderModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void HeaderModel::refresh(int r1, int c1, int r2, int c2)
{
    emit dataChanged(index(r1, c1), index(r2, c2));
    emit headerDataChanged(Qt::Horizontal, 0, m_rows - 1);
}

void HeaderModel::refresh()
{
    int tmpRowCount = rowCount();
    if (tmpRowCount != m_rows) {
        m_rows = tmpRowCount;
        beginResetModel();
        endResetModel();
    } else {
        refresh(0, 0, m_rows - 1, columnCount() - 1);
    }
}

QVariant HeaderModel::headerDataModeHeader(int section) const
{
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

QVariant HeaderModel::headerDataModeFlags1(int section) const
{
    quint8 version = m_vm->zMachineVersion();
    if (version < 4) {
        switch (section) {
        case 0:
            return "Status Line";
        case 1:
            return "Split Story File";
        case 2:
            return "Status Line Unavailable";
        case 3:
            return "Screen Splitting Available";
        case 4:
            return "Variable-width Font Default";
        }
    } else if (version == 4) {
        switch (section) {
        case 0:
            return "Bold Font Available";
        case 1:
            return "Italic Font Available";
        case 2:
            return "Fixed-width Font Available";
        case 3:
            return "Timed Keyboard Input Available";
        }
    } else if (version == 5) {
        switch (section) {
        case 0:
            return "Colors Available";
        case 1:
            return "Bold Font Available";
        case 2:
            return "Italic Font Available";
        case 3:
            return "Fixed-width Font Available";
        case 4:
            return "Timed Keyboard Input Available";
        }
    } else {
        switch (section) {
        case 0:
            return "Colors Available";
        case 1:
            return "Picture Displaying Available";
        case 2:
            return "Bold Font Available";
        case 3:
            return "Italic Font Available";
        case 4:
            return "Fixed-width Font Available";
        case 5:
            return "Sound Available";
        case 6:
            return "Timed Keyboard Input Available";
        }
    }
    return QVariant();
}

QVariant HeaderModel::headerDataModeFlags2(int section) const
{
    using namespace ZMachineCore;
    if(section > 8) return QVariant();
    switch(section) {
    case 0:
        return "Transcripting On";
    case 1:
        return "Force Fixed-width Font";
    case 2:
        return "Screen Redraw";
    case 3:
        return "Game Uses Pictures";
    case 4:
        return "Game Uses Undo";
    case 5:
        return "Game Uses Mouse";
    case 6:
        return "Game Uses Colors";
    case 7:
        return "Game Uses Sounds";
    case 8:
        return "Game Uses Menus";
    default:
        break;
    }
    return QVariant();
}

QVariant HeaderModel::dataModeHeader(int row) const
{
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

QVariant HeaderModel::dataModeFlags1(int row) const
{
    using namespace ZMachineCore;
    quint8 flagBit = 255;
    quint8 version = m_vm->zMachineVersion();
    if (version < 4) {
        switch (row) {
        case 0:
            // Status Line
            flagBit = 1;
            break;
        case 1:
            // Split Story File
            flagBit = 2;
            break;
        case 2:
            // Status Line Unavailable
            flagBit = 4;
            break;
        case 3:
            // Screen Splitting Available
            flagBit = 5;
            break;
        case 4:
            // Variable-width Font Default
            flagBit = 6;
            break;
        default:
            break;
        }
    } else if (version == 4) {
        switch (row) {
        case 0:
            // Bold font available
            flagBit = 2;
            break;
        case 1:
            // Italic font available
            flagBit = 3;
            break;
        case 2:
            // Fixed-width font available
            flagBit = 4;
            break;
        case 3:
            // Timed keyboard input available
            flagBit = 7;
            break;
        default:
            break;
        }
    } else if (version == 5) {
        switch (row) {
        case 0:
            // Colors available
            flagBit = 0;
            break;
        case 1:
            // Bold font available
            flagBit = 2;
            break;
        case 2:
            // Italic font available
            flagBit = 3;
            break;
        case 3:
            // Fixed-width font available
            flagBit = 4;
            break;
        case 4:
            // Timed keyboard input available
            flagBit = 7;
            break;
        default:
            break;
        }
    } else {
        switch (row) {
        case 0:
            // Colors available
            flagBit = 0;
            break;
        case 1:
            // Picture displaying available
            flagBit = 1;
            break;
        case 2:
            // Bold font available
            flagBit = 2;
            break;
        case 3:
            // Italic font available
            flagBit = 3;
            break;
        case 4:
            // Fixed-width font available
            flagBit = 4;
            break;
        case 5:
            // Sound available
            flagBit = 5;
            break;
        case 6:
            // Timed keyboard input available
            flagBit = 7;
            break;
        default:
            break;
        }
    }
    if (flagBit < 255) {
        quint8 flags1 = m_vm->getInt<quint8>(HeaderAddress::Flags1);
        return ((flags1 & (1 << (flagBit & 7))) > 0) ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();
}

QVariant HeaderModel::dataModeFlags2(int row) const
{
    using namespace ZMachineCore;
    if(row > 8) return QVariant();
    quint16 val = m_vm->getInt<quint16>(HeaderAddress::Flags2);
    bool checked = (val & (1 << (row & 31))) > 0;
    return checked ? Qt::Checked:Qt::Unchecked;
}

QVariant HeaderModel::data(const QModelIndex &index, int role) const
{
    using namespace ZMachineCore;
    if (m_vm->fileSize() == 0) {
        return QVariant();
    }
    if (role == Qt::DisplayRole && m_mode == TableMode::Header) {
        return dataModeHeader(index.row());
    } else if (role == Qt::CheckStateRole) {
        switch (m_mode) {
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
