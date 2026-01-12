#include <QTest>
#include <zmachinevm.h>

class ParseStoryFileHeader : public QObject
{
    Q_OBJECT

public:
    ParseStoryFileHeader();
    ~ParseStoryFileHeader();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_case1();

};

ParseStoryFileHeader::ParseStoryFileHeader() {}

ParseStoryFileHeader::~ParseStoryFileHeader() {}

void ParseStoryFileHeader::initTestCase()
{
    // code to be executed before the first test function
}

void ParseStoryFileHeader::init()
{
    // code to be executed before each test function
}

void ParseStoryFileHeader::cleanupTestCase()
{
    // code to be executed after the last test function
}

void ParseStoryFileHeader::cleanup()
{
    // code to be executed after each test function
}

void ParseStoryFileHeader::test_case1() {
    ZMachineCore::ZMachineVM vm;
    QCOMPARE(vm.filePath(), "");
    QVERIFY(vm.loadFromFile("testdata/test.z5"));
    QCOMPARE(vm.filePath(), "testdata/test.z5");
    QCOMPARE(vm.lastError(), "");
    QCOMPARE(vm.fileSize(), 86528);
    QCOMPARE(vm.zMachineVersion(), 5);
}

QTEST_APPLESS_MAIN(ParseStoryFileHeader)

#include "tst_parsestoryfileheader.moc"
