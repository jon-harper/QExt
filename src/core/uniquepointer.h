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

#ifndef QE_CORE_UNIQUEPOINTER_H
#define QE_CORE_UNIQUEPOINTER_H

#include <cstddef>
#include <QScopedPointerDeleter>

#include "pointer.h"

namespace qe {
/*! A moveable version of QScopedPointer.
  */
template <class T, class Cleanup = ::QScopedPointerDeleter<T>>
class UniquePointer : public detail::Pointer_Impl<T, Cleanup>
{
public:
    using parent = detail::Pointer_Impl<T, Cleanup>;
    using pointer = typename parent::pointer;

    //! Default constructor.
    inline UniquePointer(pointer p = nullptr) : parent(p) {}
    //! Move constructor
    inline UniquePointer(UniquePointer && rhs)  : parent(rhs.take()) { }

    //! Destructor. Calls `reset()`.
    inline ~UniquePointer()
    {
        reset();
    }

    //! Move assignment operator.
    inline UniquePointer &operator=(UniquePointer &&rhs)
    {
        if (*this != rhs)
            reset(rhs.take());
        return *this;
    }

    //! Disables copying.
    UniquePointer(const UniquePointer &) = delete;
    //! Disables copy assignment.
    UniquePointer &operator=(const UniquePointer &) = delete;

};

//! Uses forwarding to construct an instance of UniquePointer.
template <class T, class... Args>
UniquePointer<T> makeUnique(Args &&... args)
{
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}

} //namespace qe

/*!
  Returns true if \a lhs and \a rhs manage the same pointer.
  \relates qe::UniquePointer
 */
template <class T, class Cleanup>
inline bool operator==(const qe::UniquePointer<T, Cleanup> &lhs, const qe::UniquePointer<T, Cleanup> &rhs) noexcept
{
    return lhs.data() == rhs.data();
}

//! \overload
template<class T, class Cleanup>
inline bool operator ==(const qe::UniquePointer<T, Cleanup> &lhs, std::nullptr_t) noexcept
{
    return lhs.isNull();
}

//! \overload
template<class T, class Cleanup>
inline bool operator ==(std::nullptr_t, const qe::UniquePointer<T, Cleanup> &rhs) noexcept
{
    return rhs.isNull();
}

/*!
  Returns true if \a lhs and \a rhs do *not* manage the same pointer.
  \relates qe::UniquePointer
 */
template <class T, class Cleanup>
inline bool operator!=(const qe::UniquePointer<T, Cleanup> &lhs, const qe::UniquePointer<T, Cleanup> &rhs) noexcept
{
    return lhs.data() != rhs.data();
}

//! \overload
template <class T, class Cleanup>
inline bool operator !=(const qe::UniquePointer<T, Cleanup> &lhs, std::nullptr_t) noexcept
{
    return !lhs.data();
}

//! \overload
template <class T, class Cleanup>
inline bool operator !=(std::nullptr_t, const qe::UniquePointer<T, Cleanup> &rhs) noexcept
{
    return !rhs.data();
}

namespace std {
/*! Partial specialization of `std::swap` for UniquePointer.
  \relates qe::UniquePointer
 */
    template <class T, class Cleanup>
    inline void swap(qe::UniquePointer<T, Cleanup> &lhs, qe::UniquePointer<T, Cleanup> &rhs) noexcept
    {
        lhs.swap(rhs);
    }
} //namespace std

#ifndef QEXT_NO_CLUTTER
template <class T, class Cleanup = ::QScopedPointerDeleter<T>>
using QeUniquePointer = qe::UniquePointer<T, Cleanup>;

#endif

#endif //QE_CORE_UNIQUEPOINTER_H
