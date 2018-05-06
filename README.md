# QExt
## Purpose
QExt is an add-on library for Qt developers with limited dependencies. The aims of QExt in a nutshell are:
 - Ease C++11/14 use with Qt
 - Limit "rolling-your-own"
 - Extend Qt's capabilities in common scenarios

## Goals
 - Modularity
 - Minimal dependencies
 - Seamless integration with Qt
 - Sensible use of C++11/14
 - Reasonable speed and memory effeciency
 - Excellent documentation

## Features
QExt exposes and maintains functionality that is "private" in Qt. The canonical example of a private class later made public is QVarLengthArray. QExt re-implements and supports similar types of useful classes that are hidden in Qt or emulates their functionality.

QExt also makes Qt-style development easier, such as the use of the d-ptr (also known as the compiler firewall), including Private class inheritance and back links (see `QePublicBase` and `QePrivateBase` in QeCore).

C++14 is now supported by VS, gcc, and Clang, so it is used throughout the library to speed, ease, and simplify development. One of the benefits of this are macros that replace `Q_D` and `Q_Q` without needing a class name declaration (`QE_D` and `Q_EQ`). This is legal code in QExt:

    void MyPublicClass::setFoo(const Foo &f)
    {
       //Q_D(MyPublicClass); <--Not needed
       QE_D; //This works instead, along with QE_DPTR
       d->foo = f;
    }
	
	bool MyPublicClass::hasFoo() const
	{
		//Q_D(const MyPublicClass); <--nope
		QE_CONST_D; //QE_CD is an option for brevity
		return d->foo;
	
	}

Eventual plans for QExt include a library of useful widgets (QeWidgets), a ribbon toolkit (QeRibbon), and Windows/COM tools (QeWindows).

## Status
QExt is currently highly unstable (pre-alpha) and subject to change, including API changes and wholesale renaming/removal/replacement of classes and functions.

### QeCore
- Public and private base classes: done
- D-ptr and q-ptr, `auto` semantics: done
- `QeUniquePointer`: nearly complete, needs code review

### QeWidgets
- Some widgets need cleanup, polish, and documentation

### QeRibbon
- Design decisions need to be made regarding the limitations of QStyle
- Some basic classes complete
- Emulation of Windows `SplitButton` control needs rewrite
- `QeRibbonGroupLayout` needs implementation 

### QeWindows
- QeUnknown Pointer: Nearly complete
- Numerous low-level functions done
- COM smart pointers need to be used everywhere 
- Need test classes

## License
QExt is released under the GNU General Public License Version 3.

## Contributing
This is a hobby project, so development is probably going to be slow. 

### Style
QExt is written in the Qt development style, which is (circularly) dependant upon the KDE style. Deviations will be noted here if/when they occur.
