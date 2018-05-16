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
QExt makes Qt-style development easier, such as support for the d-ptr (also known as the compiler firewall), including Private class inheritance and back links (see `qe::PublicBase` and `qe::PrivateBase` in the core module).

C++14 is used throughout the library to speed, ease, and simplify development. One of the benefits of this are macros that replace `Q_D` and `Q_Q` without needing a class name declaration (`QE_D` and `QE_Q`). This is legal code in QExt:

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

Another feature of QExt is the "moveable QScopedPointer", implemented as qe::UniquePointer. It's essentially QScopedPointer with move semantics and uses Qt's own QScopedPointerDeleter by default, like QScopedPointer. However, it was designed with modern C++ in mind and written from the ground up with both Standard Library-style and Qt-style accessors (e.g. `get()` and `data()` are synonymous).
	
Eventual plans for QExt include a library of useful widgets (QeWidgets) and Windows/COM tools (QeWindows).

## Status
QExt is currently unstable (pre-alpha) and subject to change, including API changes and wholesale renaming/removal/replacement of classes and functions.

### QeCore
- Public and private base classes: done
- D-ptr and q-ptr, `auto` semantics: done
- `qe::UniquePointer`: nearly complete, needs code review

### QeWidgets
- Widgets need cleanup, tests, and documentation

### QeWindows
- `qe::windows::UnknownPointer`: nearly complete, needs code review
- Numerous low-level functions done
- COM smart pointers need to be used everywhere 
- Need test classes

## License
QExt is released under the GNU General Public License Version 3.

## Contributing
This is a hobby project, so development is probably going to be slow. 

### Style
QExt is written in the Qt development style, which is (circularly) dependant upon the KDE style.

Note that QExt supports namespaces, and is written within them. Qt-style type aliases are available with logical names (e.g. QeUniquePointer). Use the preprocessor defintion QEXT_NO_CLUTTER to disable type aliases.
