#include <QTest>
#include <zmachinememory.h>
#include <zmachinevm.h>

class ZMachineCoreTests : public QObject
{
    Q_OBJECT

public:
    ZMachineCoreTests();
    ~ZMachineCoreTests();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_parseHeader();
    void test_ZMachineMemory_getInt();
    void test_ZMachineMemory_setInt();
    void test_ZMachineMemory_operationStatus();
    void test_ZObject_getFlag();
    void test_ZObject_setFlag();

private:
    static constexpr char byteArrayStr[] = "\xab\xcd\x01\x23\xab\xcd\x01\x23";
    ZMachineCore::zobject_header header;
};

ZMachineCoreTests::ZMachineCoreTests() {}

ZMachineCoreTests::~ZMachineCoreTests() {}

void ZMachineCoreTests::initTestCase()
{
    // code to be executed before the first test function
}

void ZMachineCoreTests::init()
{
    // code to be executed before each test function
    header.flags[0] = 0x80;
    header.flags[1] = 0x80;
    header.flags[2] = 0;
    header.flags[3] = 0;
    header.flags[4] = 0;
    header.flags[5] = 0x80;
    header.parent = 0;
    header.sibling = 0;
    header.child = 0;
    header.properties = 0;
}

void ZMachineCoreTests::cleanupTestCase()
{
    // code to be executed after the last test function
}

void ZMachineCoreTests::cleanup()
{
    // code to be executed after each test function
}

void ZMachineCoreTests::test_parseHeader()
{
    ZMachineCore::ZMachineVM vm;
    QCOMPARE(vm.filePath(), "");
    QVERIFY(vm.loadFromFile(":/testdata/test.z5"));
    QCOMPARE(vm.filePath(), ":/testdata/test.z5");
    QCOMPARE(vm.lastError(), "");
    QCOMPARE(vm.fileSize(), 86528);
    QCOMPARE(vm.zMachineVersion(), 5);
}

void ZMachineCoreTests::test_ZMachineMemory_getInt()
{
    ZMachineCore::ZMachineMemory mem;
    QByteArray arr = QByteArray::fromRawData(byteArrayStr, 8);
    mem.populate(arr);
    QCOMPARE(mem.getInt<quint16>(1), 0xcd01);
    QCOMPARE(mem.getInt<qint8>(1), -51);
}

void ZMachineCoreTests::test_ZMachineMemory_setInt()
{
    ZMachineCore::ZMachineMemory mem;
    QByteArray arr = QByteArray::fromRawData(byteArrayStr, 8);
    mem.populate(arr);

    mem.setInt<quint16>(0, 0xfdfd);
    QCOMPARE(mem.getInt<quint16>(0), 0xfdfd);
}

void ZMachineCoreTests::test_ZMachineMemory_operationStatus()
{
    ZMachineCore::ZMachineMemory mem;
    QByteArray arr = QByteArray::fromRawData("\xfd\xfd", 2);
    mem.populate(arr);

    mem.getInt<quint16>(0);
    QCOMPARE(mem.lastMemoryOperationStatus(), ZMachineCore::MemoryOperationStatus::OK);
    mem.getInt<quint16>(1);
    QCOMPARE(mem.lastMemoryOperationStatus(), ZMachineCore::MemoryOperationStatus::OutOfBounds);
    mem.getInt<quint16>(2);
    QCOMPARE(mem.lastMemoryOperationStatus(), ZMachineCore::MemoryOperationStatus::OutOfBounds);
}

void ZMachineCoreTests::test_ZObject_getFlag()
{
    using namespace ZMachineCore;

    // version 1-3 tests, 4-byte attribute flags
    ZObject obj(header, 3);
    QVERIFY(obj.getAttribute(0));
    QVERIFY(!obj.getAttribute(1));
    QVERIFY(!obj.getAttribute(2));
    QVERIFY(obj.getAttribute(8));
    QVERIFY(!obj.getAttribute(40));

    // version 4+ tests, 6-byte attribute flags
    ZObject obj2(header, 6);
    QVERIFY(obj2.getAttribute(40)); // using the same attribues, it should have access to flags[5]
}

void ZMachineCoreTests::test_ZObject_setFlag()
{
    using namespace ZMachineCore;
    ZObject obj(header, 3);
    QVERIFY(obj.getAttribute(0));
    obj.setAttribute(0, false);
    QVERIFY(!obj.getAttribute(0));
    obj.setAttribute(27, true);
    QVERIFY(obj.getAttribute(27));
    obj.setAttribute(27, false);
    QVERIFY(!obj.getAttribute(27));
}

QTEST_APPLESS_MAIN(ZMachineCoreTests)

#include "tst_zmachinecore.moc"
