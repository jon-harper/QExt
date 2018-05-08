QT += core

TARGET = qecore
TEMPLATE = lib
CONFIG += shared
CONFIG += C++14

DEFINES += QE_BUILD_CORE

HEADERS += \
	$$PWD/global.h \
	$$PWD/debugutil.h \
	$$PWD/uniquepointer.h \
    $$PWD/pointer.h \
	$$PWD/dptr.h

SOURCES += \
    $$PWD/dptr.cpp \
	$$PWD/debugutil.cpp

INCLUDEPATH += "../../Include"