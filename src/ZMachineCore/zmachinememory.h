#ifndef ZMACHINEMEMORY_H
#define ZMACHINEMEMORY_H

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>

namespace ZMachineCore {

enum MemoryOperationStatus {
    OK,
    OutOfBounds,
    MemoryNotInitialized,
    WriteToReadOnlyMemory,
    UnknownError
};

class ZMachineMemory
{
public:
    ZMachineMemory();
    void populate(QByteArray bytes);
    qint32 memorySize();

    template<typename T>
    T getInt(quint16 addr)
    {
        assertIntType<T>();
        T val;
        qsizetype valSize = sizeof(T);
        if(m_bytes.length() == 0) {
            m_operationStatus = MemoryOperationStatus::MemoryNotInitialized;
        } else if (addr + valSize > m_bytes.length()) {
            m_operationStatus = MemoryOperationStatus::OutOfBounds;
        } else if (m_buffer.seek(addr)) {
            m_stream >> val;
            m_operationStatus = MemoryOperationStatus::OK;
            return val;
        } else {
            m_operationStatus = MemoryOperationStatus::UnknownError;
        }
        return 0;
    }

    template<typename T>
    void setInt(quint16 addr, T val)
    {
        assertIntType<T>();
        m_buffer.seek(addr);
        m_stream << val;
    }

    MemoryOperationStatus lastMemoryOperationStatus();

protected:
    MemoryOperationStatus m_operationStatus;

private:
    template<typename T>
    static void assertIntType()
    {
        static_assert(std::is_same_v<T, quint8> || std::is_same_v<T, quint16>
                          || std::is_same_v<T, qint8> || std::is_same_v<T, quint16>,
                      "T must be quint8, quint16, qint8, or qint16");
    }
    QByteArray m_bytes;
    QBuffer m_buffer;
    QDataStream m_stream;
};

} // namespace ZMachineCore

#endif // ZMACHINEMEMORY_H
