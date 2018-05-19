
win32 {
QT += core gui widgets winextras

TARGET = qewin
TEMPLATE = lib
CONFIG += shared C++14

DEFINES += QE_BUILD_WINDOWS

HEADERS += \
    $$PWD/compointer.h \
    $$PWD/unknownpointer.h \
    $$PWD/global.h

SOURCES +=
INCLUDEPATH += "../../Include"

LIBS += -lOle32 -lShell32 -lShlwapi -lOleAut32 -luser32
}
