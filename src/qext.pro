DEFINES += QEXT_BUILD_LIB QE_BUILD_WINDOWS

TEMPLATE = lib
TARGET = qext
CONFIG += shared c++1z
QT += core gui widgets

INCLUDEPATH += "../Include"

include (core/core.pri)

win32 {
    QT += winextras
    include(windows/windows.pri)
}
