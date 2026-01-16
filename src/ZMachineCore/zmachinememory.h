#ifndef ZMACHINEMEMORY_H
#define ZMACHINEMEMORY_H

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>

namespace ZMachineCore {

enum HeaderAddress {
    Version,
    Flags1,
    HighMemoryBase = 0x4,
    InitialPC = 0x6,
    DictionaryAddr = 0x8,
    ObjectsAddr = 0xa,
    GlobalsAddr = 0xc,
    StaticAddr = 0xe,
    Flags2 = 0x10,
    AbbreviationsAddr = 0x18,
    FileLength = 0x1a,
    Checksum = 0x1c,
    InterpreterNum = 0x1e,
    InterpreterRev,
    HeightLines,
    WidthChars,
    WidthUnits,
    HeightUnits = 0x24,
    FontWidth = 0x26,
    FontHeight,
    RoutinesOffset,
    StringsOffset = 0x2a,
    DefaultBG = 0x2c,
    DefaultFG,
    TerminatingCharsTableAddr = 0x2e,
    Stream3Width = 0x30,
    StandardRevNum = 0x32,
    AlphabetTableAddr = 0x34,
    HeaderExtTableAddr = 0x36
};

enum Flag1Bits {
    ColorsAvailable,
    PictureDisplayAvailable,
    BoldFaceAvailable,
    ItalicAvailable,
    FixedSpaceStyleAvailable,
    SoundEffectsAvailable,
    TimedKeyInputAvailable = 7
};

enum Flag2Bits {
    TranscriptingEnabled,
    ForceFixedPitchPrinting,
    ScreenRedrawRequested,
    GameWantsPictures,
    GameWantsUndo,
    GameWantsMouse,
    GameWantsColors,
    GameWantsSFX,
    GameWantsMenus
};

enum InterpreterNum {
    Unknown,
    DECSystem20,
    AppleIIe,
    Macintosh,
    Amiga,
    AtariST,
    IBMPC,
    Commodore128,
    Commodore64,
    AppleIIc,
    AppleIIgs,
    TandyColor
};

class ZMachineMemory
{
public:
    ZMachineMemory();
    void populate(QByteArray bytes);
    qint32 memorySize();
    quint8 zMachineVersion();

    template<typename T>
    T getInt(quint16 addr)
    {
        assertIntType<T>();
        T val;
        m_buffer.seek(addr);
        m_stream >> val;
        return val;
    }

    template<typename T>
    void setInt(quint16 addr, T val)
    {
        assertIntType<T>();
        m_buffer.seek(addr);
        m_stream << val;
    }

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
