#include "unhandledopcodeexception.h"

namespace ZMachineCore {

UnhandledOpcodeException::UnhandledOpcodeException(quint8 opcode, quint8 vmVersion, quint8 minVersion, quint8 maxVersion)
{
    m_opcode = opcode;
    m_vmVersion = vmVersion;
    m_minVersion = minVersion;
    m_maxVersion = maxVersion;
    m_message = QString::asprintf("Unimplemented opcode %X (vm version: %d", opcode, vmVersion);
    if(minVersion > 1) {
        m_message = m_message.append(QString::asprintf(", min: %d", minVersion));
        if(maxVersion > 0) {
            m_message = m_message.append(QString::asprintf(", max: %d)", maxVersion));
        } else {
            m_message = m_message.append(")");
        }
    } else if(maxVersion > 0) {
        m_message = m_message.append(QString::asprintf(", max: %d)", maxVersion));
    } else {
        m_message = m_message.append(")");
    }
}

quint8 UnhandledOpcodeException::vmVersion() const
{
    return m_vmVersion;
}

quint8 UnhandledOpcodeException::minVersion() const
{
    return m_minVersion;
}

quint8 UnhandledOpcodeException::maxVersion() const
{
    return m_maxVersion;
}

QString UnhandledOpcodeException::message() const
{
    return m_message;
}

} // namespace ZMachineCore
