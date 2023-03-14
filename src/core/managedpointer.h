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

/*!
 \headerfile managedpointer.h <qecore/managedpointer.h>
 \brief Defines the base classes for managed pointers and deleters.
*/

#ifndef QE_CORE_MANAGEDPOINTER_H
#define QE_CORE_MANAGEDPOINTER_H

#include <qecore/uniquepointer.h>

namespace qe {

//! \brief A trivial example of a pointer manager.
template <class T>
struct DefaultManager : DefaultDeleter<T>
{
    //! Returns `new T(*pointer)` if `pointer` is not null.
    static T * copy(T *pointer)
    {
        if (pointer)
            return new T(*pointer);
        return nullptr;
    }
};

//! \brief Smart pointer for externally managed allocation, copy, and deletion.
//! This class is primarily the template base for QeWindows types that do not derive from IUnknown.
//! The pointer it manages must be manually allocated externally. Copying and destruction are
//! handled by the manager object.
//!
//! \arg Manager must be a struct containing the following signatures:
//!         static void cleanup(T *);
//!         static T * copy(T *);
//!
//! Note that `copy` may return `nullptr` if it is used on a non-copyable type (though you should
//! use \ref UniquePointer instead.
//!
//! See \ref DefaultManager for a trivial implementation.
template <class T, class Manager>
class ManagedPointer : public UniquePointer<T, Manager>
{
public:
    //! Alias for the `Manager` template argument. This is used during copy construction.
    using copier_type = Manager;
    //! Introduces `pointer` as a valid return and argument type.
    using pointer = typename UniquePointer<T, Manager>::pointer;

    //! Default constructor. Takes ownership of \arg p.
    ManagedPointer(pointer p = nullptr) noexcept : UniquePointer<T, Manager>(p) {}

    //! Creates a copy of \arg other.
    ManagedPointer(const ManagedPointer &other)
        : UniquePointer<T, Manager>(copier_type::copy(other.data()))
    {
    }

    //! Move constructs from \arg other.
    ManagedPointer(ManagedPointer &&other)
        : UniquePointer<T, Manager>(other.release())
    {
    }

    //! Move assignement operator
    ManagedPointer &operator=(ManagedPointer &&other)
    {
        reset(other.release());
        return *this;
    }

    //! Copies \arg other and returns a reference to `this`.
    ManagedPointer &operator=(const ManagedPointer &other)
    {
        reset(copier_type::copy(other.data()));
        return *this;
    }
};

} // namespace qe

namespace std {
/*! Partial specialization of `std::hash` for `ManagedPointer`.
   \relates qe::ManagedPointer
 */
template <class T, class Manager>
struct hash<qe::ManagedPointer<T, Manager>>
{
    using argument_type = qe::ManagedPointer<T, Manager>;
    using result_type = std::size_t;
    result_type operator()(const argument_type & p) const noexcept
    {
        return std::hash<T *>{}(p.data());
    }
};
} //namespace std

//Q_DECLARE_SMART_POINTER_METATYPE does not take arguments with more than one template parameter.
//#ifndef QEXT_CORE_NO_QT
//#include <QtCore/QMetaType>
//Q_DECLARE_SMART_POINTER_METATYPE(qe::ManagedPointer);
//#endif

#ifndef QEXT_NO_CLUTTER
//! \relates qe::DefaultManager
template <class T>
using QeDefaultManager = qe::DefaultManager<T>;

//! \relates qe::ManagedPointer
template <class T, class Manager>
using QeManagedPointer = qe::ManagedPointer<T, Manager>;

#endif

#endif // QE_CORE_MANAGEDPOINTER_H
