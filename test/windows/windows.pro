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
    $$PWD/test_shellnodeinfo.h

LIBS += -lOle32 -lShell32 -lShlwapi

#release build
CONFIG(release, debug|release) {
    LIBS += -L$$OUT_PWD/../windows/release -lqewin
    LIBS += -L$$OUT_PWD/../../build/windows/release -lqewin
}
#debug build
CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/../windows/debug -lqewin
    LIBS += -L$$OUT_PWD/../../build/windows/debug -lqewin
}
} #win32

HEADERS += \
    test_shellcache.h
