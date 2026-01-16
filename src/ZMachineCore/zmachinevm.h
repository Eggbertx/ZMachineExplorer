#ifndef ZMACHINEVM_H
#define ZMACHINEVM_H
#include <QString>

#include "zmachinememory.h"

namespace ZMachineCore {


class ZMachineVM
{
public:
    ZMachineVM();

    bool loadFromFile(const QString& filePath);
    void reset();

    QString& lastError();
    QString& filePath();
    quint32 fileSize();

    enum InterpreterNum interpreterNumber();
    void setInterpreterNum(enum InterpreterNum, bool setInFile = true);

    quint8 zMachineVersion();


private:
    ZMachineMemory m_memory;
    QString m_lastError;
    QString m_filePath;
    enum InterpreterNum m_interpreterNum = InterpreterNum::IBMPC;
};

}

#endif // ZMACHINEVM_H
