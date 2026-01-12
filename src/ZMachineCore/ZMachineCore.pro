# CONFIG -= qt
QT += core

TEMPLATE = lib
DEFINES += ZMACHINECORE_LIBRARY

CONFIG += c++17 shared

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    zmachinememory.cpp \
    zmachinevm.cpp

HEADERS += \
    ZMachineCore_global.h \
    zmachinememory.h \
    zmachinevm.h

DESTDIR = $$OUT_PWD/lib

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
