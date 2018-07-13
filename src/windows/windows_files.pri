win32 {

INCLUDEPATH += "../../Include"

LIBS += -lOle32 -lShell32 -lShlwapi

HEADERS += \
    $$PWD/global.h \
    $$PWD/types.h \
    $$PWD/unaligned.h \
    $$PWD/compointer.h \
    $$PWD/unknownpointer.h \
    $$PWD/winutil.h \
    $$PWD/shell.h \
    $$PWD/shellnode.h \
    $$PWD/shellnodedata.h \
    $$PWD/shellnodeinfo.h \
    $$PWD/shell_impl.h \
    $$PWD/idlist.h \
    $$PWD/idlist_impl.h

SOURCES += \
    $$PWD/winutil.cpp \
    $$PWD/shell.cpp \
    $$PWD/shellnode.cpp \
    $$PWD/shellnodedata.cpp \
    $$PWD/shellnodeinfo.cpp

} #win32
