#include <type_traits>
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

quint8 ZMachineMemory::zMachineVersion()
{
    return getInt<quint8>(0);
}

} // namespace ZMachineCore
