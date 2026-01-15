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
    uint32_t memorySize();
    uint8_t zMachineVersion();
    uint8_t getByte(uint16_t addr);
    void setByte(uint16_t addr, uint8_t val);
    uint16_t getWord(uint16_t addr);

private:
    // data storage and manipulation
    QByteArray m_bytes;
    QBuffer m_buffer;
    QDataStream m_stream;
};

} // namespace ZMachineCore

#endif // ZMACHINEMEMORY_H
