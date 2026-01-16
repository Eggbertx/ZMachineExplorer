#include <QTest>
#include <zmachinememory.h>

class ZMachineMemoryTests : public QObject
{
    Q_OBJECT

public:
    ZMachineMemoryTests();
    ~ZMachineMemoryTests();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_templateMethods();
};

ZMachineMemoryTests::ZMachineMemoryTests() {}

ZMachineMemoryTests::~ZMachineMemoryTests() {}

void ZMachineMemoryTests::initTestCase()
{
    // code to be executed before the first test function
}

void ZMachineMemoryTests::init()
{
    // code to be executed before each test function
}

void ZMachineMemoryTests::cleanupTestCase()
{
    // code to be executed after the last test function
}

void ZMachineMemoryTests::cleanup()
{
    // code to be executed after each test function
}

void ZMachineMemoryTests::test_templateMethods()
{
    // ZMachineCore::ZMachineVM vm;
    // QCOMPARE(vm.filePath(), "");
    // QVERIFY(vm.loadFromFile("testdata/test.z5"));
    // QCOMPARE(vm.filePath(), "testdata/test.z5");
    // QCOMPARE(vm.lastError(), "");
    // QCOMPARE(vm.fileSize(), 86528);
    // QCOMPARE(vm.zMachineVersion(), 5);
    ZMachineCore::ZMachineMemory mem;
    QByteArray arr = QByteArray::fromRawData("\xab\xcd\x01\x23\xab\xcd\x01\x23", 8);
    mem.populate(arr);
    QCOMPARE(mem.getInt<quint16>(1), 0xcd01);
}


QTEST_APPLESS_MAIN(ZMachineMemoryTests)

#include "tst_ZMachineMemoryTests.moc"
