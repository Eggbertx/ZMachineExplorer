QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += ZME_VERSION=\\\"0.1\\\"

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    recentfilesmanager.cpp

HEADERS += \
    mainwindow.h \
    recentfilesmanager.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    $$PWD/../ZMachineCore


# Copy the ZMachineCore project output to this output directory
unix {
    QMAKE_PRE_LINK += cp $$OUT_PWD/../ZMachineCore/lib/libZMachineCore* $$OUT_PWD
}
win32 {
    QMAKE_PRE_LINK += copy $$OUT_PWD/../ZMachineCore/lib/libZMachineCore.dll $$OUT_PWD
}

LIBS += -L$$OUT_PWD -lZMachineCore -Wl,-rpath,$$OUT_PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
