win32 {
QT -= gui

TARGET = windows_test
TEMPLATE = app
CONFIG += console C++14

INCLUDEPATH += ../../Include

SOURCES += \
    $$PWD/main.cpp

HEADERS += \
    $$PWD/test_unknownpointer.h \
    $$PWD/test_shellnodeinfo.h \
    $$PWD/test_unaligned.h \
    $$PWD/test_shellidlist.h \
    $$PWD/test_shellnode.h

LIBS += -lOle32 -lShell32 -lShlwapi

#release build
CONFIG(release, debug|release) {
    LIBS += -L$$OUT_PWD/release -lqewin
    LIBS += -L$$OUT_PWD/../../build/windows/release -lqewin
}
#debug build
CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/debug/ -lqewin
    LIBS += -L$$OUT_PWD/../../build/windows/debug -lqewin
}
} #win32
