win32 {
QT -= gui

TARGET = windows_test
TEMPLATE = app
CONFIG += console C++14

INCLUDEPATH += ../../Include

SOURCES += \
    $$PWD/main.cpp

HEADERS += \
    $$PWD/test_unknownpointer.h

LIBS += -lOle32 -lShell32 -lShlwapi
}
