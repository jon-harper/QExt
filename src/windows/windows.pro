
win32 {
QT += core gui winextras

TARGET = qewin
TEMPLATE = lib
CONFIG += shared c++1z
DEFINES += QE_BUILD_WINDOWS

include(windows_files.pri)

} #win32
