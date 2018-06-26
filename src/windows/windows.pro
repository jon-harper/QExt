
win32 {
QT += core gui winextras

TARGET = qewin
TEMPLATE = lib
CONFIG += shared C++14

DEFINES += QE_BUILD_WINDOWS

HEADERS += \
    $$PWD/global.h \
    $$PWD/types.h \
    $$PWD/unaligned.h \
    $$PWD/compointer.h \
    $$PWD/unknownpointer.h \
    $$PWD/winutil.h \
    $$PWD/shell.h \
    $$PWD/shellnode.h \
    $$PWD/shellnodedata.h \
    $$PWD/shellnodeinfo.h \
    $$PWD/shell_impl.h \
    $$PWD/idlist.h \
    $$PWD/idlist_impl.h

SOURCES += \
    $$PWD/winutil.cpp \
    $$PWD/shell.cpp \
    $$PWD/shellnode.cpp \
    $$PWD/shellnodedata.cpp \
    $$PWD/shellnodeinfo.cpp

INCLUDEPATH += "../../Include"

LIBS += -lOle32 -lShell32 -lShlwapi

#release build
CONFIG(release, debug|release) {
    LIBS += -L$$OUT_PWD/../core/release -lqecore
}
#debug build
CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/../core/debug -lqecore
}
} #win32
