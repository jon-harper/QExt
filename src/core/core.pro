QT += core

TARGET = qecore
TEMPLATE = lib
CONFIG += shared C++14

DEFINES += QE_BUILD_CORE

HEADERS += \
	$$PWD/global.h \
	$$PWD/debugutil.h \
	$$PWD/uniquepointer.h \
    $$PWD/type_util.h \
	$$PWD/dptr.h

SOURCES += \
    $$PWD/dptr.cpp \
	$$PWD/debugutil.cpp

INCLUDEPATH += "../../Include"
