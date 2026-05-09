#ifndef UNHANDLEDOPCODEEXCEPTION_H
#define UNHANDLEDOPCODEEXCEPTION_H

#include <QException>

namespace ZMachineCore {

class UnhandledOpcodeException : public QException
{
public:
    UnhandledOpcodeException(quint8 opcode, quint8 vmVersion, quint8 minVersion = 1, quint8 maxVersion = 0);

    QString message() const;
    quint8 vmVersion() const;
    quint8 minVersion() const;
    quint8 maxVersion() const;

private:
    QString m_message;
    quint8 m_opcode;
    quint8 m_vmVersion;
    quint8 m_minVersion;
    quint8 m_maxVersion;
};

} // namespace ZMachineCore

#endif // UNHANDLEDOPCODEEXCEPTION_H
