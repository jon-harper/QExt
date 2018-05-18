#include "dptr.h"

namespace qe {
//! Constructs a new object with \a qq as the back pointer (q-ptr).
QE_CORE_EXPORT PrivateBase::PrivateBase(PublicBase *qq)
    : qe_ptr(qq)
{
}


//! Constructs a new object with \a dd as the source for the d-ptr.
QE_CORE_EXPORT PublicBase::PublicBase(PrivateBase &dd)
    : qed_ptr(&dd)
{
}

} // namespace qe

/*!
    \def QE_DECLARE_PRIVATE(Classname)
    \relates qe::PrivateBase
    \brief Enables a class to use d_func() pointers.

    Prepares \a Classname to utilize the functionality of qe::PrivateBase.

    \warning This declaration must be in the private section of your class.

    \note You must implement a qe::PrivateBase-derived class that follows the naming
    scheme YourClassNamePrivate. Example:

    \code
    CREATE EXAMPLE HERE
    \endcode

    \note This does not give access to Qt's private classes. Instead,
    it creates a pointer to a qe::PrivateBase class and allows using the QE_Q and QE_D macros.

    \sa qe::PrivateBase
*/

/*!
    \def QE_DECLARE_PUBLIC(Classname)
    \relates qe::PrivateBase
    \brief Declares a derived class of qe::PrivateBase for use with `Classname`.

    To use this macro, your private class must:
     - Follow the naming convention of `[Classname]Private`.
     - Inherit from qe::PrivateBase, directly or indirectly.

    Classes with this declaration can use `Q_Q`, \ref QE_DPTR, and `q_func()` to
    get a pointer to their associated public class.

    \sa qe::PublicBase, QE_DECLARE_PRIVATE
*/

/*!
    \def QE_QPTR
    \relates qe::PrivateBase
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
    \relates qe::PrivateBase
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
\relates qe::PublicBase
\brief This macro retrieves the d-ptr for a class deriving from qe::PublicBase
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
\relates qe::PublicBase
\brief This macro retrieves the a `const` d-ptr for a class deriving from qe::PublicBase
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
