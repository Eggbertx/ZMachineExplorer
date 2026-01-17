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
    setInt<quint8>(HeaderAddress::InterpreterNum, m_interpreterNum);
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

void ZMachineVM::setInterpreterNum(enum InterpreterNum num, bool setInFile)
{
    m_interpreterNum = num;
    if (setInFile && memorySize() > 0) {
        setInt<quint8>(HeaderAddress::InterpreterNum, num);
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

} // namespace ZMachineCore
