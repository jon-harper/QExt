QT += core

TARGET = qecore
TEMPLATE = lib
CONFIG += shared

DEFINES += QE_BUILD_CORE

HEADERS += \
	$$PWD/global.h \
	$$PWD/private.h \
	$$PWD/laptimer.h \
	$$PWD/debugutil.h \
	$$PWD/uniquepointer.h \
    $$PWD/pointer.h \
	$$PWD/dptr.h

SOURCES += \
    $$PWD/dptr.cpp \
	$$PWD/debugutil.cpp

INCLUDEPATH += "../../Include"
