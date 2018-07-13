
QT += core

CONFIG += c++1z

HEADERS += \
	$$PWD/global.h \
	$$PWD/debugutil.h \
	$$PWD/uniquepointer.h \
    $$PWD/type_util.h \
	$$PWD/dptr.h \
    $$PWD/managedpointer.h \
    $$PWD/pointer_deleters.h

SOURCES += \
    $$PWD/dptr.cpp \
	$$PWD/debugutil.cpp
