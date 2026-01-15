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

uint32_t ZMachineMemory::memorySize()
{
    return m_bytes.size();
}

uint8_t ZMachineMemory::zMachineVersion()
{
    return getByte(0);
}

uint8_t ZMachineMemory::getByte(uint16_t addr)
{
    uint8_t val;
    m_buffer.seek(addr);
    m_stream >> val;
    return val;
}

void ZMachineMemory::setByte(uint16_t addr, uint8_t val)
{
    m_buffer.seek(addr);
    m_stream << val;
}

} // namespace ZMachineCore
