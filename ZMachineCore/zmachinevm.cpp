#include "zmachinevm.h"
#include <QException>
#include <QFile>
#include "illegalopcodeexception.h"

namespace ZMachineCore {

ZMachineVM::ZMachineVM()
{
    m_lastError = "";
    m_filePath = "";
    defineInstructions();
}

bool ZMachineVM::loadFromFile(const QString &filePath)
{
    QFile storyFile(filePath);
    if (!storyFile.open(QIODevice::ReadOnly)) {
        m_lastError = storyFile.errorString();
        return false;
    }
    populate(storyFile.readAll());
    storyFile.close();
    m_filePath = filePath;
    reset();
    return true;
}

bool ZMachineVM::loadFromBytes(QByteArray bytes, QString filePath)
{
    populate(bytes);
    m_filePath = filePath;
    reset();
    return true;
}

void ZMachineVM::reset()
{
    setInt<quint8>(HeaderAddress::InterpreterNum, m_interpreterNum, MemoryWriteSource::ResetSource);
    m_PC = getInt<quint16>(HeaderAddress::InitialPC);
}

QString &ZMachineVM::lastError()
{
    return m_lastError;
}

QString &ZMachineVM::filePath()
{
    return m_filePath;
}

quint32 ZMachineVM::fileSize()
{
    return memorySize();
}

void ZMachineVM::setInterpreterNum(enum InterpreterNum num, bool setInFile, bool isReset)
{
    m_interpreterNum = num;
    if (setInFile && memorySize() > 0) {
        setInt<quint8>(HeaderAddress::InterpreterNum,
                       num,
                       isReset ? MemoryWriteSource::ResetSource
                               : MemoryWriteSource::InterpreterSource);
    }
}

enum InterpreterNum ZMachineCore::ZMachineVM::interpreterNumber()
{
    return m_interpreterNum;
}

quint8 ZMachineVM::zMachineVersion()
{
    return getInt<quint8>(0);
}

bool ZMachineVM::validateMemoryWrite(quint16 addr, enum MemoryWriteSource source)
{
    MemoryRegionType regionType = memoryRegion(addr);
    switch (regionType) {
    case MemoryRegionType::UnknownMemory:
        m_operationStatus = MemoryOperationStatus::MemoryNotInitialized;
        return false;
    case MemoryRegionType::DynamicMemory:
        if (addr <= HeaderAddress::HeaderExtTableAddr + 1) {
            return validateHeaderWrite(addr, source);
        }
        return true;
    default:
        // static or high memory
        return false;
    }
}

bool ZMachineVM::validateHeaderWrite(quint16 addr, MemoryWriteSource source)
{
    switch (addr) {
    case HeaderAddress::Version:
    case HeaderAddress::Flags1:
    case HeaderAddress::Flags1 + 1:
    case HeaderAddress::Flags1 + 2:
    case HeaderAddress::HighMemoryBase:
    case HeaderAddress::HighMemoryBase + 1:
    case HeaderAddress::InitialPC:
    case HeaderAddress::InitialPC + 1:
    case HeaderAddress::DictionaryAddr:
    case HeaderAddress::DictionaryAddr + 1:
    case HeaderAddress::ObjectsAddr:
    case HeaderAddress::ObjectsAddr + 1:
    case HeaderAddress::GlobalsAddr:
    case HeaderAddress::GlobalsAddr + 1:
    case HeaderAddress::StaticAddr:
    case HeaderAddress::StaticAddr + 1:
    case HeaderAddress::Flags2:
    case HeaderAddress::Flags2 + 1:
    case HeaderAddress::Flags2 + 2:
    case HeaderAddress::Flags2 + 3:
    case HeaderAddress::Flags2 + 4:
    case HeaderAddress::Flags2 + 5:
    case HeaderAddress::Flags2 + 6:
    case HeaderAddress::Flags2 + 7:
    case HeaderAddress::AbbreviationsAddr:
    case HeaderAddress::AbbreviationsAddr + 1:
    case HeaderAddress::FileLength:
    case HeaderAddress::FileLength + 1:
    case HeaderAddress::Checksum:
    case HeaderAddress::Checksum + 1:
    case HeaderAddress::RoutinesOffset:
    case HeaderAddress::RoutinesOffset + 1:
    case HeaderAddress::StringsOffset:
    case HeaderAddress::StringsOffset + 1:
    case HeaderAddress::TerminatingCharsTableAddr:
    case HeaderAddress::TerminatingCharsTableAddr + 1:
    case HeaderAddress::AlphabetTableAddr:
    case HeaderAddress::AlphabetTableAddr + 1:
    case HeaderAddress::HeaderExtTableAddr:
    case HeaderAddress::HeaderExtTableAddr + 1:
        // all read-only addresses, except for Flags1 and Flags2, which must have flags set individually
        return false;
    case HeaderAddress::InterpreterNum:
    case HeaderAddress::InterpreterRev:
    case HeaderAddress::ScreenHeightLines:
    case HeaderAddress::ScreenWidthChars:
    case HeaderAddress::ScreenWidthUnits:
    case HeaderAddress::ScreenWidthUnits + 1:
    case HeaderAddress::ScreenHeightUnits:
    case HeaderAddress::ScreenHeightUnits + 1:
    case HeaderAddress::FontWidth:
    case HeaderAddress::FontHeight:
    case HeaderAddress::DefaultBG:
    case HeaderAddress::DefaultFG:
    case HeaderAddress::StandardRevNum:
        return source == MemoryWriteSource::InterpreterSource
               || source == MemoryWriteSource::ResetSource;
    case HeaderAddress::Stream3Width:
        return source == MemoryWriteSource::InterpreterSource;
    }
    return true;
}

MemoryRegionType ZMachineVM::memoryRegion(quint16 addr)
{
    if (memorySize() == 0) {
        return MemoryRegionType::UnknownMemory;
    }
    quint16 staticStart = ZMachineMemory::getInt<quint16>(HeaderAddress::StaticAddr);
    quint16 highStart = ZMachineMemory::getInt<quint16>(HeaderAddress::HighMemoryBase);
    if (addr < staticStart) {
        return MemoryRegionType::DynamicMemory;
    }
    if (addr >= staticStart && addr < highStart) {
        return MemoryRegionType::StaticMemory;
    }
    return MemoryRegionType::HighMemory;
}

QList<zobject_header> &ZMachineVM::getObjectList()
{
    if (m_objectList.length() > 0) {
        return m_objectList;
    }
    quint8 version = zMachineVersion();
    if (m_operationStatus != MemoryOperationStatus::OK)
        return m_objectList;

    quint16 objAddr = getInt<quint16>(HeaderAddress::ObjectsAddr);
    if (m_operationStatus != MemoryOperationStatus::OK)
        return m_objectList;

    quint16 maxObjects;
    quint8 numFlags;
    quint16 propertyTableMin = 65535;
    quint16 firstObjectAddr;
    if (version < 4) {
        maxObjects = 255;
        numFlags = 4;
        firstObjectAddr = objAddr + 31 * 2; // temporarily skipping the default properties table
    } else {
        maxObjects = 65535;
        numFlags = 6;
        firstObjectAddr = objAddr + 63 * 2;
    }
    while (objAddr < firstObjectAddr) {
        quint16 w = getInt<quint16>(objAddr);
        if (m_operationStatus != MemoryOperationStatus::OK)
            return m_objectList;
        m_defaultProperties.append(w);
        objAddr += 2;
    }
    for (int i = 0; i < maxObjects; i++) {
        zobject_header header;
        if (objAddr >= propertyTableMin) {
            // object table ends where property table begins
            // qDebug("Reached property table, number of objects: %d", numObjects());
            break;
        }
        for (int f = 0; f < numFlags; f++) {
            header.flags[f] = getInt<quint8>(objAddr++);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
        }
        if (version < 4) {
            header.parent = getInt<quint8>(objAddr++);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
            header.parent = getInt<quint8>(objAddr++);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
            header.parent = getInt<quint8>(objAddr++);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
        } else {
            header.parent = getInt<quint16>(objAddr);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
            header.sibling = getInt<quint16>(objAddr + 2);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
            header.child = getInt<quint16>(objAddr + 4);
            if (m_operationStatus != MemoryOperationStatus::OK)
                return m_objectList;
            objAddr += 6;
        }
        header.properties = getInt<quint16>(objAddr);
        if (m_operationStatus != MemoryOperationStatus::OK)
            return m_objectList;
        objAddr += 2;
        if (header.properties < propertyTableMin) {
            propertyTableMin = header.properties;
        }
        // qDebug(
        //     "Object %d at %x\n\tParent: %d\n\tSibling: %d\n\tChild: %d\n\tProperties: %d\n\n",
        //     i, objAddr, header.parent, header.sibling, header.child, header.properties);
        m_objectList.append(header);
    }

    return m_objectList;
}

void ZMachineVM::defineInstructions()
{
    REGISTER_INSTRUCTION(0xb0, implRTrue, 1, 0);
    REGISTER_INSTRUCTION(0xb1, implRFalse, 1, 0);
    REGISTER_INSTRUCTION(0xba, implQuit, 1, 0);
    REGISTER_INSTRUCTION(0xbe, implPrintRet, 1, 0);
    REGISTER_INSTRUCTION(0xe0, implCallVS, 4, 0);
}

void ZMachineVM::executeInstruction(quint8 opcode)
{
    auto it = m_instructions.find(opcode);
    if (it != m_instructions.end()) {
        const InstructionEntry& entry = it.value();
        QList<quint16> args; // TODO: decode arguments to be passed to function
        quint8 version = zMachineVersion();
        if(version >= entry.minVersion && (entry.maxVersion < 0 || version <= entry.maxVersion)) {
            // Originally was going to have each instruction implementation named _V<version>, for instructions with different possible effects.
            // Now implementation handles this
            entry.fn(*this, args);
        }
    }
    throw IllegalOpcodeException(opcode, "", zMachineVersion());
}

void ZMachineVM::executeInstruction()
{

}

InstructionData ZMachineVM::decodeInstruction(quint8 address)
{
    InstructionData data;

    quint8 byteVal = getInt<quint8>(address);
    if(address == 0xbe) {
        data.form = InstructionForm::ExtendedForm;
        data.opcode = getInt<quint8>(address + 1);
        data.operandCount = 4;
    } else {
        switch((byteVal & 0b11000000) >> 6) {
        case 2:
            data.form = InstructionForm::ShortForm;
            data.opcode = byteVal & 0xf;
            if((byteVal & 0b110000) == 0) {
                data.operandCount = 1;
                data.operandTypes.resize(1);
                data.operandTypes[0] = static_cast<OperandType>((byteVal >> 4) & 0b11);
            } else {
                data.operandCount = 0;
            }
            break;
        case 3:
            data.form = InstructionForm::VariableForm;
            data.opcode = byteVal & 0xf;
            if(data.opcode == 12 || data.opcode == 26) {
                // call_vn2, call_vs2 can both take up to 8 operands
                data.operandCount = 8;
            } else if((byteVal & 0b100000) == 0) {
                data.operandCount = 2;
            } else {
                data.operandCount = 4;
            }
            break;
        default:
            // 0xxxxxxx
            data.form = InstructionForm::LongForm;
            data.operandCount = 2;
            data.opcode = byteVal & 0b11111;
            data.operandTypes.resize(2);
            data.operandTypes[0] = ((byteVal & 0b1000000 ) == 0)? OperandType::SmallConstant : OperandType::VariableOperand;
            data.operandTypes[1] = ((byteVal & 0b100000 ) == 0)? OperandType::SmallConstant : OperandType::VariableOperand;
        }
    }

    if(data.operandCount >= 4) {
        int start = (data.operandCount == 8)?14:6;
        quint16 types = getInt<quint16>(address + 2);
        OperandType type;
        for(int i = start; i >= 0; i -= 2) {
            type = static_cast<OperandType>(types & (0b11 << i));
            if(type == OperandType::OmittedOperand) {
                data.operandCount = data.operandTypes.count();
                break;
            }
            data.operandTypes.append(type);
        }
    }

    return data;
}

} // namespace ZMachineCore
