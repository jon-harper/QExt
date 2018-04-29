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

#ifndef QEDPTR_H
#define QEDPTR_H

#include <QObject>
#include <QScopedPointer>

#include <qecore/global.h>

#define QE_DECLARE_PUBLIC(Classname) \
    Classname *q_func() { return static_cast<Classname *>(qe_ptr); } \
    inline const Classname* q_func() const { return static_cast<const Classname *>(qe_ptr); } \
    inline const Classname* cq_func() const { return static_cast<const Classname *>(qe_ptr); } \
    friend class Classname;
#define QE_DECLARE_PRIVATE(Classname) \
    inline Classname##Private* d_func() { return reinterpret_cast<Classname##Private *>(qed_ptr.data()); } \
    inline const Classname##Private* d_func() const { return reinterpret_cast<const Classname##Private *>(qed_ptr.data()); } \
    inline const Classname##Private* cd_func() const { return reinterpret_cast<const Classname##Private *>(qed_ptr.data()); } \
    friend class Classname##Private;

#define QE_DPTR auto d = d_func()
#define QE_CONST_DPTR const auto d = cd_func()
#define QE_QPTR auto q = q_func()
#define QE_CONST_QPTR const auto q = cq_func()

#define QE_D QE_AUTO_D
#define QE_CD QE_CONST_DPTR
#define QE_Q QE_AUTO_D
#define QE_CQ QE_CONST_QPTR

class QePublicBase;
class QE_CORE_EXPORT QePrivateBase
{
public:
    explicit QePrivateBase(QePublicBase *qq);
    virtual ~QePrivateBase();

protected:
    QePublicBase *qe_ptr;
};

class QE_CORE_EXPORT QePublicBase
{
public:
    explicit QePublicBase(QePrivateBase *dd);
    virtual ~QePublicBase();

protected:
    QScopedPointer<QePrivateBase> qed_ptr;
};

#endif // QEDPTR_H
