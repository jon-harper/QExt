
win32 {
QT += core gui widgets winextras

TARGET = qewin
TEMPLATE = lib
CONFIG += shared C++14

DEFINES += QE_BUILD_WINDOWS

HEADERS += \
    $$PWD/global.h \
    $$PWD/types.h \
    $$PWD/compointer.h \
    $$PWD/unknownpointer.h \
    $$PWD/winutil.h \
    $$PWD/shell.h \
    $$PWD/shellnode.h \
    $$PWD/shellnodedata.h \
    $$PWD/shellnodeinfo.h

SOURCES += \
    $$PWD/winutil.cpp \
    $$PWD/shell.cpp \
    $$PWD/shellnode.cpp \
    $$PWD/shellnodedata.cpp \
    $$PWD/shellnodeinfo.cpp

INCLUDEPATH += "../../Include"

LIBS += -lOle32 -lShell32 -lShlwapi -lOleAut32 -luser32
} #win32
