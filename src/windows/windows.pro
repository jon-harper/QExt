
win32 {
QT += core gui widgets winextras

TARGET = qewin
TEMPLATE = lib
CONFIG += shared C++14

DEFINES += QE_BUILD_WINDOWS

HEADERS += \
    $$PWD/compointer.h \
    $$PWD/unknownpointer.h \
    $$PWD/global.h \
    $$PWD/shellutil.h \
    $$PWD/winutil.h \
#    $$PWD/shellitem.h \
#    $$PWD/shellitemdata.h \
#    $$PWD/shellitemflags.h \
    $$PWD/types.h

SOURCES += \
    $$PWD/shellutil.cpp \
    $$PWD/winutil.cpp \
#    $$PWD/shellitem.cpp \
#    $$PWD/shellitemdata.cpp

SOURCES +=
INCLUDEPATH += "../../Include"

LIBS += -lOle32 -lShell32 -lShlwapi -lOleAut32 -luser32
} #win32
