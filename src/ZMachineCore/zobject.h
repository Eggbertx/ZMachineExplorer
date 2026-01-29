#ifndef ZOBJECT_H
#define ZOBJECT_H
#include <QString>

namespace ZMachineCore {

typedef struct
{
    quint8 flags[6];
    quint16 parent;
    quint16 sibling;
    quint16 child;
    quint16 properties;
} zobject_header;

class ZObject
{
public:
    ZObject(zobject_header header, quint8 version);
    bool getAttribute(quint8 f);
    void setAttribute(quint8 f, bool v);
    quint16 parentID() { return m_header.parent; }
    quint16 siblingID() { return m_header.sibling; }
    quint16 childID() { return m_header.child; }
    quint16 propertiesAddress() { return m_header.properties; }

private:
    bool flagOutOfBounds(quint8 index) { return (m_version < 4 && index >= 4) || index >= 6; }
    zobject_header m_header;
    quint8 m_version;
};

} // namespace ZMachineCore

#endif // ZOBJECT_H
