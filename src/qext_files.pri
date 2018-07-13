DEFINES += QEXT_NO_EXPORT
include (core/core.pri)

win32 {
include(windows/windows.pri)
}
