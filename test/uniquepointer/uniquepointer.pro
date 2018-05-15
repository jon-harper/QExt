
QT -= gui

TARGET = uniquepointer
TEMPLATE = app
CONFIG += console
CONFIG += C++14

SOURCES += \
	$$PWD/main.cpp \

INCLUDEPATH += ../../Include

HEADERS += \
    test_uniquepointer.h

