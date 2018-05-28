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
#ifndef QE_CORE_MANAGEDPOINTER_H
#define QE_CORE_MANAGEDPOINTER_H

#include <qecore/uniquepointer.h>

namespace qe {

template <class T>
struct DefaultManager : DefaultDeleter<T>
{
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
    using copier_type = Manager;
    using pointer = std::add_pointer_t<T>;

    ManagedPointer(pointer p = nullptr) noexcept : UniquePointer<T, Manager>(p) {}

    ManagedPointer(const ManagedPointer &other)
        : UniquePointer<T, Manager>(copier_type::copy(other.data()))
    {
    }

    ManagedPointer(ManagedPointer &&other)
        : UniquePointer<T, Manager>(other.release())
    {
    }

    template<class U, class CleanupU, class = std::enable_if_t<detail::is_pointer_static_castable<U*,pointer>::value>>
    ManagedPointer(ManagedPointer<U, CleanupU> && other) noexcept
        : UniquePointer<T, Manager>(std::move(other))
    {
    }

    ManagedPointer &operator=(ManagedPointer &&other)
    {
        reset(other.release());
        return *this;
    }

    template<class U, class CleanupU, class = std::enable_if_t<detail::is_pointer_static_castable<U*,pointer>::value>>
    ManagedPointer &operator=(ManagedPointer<U, CleanupU> &&other) noexcept
    {
        swap(static_cast<pointer>(other));
        return *this;
    }

    ManagedPointer &operator=(const ManagedPointer &other)
    {
        reset(copier_type::copy(other.data()));
        return *this;
    }
};

} // namespace qe

#endif // QE_CORE_MANAGEDPOINTER_H
