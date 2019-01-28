# QExt
##
NOTE: This project is currently inactive, as I'm working more with the standard library and sticking with Qt for UI work. Some work here may be re-used later in another project that will be actively maintained.

## Purpose
QExt is an add-on library for Qt developers with no other dependancies. The aims of QExt in a nutshell are:
 - Ease C++14/C++17 use with Qt
 - Limit "rolling-your-own"
 - Extend Qt's capabilities in common scenarios

## Features
QExt makes "Qt-style" pImpl development easier, such as support for Qt's d-ptr (also known as the compiler firewall), including Private class inheritance and back links (see `qe::PublicBase` and `qe::PrivateBase` in the core module).

### Modern C++
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

Another feature of QExt is a "moveable `QScopedPointer`", implemented as `qe::UniquePointer`. It's essentially `QScopedPointer` with move semantics and is compatible with `QScopedPointerDeleter`. However, it was designed with modern C++ in mind and written with both Standard Library-style and Qt-style accessors (e.g. `get()` and `data()` are synonymous).

### Windows and COM
`qe::windows::UnknownPointer<T>` is a header-only wrapper around `IUnknown` pointers. It will add your `->AddRef()`s and release your `->Release()`'s. It will copy and move intelligently. It also returns itself as `T**` via `ptr.addressOf()` and even as `void *` and `void **` with `ptr.pVoid()` and `ptr.ppVoid()`.

The `qe::windows::shell` namespace and member classes make handling PIDLs and `IShellItem`/`IShellFolder` interfaces much easier. `qe::windows::shell::IdList` is an efficient, header only wrapper around an `ITEMIDLIST` pointer. It provides platform-safe casting between the many PIDL typedefs, a `const_iterator`, and `noexcept` member functions that replace common IL* ShlObj.h functions. IdList will even guess at the type of data it represents based on the `abID[0]` value of the final element.

Eventual plans for QExt include a library of useful widgets (QeWidgets) and more Windows/COM tools in QeWindows.

## Status
QExt is currently beta in the QeCore module and unstable (pre-alpha) in the QeWindows module.

### QeCore
The core module is relatively stable. API changes are unlikely. Classes and functions may be added in the future, but in their own headers.

### QeWidgets
This module needs a lot of love. It has not been merged because of this.

### QeWindows
- `VARIANT` and `PROPVARIANT` to `QVariant` conversion functions are done except for &%$@ing SAFEARRAY values and unconvertible (to Qt) types, like `VT_CY`.
- Numerous low-level functions done
- shell::IdList is working and nearly complete
- ShellNode: needs a unit test
- ShellNodeInfo: missing lots of functionality at present

## License
QExt is released under the GNU General Public License Version 3.

## Contributing
This is a hobby project, so development may be slow. That said, this is GitHub. Branch (and fork) away.

### Coding Style
QExt is written in a form of the Qt development style, which is (circularly) dependant upon the KDE style. That is, classes are InitialCaps, functions are camelCased. 

Note that QExt supports namespaces, and is written within them. Qt-style type aliases are available with logical names. For example: `qecore/uniquepointer.h` contains the definition of `qe::UniquePointer` with an including `using` declaration for `QeUniquePointer`. Use the preprocessor defintion `QEXT_NO_CLUTTER` to disable these typedefs.

In QExt, a namespace will always be lower cased in order to distinguish namespaces from classes.
