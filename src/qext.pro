DEFINES += QEXT_BUILD_LIB

TEMPLATE = subdirs
SUBDIRS = core

win32 {
SUBDIRS += windows
}
