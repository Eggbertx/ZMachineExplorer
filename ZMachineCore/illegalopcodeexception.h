#ifndef ILLEGALOPCODEEXCEPTION_H
#define ILLEGALOPCODEEXCEPTION_H

#include <QException>

namespace ZMachineCore {

class IllegalOpcodeException : public QException
{
public:
    IllegalOpcodeException(quint8 opcode,
                           QString message,
                           quint8 vmVersion,
                           quint8 minVersion = 1,
                           quint8 maxVersion = 0);
    IllegalOpcodeException(quint8 opcode,
                           quint8 vmVersion,
                           quint8 minVersion = 1,
                           quint8 maxVersion = 0);

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

#endif // ILLEGALOPCODEEXCEPTION_H
