#include "zmachinememory.h"

namespace ZMachineCore {

ZMachineMemory::ZMachineMemory()
{
    m_stream.setByteOrder(QDataStream::BigEndian);
    m_buffer.setBuffer(&m_bytes);
    m_stream.setDevice(&m_buffer);
}

void ZMachineMemory::populate(QByteArray bytes)
{
    m_buffer.setData(bytes);
    m_buffer.open(QIODevice::ReadWrite);
    m_buffer.seek(0);
}

qint32 ZMachineMemory::memorySize()
{
    return m_bytes.size();
}

MemoryOperationStatus ZMachineMemory::lastMemoryOperationStatus()
{
    return m_operationStatus;
}

bool ZMachineMemory::seekAndValidateBounds(quint16 addr, qsizetype valSize)
{
    if(m_bytes.length() == 0) {
        m_operationStatus = MemoryOperationStatus::MemoryNotInitialized;
    } else if (addr + valSize > m_bytes.length()) {
        m_operationStatus = MemoryOperationStatus::OutOfBounds;
    } else if (m_buffer.seek(addr)) {
        m_operationStatus = MemoryOperationStatus::OK;
        return true;
    } else {
        m_operationStatus = MemoryOperationStatus::UnknownError;
    }
    return false;
}

} // namespace ZMachineCore
