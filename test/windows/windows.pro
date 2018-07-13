win32 {
QT -= gui

TARGET = windows_test
TEMPLATE = app
CONFIG += console c++1z

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
    LIBS += -L$$OUT_PWD/release -lqext
    LIBS += -L$$OUT_PWD/../../build/release -lqext
}
#debug build
CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/debug/ -lqewin
    LIBS += -L$$OUT_PWD/../../build/debug -lqext
}
} #win32
