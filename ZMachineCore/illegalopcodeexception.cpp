#include "illegalopcodeexception.h"

namespace ZMachineCore {

IllegalOpcodeException::IllegalOpcodeException(
    quint8 opcode, QString message, quint8 vmVersion, quint8 minVersion, quint8 maxVersion)
{
    m_opcode = opcode;
    m_vmVersion = vmVersion;
    m_minVersion = minVersion;
    m_maxVersion = maxVersion;
    if (!message.isEmpty()) {
        m_message = message;
    } else {
        m_message = QString::asprintf("Unimplemented opcode %X (vm version: %d", opcode, vmVersion);
        if (minVersion > 1) {
            m_message = m_message.append(QString::asprintf(", min: %d", minVersion));
            if (maxVersion > 0) {
                m_message = m_message.append(QString::asprintf(", max: %d)", maxVersion));
            } else {
                m_message = m_message.append(")");
            }
        } else if (maxVersion > 0) {
            m_message = m_message.append(QString::asprintf(", max: %d)", maxVersion));
        } else {
            m_message = m_message.append(")");
        }
    }
}

IllegalOpcodeException::IllegalOpcodeException(quint8 opcode, quint8 vmVersion, quint8 minVersion, quint8 maxVersion)
    : IllegalOpcodeException(opcode, "", vmVersion, minVersion, maxVersion)
{}

quint8 IllegalOpcodeException::vmVersion() const
{
    return m_vmVersion;
}

quint8 IllegalOpcodeException::minVersion() const
{
    return m_minVersion;
}

quint8 IllegalOpcodeException::maxVersion() const
{
    return m_maxVersion;
}

QString IllegalOpcodeException::message() const
{
    return m_message;
}

} // namespace ZMachineCore
