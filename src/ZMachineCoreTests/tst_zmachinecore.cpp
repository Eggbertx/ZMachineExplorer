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

private:
    static constexpr char byteArrayStr[] = "\xab\xcd\x01\x23\xab\xcd\x01\x23";
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
    QVERIFY(vm.loadFromFile("testdata/test.z5"));
    QCOMPARE(vm.filePath(), "testdata/test.z5");
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



QTEST_APPLESS_MAIN(ZMachineCoreTests)

#include "tst_zmachinecore.moc"
