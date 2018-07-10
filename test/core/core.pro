
QT -= gui

TARGET = core_test
TEMPLATE = app
CONFIG += console c++1z

INCLUDEPATH += ../../Include

SOURCES += \
	$$PWD/main.cpp

HEADERS += \
    $$PWD/test_uniquepointer.h \
    $$PWD/test.h \
    $$PWD/test_managedpointer.h
