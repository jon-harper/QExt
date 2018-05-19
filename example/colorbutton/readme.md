# QeColorButton Example

## Overview

This is a contrived example to demonstrate using QExt's public/private class
framework, which is based on Qt's. If you already inherit from Qt's private
classes in your own projects, there are macros for Qt that are equivalent to
the `auto`-using macros that QExt has (see core/dptr.h).

QeColorButton displays a color via a swatch and configurable text (by default,
it displays the RGB values of the color). Clicking on the button opens a
QColorDialog. Selecting a different color then updates the button. It's a very
simple class to demonstrate how to inherit from and use qe::PublicBase and
qe::PrivateBase in your own projects.

## colorbutton.h

QeColorButton *looks* like a basic developer's Qt-widget-inheriting class
definition, with a few changes, making the header file look more like a
Qt header file than that of a user.

* First, we forward declare [Classname]Private before the class, in this case:
        ```
        class QeColorButtonPrivate;
        ```
* In the next line, we *also* inherit from `qe::PublicBase`, but do so **after**
inheriting from `QToolButton`. This is necessary any time you inherit from
multiple classes, one of which derives from `QObject`.
* Much of the header looks normal, with property declarations, constructor,
getters and setters for the various properties, and a signal declaration.
* The class ends with the declaration of a protected constructor:
        ```
        QeColorButton(QeColorButtonPrivate &dd, QWidget *parent = nullptr);
        ```
note that this takes a reference to the private class instance, not a pointer.
This ensures that we don't get a nullptr. See the .cpp section for how this is
done.
* The class declaration ends with the usual `Q_DISABLE_COPY` and a variation
of `Q_DECLARE_PRIVATE`: `QE_DECLARE_PRIVATE`. It's entirely possible to use
Qt's macros, but QExt re-implemented them with different, non-conflicting
variable names to avoid dependancy on Qt in the core files. The argument given
to the macro must be the class's name *exactly* or you'll get compile errors.

## colorbutton_p.h

This private header file could potentially be placed at the head of the .cpp
file, but for purposes of demonstration, it has its own header. The header
makes it possible for another class to inherit from QeColorButton and also
derive from and use QeColorButtonPrivate.

* Notice that colorbutton.h is included. This is required to make the macro for
`QE_DECLARE_PUBLIC` work.
* The constructor takes a pointer (not a reference, as in the PublicBase class)
to the public class.
* The destructor is virtual.
* `updateIcon()` and `updateText()` are virtual functions to allow overriding
the base behavior of the class in inheriting classes without making public-facing
functions virtual. This is a take on "protected virtual" functions.

## colorbutton.cpp

### The init() function

During construction of a public class (QeColorButton), an unknown number of
constructors may be called and a simiarly unknown number of member variables
may be initialized. At the time of its construction, the private class
(QeColorButtonPrivate) receives a pointer *to an incompletely constructed class*.
It is therefore necessary to defer any initialization that will directly or
indirectly call the public class; thus, we defer some of our initialization to
the `init()` function. Our constructor can then, after other initialization is
done, call `d->init()` and complete setting up.

In this case, we grab the q-ptr, via the `QE_QPTR` macro. We then do some other
initialization, all of which uses the q-ptr and couldn't be done earlier.

### The public constructors

Note the similarity in the constructor initializers: they all delegate to one
other constructor, the protected one. This is because any inheriting class
that also accesses the d-ptr (inherits from QeColorButtonPrivate, etc.) will
*have* to call this constructor to pass, making it an excellent funnel for
our constructors towards calling `d->init()`.

### The public getters and setters

These serve mostly to illustrate using the auto d-ptr in practice. There is a
long form and a shorthand for both regular and `const` versions of the d-ptr:
* `QE_DPTR` or `QE_D` for the standard d-ptr
* `QE_CONST_DPTR` or `QE_CD` for the const d-ptr (necessary in functions declared
as `const`)

There are equivalent macros for getting the q-ptr, as well: `QE_QPTR`, `QE_Q`,
`QE_CONST_QPTR` and `QE_CQ`. They work similarly to the d-ptr versions, but
are used in the private class implementation.
