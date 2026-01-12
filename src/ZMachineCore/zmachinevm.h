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
    uint32_t fileSize();

    enum InterpreterNum interpreterNumber();
    void setInterpreterNum(enum InterpreterNum, bool setInFile = true);

    uint8_t zMachineVersion();


private:
    ZMachineMemory m_memory;
    QString m_lastError;
    QString m_filePath;
    enum InterpreterNum m_interpreterNum = InterpreterNum::IBMPC;
};

}

#endif // ZMACHINEVM_H
