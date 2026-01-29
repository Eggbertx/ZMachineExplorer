#include "zobject.h"

namespace ZMachineCore {

ZObject::ZObject(zobject_header header, quint8 version)
{
    m_header = header;
    m_version = version;
}

bool ZObject::getAttribute(quint8 f)
{
    quint8 index = f / 8;
    if (flagOutOfBounds(index))
        return 0; // out of bounds
    quint8 mod = (f % 8) + 1;
    quint8 bit = m_header.flags[index] & (1 << (8 - mod));
    return bit > 0;
}

void ZObject::setAttribute(quint8 f, bool v)
{
    quint8 index = f / 8;
    if (flagOutOfBounds(index))
        return; // out of bounds
    quint8 mod = (f % 8) + 1;
    quint8 mask = (1 << (8 - mod));
    if (v) {
        m_header.flags[index] |= mask;
    } else {
        m_header.flags[index] &= (~mask);
    }
}
} // namespace ZMachineCore
