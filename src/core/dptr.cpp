#include "dptr.h"

/*!
    \class QePrivateBase
    \ingroup module_core
    \brief The QePrivateBase class.
*/

/*!
    \fn explicit QePrivateBase::QePrivateBase(QObject *qq = 0)
    \brief Constructs an instance of QePrivateBase with \a qq as the public parent.
 */

/*!
    \fn virtual QePrivateBase::~QePrivateBase()
    \brief Default destructor for class.
 */

/*!
    \def QE_DECLARE_PRIVATE(Classname)
    \ingroup module_core
    \relates QePrivateBase
    \brief Enables a class to use d_func() pointers.

    Prepares \a Classname to utilize the functionality of QePrivateBase.

    \warning This declaration must be in the private section of your class.

    \note You must implement a QePrivateBase-derived class that follows the naming
    scheme YourClassNamePrivate. Example:

    \code
    CREATE EXAMPLE HERE
    \endcode

    \note This does not give access to Qt's private classes. Instead,
    it creates a pointer to a QBasePrivate class and allows using the QE_Q and QE_D macros.

    \sa QePrivateBase
*/

/*!
    \def QE_DECLARE_PUBLIC(Classname)
    \ingroup module_core
    \relates QePrivateBase
    \brief Declares a derived class of QePrivateBase for use with `Classname`.

    To use this macro, your private class must:
     - Follow the naming convention of `[Classname]Private`.
     - Inherit from QePrivateBase, directly or indirectly.

    Classes with this declaration can use `Q_Q`, \ref QE_DPTR, and `q_func()` to
    get a pointer to their associated public class.

    \sa QePublicBase, QE_DECLARE_PRIVATE
*/

/*!
    \def QE_QPTR
    \ingroup module_core
    \relates QePrivateBase
    \brief This macro retrieves the q-ptr.

    This macro does *not* require the class name be declared when used. This compares
    favorably against `Q_Q`:

        void MyClassPrivate::someFunc()
        {
            QE_QPTR;
            q->someOtherFunc();
        }

    \sa QE_CONST_QPTR, QE_DPTR
*/

/*!
    \def QE_CONST_QPTR
    \ingroup module_core
    \relates QePrivateBase
    \brief This macro retrieves the q-ptr as a `const` pointer.

    This macro does *not* require the class name be declared when used. This compares
    favorably against Q_Q:

        void MyClassPrivate::someFunc() const
        {
            QE_CONST_QPTR;
            q->someOtherConstFunc();
        }

    \sa QE_CONST_DPTR, QE_QPTR
*/

/*!
\def QE_DPTR
\ingroup module_core
\relates QePublicBase
\brief This macro retrieves the d-ptr for a class deriving from QePublicBase
*or* a native Qt private d-ptr.

This macro does *not* require the class name be declared when used. This compares
favorably against `Q_D`:

    void MyClass::someFunc()
    {
        QE_DPTR;
        d->someOtherFunc();
    }

Use QE_CONST_DPTR when you need a d-ptr for a `const` function.

\sa QE_CONST_DPTR, QE_QPTR
*/

/*!
\def QE_CONST_DPTR
\ingroup module_core
\relates QePublicBase
\brief This macro retrieves the a `const` d-ptr for a class deriving from QePublicBase
*or* a native Qt private d-ptr.

This macro does *not* require the class name be declared when used. This compares
favorably against `Q_D`:

    void MyClassPrivate::someFunc() const
    {
        QE_CONST_DPTR;
        d->someOtherConstFunc();
    }

\sa QE_CONST_QPTR, QE_DPTR
*/

//! Constructs a new object with \a qq as the back pointer (q-ptr).
QePrivateBase::QePrivateBase(QePublicBase *qq)
    : qe_ptr(qq)
{
}

//! Destroys the QePrivateBase instance.
QePrivateBase::~QePrivateBase()
{
}


/*!
    \class QePublicBase
    \ingroup module_core
    \brief The QePublicBase class.
*/

//! Constructs a new object with \a dd as the source for the d-ptr.
QePublicBase::QePublicBase(QePrivateBase &dd)
    : qed_ptr(&dd)
{
}

//! Destroys the QePublicBase instance.
QePublicBase::~QePublicBase()
{
}


