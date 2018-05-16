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

#include <QScopedPointer>

#include <qecore/global.h>

/* Use this as you would the Q_DECLARE_PUBLIC macro. */
#define QE_DECLARE_PUBLIC(Classname) \
    Classname *qe_q_func() { return static_cast<Classname *>(qe_ptr); } \
    inline const Classname* qe_q_func() const { return static_cast<const Classname *>(qe_ptr); } \
    inline const Classname* qe_cq_func() const { return static_cast<const Classname *>(qe_ptr); } \
    friend class Classname;

/* Use this as you would the Q_DECLARE_PRIVATE macro. */
#define QE_DECLARE_PRIVATE(Classname) \
    inline Classname##Private* qe_d_func() { return reinterpret_cast<Classname##Private *>(qed_ptr.data()); } \
    inline const Classname##Private* qe_d_func() const { return reinterpret_cast<const Classname##Private *>(qed_ptr.data()); } \
    inline const Classname##Private* qe_cd_func() const { return reinterpret_cast<const Classname##Private *>(qed_ptr.data()); } \
    friend class Classname##Private;

#define QE_DPTR         auto d = qe_d_func()
#define QE_CONST_DPTR   const auto d = qe_cd_func()
#define QE_QPTR         auto q = qe_q_func()
#define QE_CONST_QPTR   const auto q = qe_cq_func()

#define QE_D    QE_AUTO_D
#define QE_CD   QE_CONST_DPTR
#define QE_Q    QE_AUTO_D
#define QE_CQ   QE_CONST_QPTR

#define Q_DPTR          auto d = d_func()
#define Q_CONST_DPTR    const auto d = d_func()
#define Q_QPTR          auto q = q_func()
#define Q_CONST_QPTR    const auto q = q_func()

namespace qe {

/*!
    \class PrivateBase
    \ingroup module_core
    \brief The PrivateBase class.
*/

class PublicBase;
class QE_CORE_EXPORT PrivateBase
{
public:
    explicit PrivateBase(PublicBase *qq);

protected:
    PublicBase *qe_ptr;
};

/*!
    \class PublicBase
    \ingroup module_core
    \brief The PublicBase class.
*/

class QE_CORE_EXPORT PublicBase
{
public:
    explicit PublicBase(PrivateBase &dd);

protected:
    QScopedPointer<PrivateBase> qed_ptr;
};

//! Constructs a new object with \a qq as the back pointer (q-ptr).
PrivateBase::PrivateBase(PublicBase *qq)
    : qe_ptr(qq)
{
}


//! Constructs a new object with \a dd as the source for the d-ptr.
PublicBase::PublicBase(PrivateBase &dd)
    : qed_ptr(&dd)
{
}

} // namespace qe

#ifndef QEXT_NO_CLUTTER

#endif

#endif //QE_CORE_DPTR_H
