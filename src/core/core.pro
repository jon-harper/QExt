QT += core

TEMPLATE = lib
CONFIG += C++1z
#win32 {
#    QMAKE_CXXFLAGS += /std:c++17
#} else {
#    QMAKE_CXXFLAGS += -std=c++17
#}

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

INCLUDEPATH += "../../Include"
