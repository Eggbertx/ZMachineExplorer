#include "zmachinevm.h"
#include <QFile>

ZMachineCore::ZMachineVM::ZMachineVM()
{
    m_lastError = "";
    m_filePath = "";
}

bool ZMachineCore::ZMachineVM::loadFromFile(const QString &filePath)
{
    QFile storyFile(filePath);
    if (!storyFile.open(QIODevice::ReadOnly)) {
        m_lastError = storyFile.errorString();
        return false;
    }
    m_memory.populate(storyFile.readAll());
    storyFile.close();
    m_filePath = filePath;
    return true;
}

void ZMachineCore::ZMachineVM::reset()
{
    m_memory.setByte(HeaderAddress::InterpreterNum, m_interpreterNum);
}

QString &ZMachineCore::ZMachineVM::lastError()
{
    return m_lastError;
}

QString &ZMachineCore::ZMachineVM::filePath()
{
    return m_filePath;
}

uint32_t ZMachineCore::ZMachineVM::fileSize()
{
    return m_memory.memorySize();
}

void ZMachineCore::ZMachineVM::setInterpreterNum(enum InterpreterNum num, bool setInFile)
{
    m_interpreterNum = num;
    if (setInFile && m_memory.memorySize() > 0) {
        m_memory.setByte(HeaderAddress::InterpreterNum, num);
    }
}

enum ZMachineCore::InterpreterNum ZMachineCore::ZMachineVM::interpreterNumber()
{
    return m_interpreterNum;
}

uint8_t ZMachineCore::ZMachineVM::zMachineVersion()
{
    return m_memory.zMachineVersion();
}
