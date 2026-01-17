#include "zmachinevm.h"
#include <QFile>

namespace ZMachineCore
{

ZMachineVM::ZMachineVM()
{
    m_lastError = "";
    m_filePath = "";
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
    return true;
}

void ZMachineVM::reset()
{
    setInt<quint8>(HeaderAddress::InterpreterNum, m_interpreterNum, MemoryWriteSource::ResetSource);
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
        setInt<quint8>(HeaderAddress::InterpreterNum, num, isReset?MemoryWriteSource::ResetSource:MemoryWriteSource::InterpreterSource);
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
    switch(regionType) {
    case MemoryRegionType::UnknownMemory:
        m_operationStatus = MemoryOperationStatus::MemoryNotInitialized;
        return false;
    case MemoryRegionType::DynamicMemory:
        if(addr <= HeaderAddress::HeaderExtTableAddr + 1) {
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
    switch(addr) {
    case HeaderAddress::Version:
    case HeaderAddress::Flags1:
    case HeaderAddress::Flags1+1:
    case HeaderAddress::Flags1+2:
    case HeaderAddress::HighMemoryBase:
    case HeaderAddress::HighMemoryBase+1:
    case HeaderAddress::InitialPC:
    case HeaderAddress::InitialPC+1:
    case HeaderAddress::DictionaryAddr:
    case HeaderAddress::DictionaryAddr+1:
    case HeaderAddress::ObjectsAddr:
    case HeaderAddress::ObjectsAddr+1:
    case HeaderAddress::GlobalsAddr:
    case HeaderAddress::GlobalsAddr+1:
    case HeaderAddress::StaticAddr:
    case HeaderAddress::StaticAddr+1:
    case HeaderAddress::Flags2:
    case HeaderAddress::Flags2+1:
    case HeaderAddress::Flags2+2:
    case HeaderAddress::Flags2+3:
    case HeaderAddress::Flags2+4:
    case HeaderAddress::Flags2+5:
    case HeaderAddress::Flags2+6:
    case HeaderAddress::Flags2+7:
    case HeaderAddress::AbbreviationsAddr:
    case HeaderAddress::AbbreviationsAddr+1:
    case HeaderAddress::FileLength:
    case HeaderAddress::FileLength+1:
    case HeaderAddress::Checksum:
    case HeaderAddress::Checksum+1:
    case HeaderAddress::RoutinesOffset:
    case HeaderAddress::RoutinesOffset+1:
    case HeaderAddress::StringsOffset:
    case HeaderAddress::StringsOffset+1:
    case HeaderAddress::TerminatingCharsTableAddr:
    case HeaderAddress::TerminatingCharsTableAddr+1:
    case HeaderAddress::AlphabetTableAddr:
    case HeaderAddress::AlphabetTableAddr+1:
    case HeaderAddress::HeaderExtTableAddr:
    case HeaderAddress::HeaderExtTableAddr+1:
        // all read-only addresses, except for Flags1 and Flags2, which must have flags set individually
        return false;
    case HeaderAddress::InterpreterNum:
    case HeaderAddress::InterpreterRev:
    case HeaderAddress::ScreenHeightLines:
    case HeaderAddress::ScreenWidthChars:
    case HeaderAddress::ScreenWidthUnits:
    case HeaderAddress::ScreenWidthUnits+1:
    case HeaderAddress::ScreenHeightUnits:
    case HeaderAddress::ScreenHeightUnits+1:
    case HeaderAddress::FontWidth:
    case HeaderAddress::FontHeight:
    case HeaderAddress::DefaultBG:
    case HeaderAddress::DefaultFG:
    case HeaderAddress::StandardRevNum:
        return source == MemoryWriteSource::InterpreterSource || source == MemoryWriteSource::ResetSource;
    case HeaderAddress::Stream3Width:
        return source == MemoryWriteSource::InterpreterSource;
    }
    return true;
}

MemoryRegionType ZMachineVM::memoryRegion(quint16 addr)
{
    if(memorySize() == 0) {
        return MemoryRegionType::UnknownMemory;
    }
    quint16 staticStart = ZMachineMemory::getInt<quint16>(HeaderAddress::StaticAddr);
    quint16 highStart = ZMachineMemory::getInt<quint16>(HeaderAddress::HighMemoryBase);
    if (addr < staticStart) {
        return MemoryRegionType::DynamicMemory;
    }
    if(addr >= staticStart && addr < highStart) {
        return MemoryRegionType::StaticMemory;
    }
    return MemoryRegionType::HighMemory;
}

} // namespace ZMachineCore
