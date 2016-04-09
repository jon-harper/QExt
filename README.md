# Qext
QExt is meant as an add-on to Qt without external dependencies, save for option modules for specific platforms (primarily Windows). Ultimately, the aim of QExt is to limit "rolling your own", expose useful Qt utility functions and classes, and leverage new C++11/14 features in a rational manner.

## Features
QExt exposes and maintains functionality (classes and functions) that are "private" in Qt. The canonical example of a private class made public is QVarLengthArray. QExt implements and supports similar types of useful classes that are hidden in Qt or emulates their functionality.

QExt also makes Qt-style development easier, such as the use of the d-ptr (also known as the compiler firewall), including private class inheritance. C++14 is now supported by VS, gcc, and Clang, so it is used throughout the library to speed, ease, and simplify development.

Optional components of QExt include a rich library of widgets (QeWidgets), a ribbon toolkit (QeRibbon), and Windows/COM integration (QeWindows).

## Status
QExt is currently highly unstable (pre-alpha) and subject to change, including API changes and removal.

## License
QExt is released under the LGPLv3.

## Contributing
We're not quite there yet. Let me put some code up first.

### Style
QExt is written in the Qt development style, which is (circularly) dependant upon the KDE style.
