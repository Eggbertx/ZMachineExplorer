QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
	tst_zmachinecore.cpp

INCLUDEPATH += \
	$$PWD/../ZMachineCore

LIBS += -L$$OUT_PWD/../ZMachineCore/lib -lZMachineCore

QMAKE_POST_LINK += cp -r $$PWD/testdata/ $$OUT_PWD/
