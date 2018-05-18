
QT += core gui widgets

TARGET = colorbutton_example
TEMPLATE = app
CONFIG += C++14

INCLUDEPATH += "../../Include"

HEADERS += \ 
	$$PWD/colorbutton.h \
	$$PWD/colorbutton_p.h

SOURCES += \ 
	$$PWD/colorbutton.cpp \ 
    $$PWD/main.cpp
	
win32 {
	#release build
	CONFIG(release, debug|release) {
        LIBS += -L$$OUT_PWD/../core/release -lqecore
	}
	#debug build
	CONFIG(debug, debug|release) {
        LIBS += -L$$OUT_PWD/../core/debug -lqecore
	}
}
