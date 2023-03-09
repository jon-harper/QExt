/*  QExt: Extensions to Qt
 *  Copyright (C) 2016  Jonathan Harper
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QE_CORE_DPTR_H
#define QE_CORE_DPTR_H

#include "uniquepointer.h"
#include <qecore/global.h>

//! Use this as you would the Q_DECLARE_PUBLIC macro.
#define QE_DECLARE_PUBLIC(Classname) \
    inline Classname *qe_q_func() noexcept { return static_cast<Classname *>(qe_ptr); } \
    inline const Classname* qe_q_func() const noexcept { return static_cast<const Classname *>(qe_ptr); } \
    inline const Classname* qe_cq_func() const noexcept { return static_cast<const Classname *>(qe_ptr); } \
    friend class Classname;

//! Use this as you would the Q_DECLARE_PRIVATE macro.
#define QE_DECLARE_PRIVATE(Classname) \
    inline Classname##Private* qe_d_func() noexcept { return reinterpret_cast<Classname##Private *>(qed_ptr.data()); } \
    inline const Classname##Private* qe_d_func() const noexcept { return reinterpret_cast<const Classname##Private *>(qed_ptr.data()); } \
    inline const Classname##Private* qe_cd_func() const noexcept { return reinterpret_cast<const Classname##Private *>(qed_ptr.data()); } \
    friend class Classname##Private;

#define QE_DPTR         auto d = qe_d_func()  //! Retrieves the d-ptr for a non-`const` object.
#define QE_CONST_DPTR   auto d = qe_cd_func() //! Retrieves a `const` d-ptr; used in `const` functions.
#define QE_QPTR         auto q = qe_q_func()  //! Used in a PrivateBase-derived class to retrieve the back-pointer.
#define QE_CONST_QPTR   auto q = qe_cq_func() //! Used in a PrivateBase-derived class in a `const` member function to retrieve the back-pointer.

#define QE_D    QE_DPTR         //! Qt-style shorthand for QE_DPTR.
#define QE_CD   QE_CONST_DPTR   //! Qt-style shorthand for QE_CONST_DPTR.
#define QE_Q    QE_QPTR         //! Qt-style shorthand for QE_QPTR.
#define QE_CQ   QE_CONST_QPTR   //! Qt-style shorthand for QE_CONST_QPTR.

//! \cond
#ifndef QEXT_CORE_NO_QT
#include <qglobal.h>
#define Q_DPTR          auto d = d_func() 
#define Q_CONST_DPTR    const auto d = d_func()
#define Q_QPTR          auto q = q_func()
#define Q_CONST_QPTR    const auto q = q_func()
#endif
//! \endcond
namespace qe {

/*!
    \class PrivateBase
    \brief The PrivateBase class.
*/

class PublicBase;
class PrivateBase
{
public:
    //! Constructs a new `PrivateBase` instance with \a qq as the back-pointer.
    explicit PrivateBase(PublicBase *qq);

    //! Deleted copy constructor.
    PrivateBase(const PrivateBase &) = delete;

    //! Deleted move constructor.
    PrivateBase(PrivateBase &&) = delete;

    //! Deleted default constructor.
    PrivateBase() = delete;

    //! Virtual destructor to prevent slicing.
    inline virtual ~PrivateBase() {}

    //! Deleted copy assignment operator.
    void operator =(const PrivateBase &) = delete;
    //! Move constructor is deleted.
    void operator =(PrivateBase &&) = delete;
protected:
    PublicBase *qe_ptr;
};

/*!
    \brief The PublicBase class enables the use of pImpl, or the d-ptr.

    Inherit from this class for the public face of your class. If you also inherit from QObject,
    it must be inherited from first.

    \code
        class MyClass : public QObject, public qe::PublicBase
        {
            Q_OBJECT
            ...
        };
    \endcode

    You must also use the \ref QE_DECLARE_PUBLIC macro in the private section of your class.
*/
class PublicBase
{
public:
    //! The only functional constructor. Note that this takes a \em reference, i.e. it cannot be null.
    explicit PublicBase(PrivateBase &dd);

protected:
    UniquePointer<PrivateBase> qed_ptr;
};

//! Constructs a new object with \a qq as the back pointer (q-ptr).
inline PrivateBase::PrivateBase(PublicBase *qq)
    : qe_ptr(qq)
{
}


//! Constructs a new object with \a dd as the source for the d-ptr.
inline PublicBase::PublicBase(PrivateBase &dd)
    : qed_ptr(&dd)
{
}


} // namespace qe

#ifndef QEXT_NO_CLUTTER
using QePublicBase = qe::PublicBase;
using QePrivateBase = qe::PrivateBase;
#endif

#endif //QE_CORE_DPTR_H


