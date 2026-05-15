#ifndef ZMACHINEVM_H
#define ZMACHINEVM_H

#include <QHash>
#include <QString>
#include <QStack>

#include "enums.h"
#include "zmachinememory.h"
#include "zobject.h"

#define REGISTER_INSTRUCTION(opcode, method, minVersion, maxVersion) \
    m_instructions[opcode].append({ \
        [this](ZMachineVM& vm, const QList<quint16>& args) { method(args); }, \
        minVersion, \
        maxVersion \
    })

#define STACK_SIZE 1024

namespace ZMachineCore {

class ZMachineVM : public ZMachineMemory
{
public:
    ZMachineVM();

    bool loadFromFile(const QString &filePath);
    void reset();

    QString &lastError();
    QString &filePath();
    quint32 fileSize();

    enum InterpreterNum interpreterNumber();
    void setInterpreterNum(enum InterpreterNum, bool setInFile = true, bool isReset = false);
    quint8 zMachineVersion();
    quint16 pc() { return m_PC; }
    void setPC(quint16 newPC) { m_PC = newPC; }

    template<typename T>
    void setInt(quint16 addr, T val, enum MemoryWriteSource source)
    {
        assertIntType<T>();
        if (validateMemoryWrite(addr, source)) {
            ZMachineMemory::setInt<T>(addr, val);
        } else {
            m_operationStatus = MemoryOperationStatus::WriteToReadOnlyMemory;
        }
    }

    QList<zobject_header> &getObjectList();
    int numObjects() { return m_objectList.length(); }

private:
    using InstructionImplFunc = std::function<void(ZMachineVM&, const QList<quint16>&)>;

    bool validateMemoryWrite(quint16 addr, enum MemoryWriteSource source);
    bool validateHeaderWrite(quint16 addr, enum MemoryWriteSource source);
    enum MemoryRegionType memoryRegion(quint16 addr);

    QList<quint16> m_defaultProperties;
    QList<zobject_header> m_objectList;
    QString m_lastError;
    QString m_filePath;
    enum InterpreterNum m_interpreterNum = InterpreterNum::IBMPC;

    struct InstructionEntry {
        InstructionImplFunc fn;
        int minVersion;
        int maxVersion;
    };

    quint16 m_PC = 0;
    QHash<quint8, QList<InstructionEntry>> m_instructions; // stored as opcode -> [list of implementations, in ascending order by minimum version]
    void defineInstructions();
    void executeInstruction(quint8 opcode, const QList<quint16>& args, int version);
    void executeInstruction();

    // Instruction implementations
    // V1
    void implRTrue_V1(const QList<quint16>& args);
    void implRFalse_V1(const QList<quint16>& args);
    void implPrintRet_V1(const QList<quint16>& args);
    void implQuit_V1(const QList<quint16>& args);
    // V2
    // V3
    // V4
    void implCallVS_V4(const QList<quint16>& args);
    // V5
    // V6
};

} // namespace ZMachineCore

#endif // ZMACHINEVM_H
