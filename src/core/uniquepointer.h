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

#pragma once

#include "pointer.h"
#include <QScopedPointerDeleter>
#include <cstddef>

template <class T, class Cleanup = QScopedPointerDeleter<T>>
class QeUniquePointer : public qe::detail::Pointer_Impl<T, Cleanup>
{
public:
    //! Default constructor.
    inline QeUniquePointer(T *p = nullptr) : Pointer_Impl(p) {}
    //! Move constructor
    inline QeUniquePointer(QeUniquePointer && rhs)  : Pointer_Impl(rhs.take()) { }

    //! Move assignment operator.
    inline QeUniquePointer &operator=(QeUniquePointer &&rhs)
    {
        if (*this != rhs)
            reset(rhs.take());
        return *this;
    }

    //! Disables copying.
    QeUniquePointer(const QeUniquePointer &) = delete;
    //! Disables copy assignment.
    QeUniquePointer &operator=(const QeUniquePointer &) = delete;

    //! The destructor checks if the stored pointer is valid, and destroys it if necessary.
    inline ~QeUniquePointer()
    {
        reset();
    }

};

//! Uses forwarding to construct an instance of UniquePointer.
template <class T, class... Args>
QeUniquePointer<T> qeMakeUnique(Args &&... args)
{
    return new T(std::forward<Args>(args)...);
}

/*!
  Returns true if \a lhs and \a rhs manage the same pointer.
  \relates UniquePointer
 */
template <class T, class Cleanup>
inline bool operator==(const QeUniquePointer<T, Cleanup> &lhs, const QeUniquePointer<T, Cleanup> &rhs)
{
    return lhs.data() == rhs.data();
}

template<class T, class Cleanup>
inline bool operator ==(const QeUniquePointer<T, Cleanup> &lhs, std::nullptr_t rhs)
{
    return lhs.data() == rhs;
}

template<class T, class Cleanup>
inline bool operator ==(std::nullptr_t lhs, const QeUniquePointer<T, Cleanup> &rhs)
{
    return lhs == rhs.data();
}

/*!
  Returns true if \a lhs and \a rhs do *not* manage the same pointer.
  \relates UniquePointer
 */
template <class T, class Cleanup>
inline bool operator!=(const QeUniquePointer<T, Cleanup> &lhs, const QeUniquePointer<T, Cleanup> &rhs)
{
    return !(lhs.data() == rhs.data());
}

template <class T, class Cleanup>
inline bool operator !=(const QeUniquePointer<T, Cleanup> &lhs, std::nullptr_t rhs)
{
    return !(lhs.data() == rhs);
}

template <class T, class Cleanup>
inline bool operator !=(std::nullptr_t lhs, const QeUniquePointer<T, Cleanup> &rhs)
{
    return !(lhs == rhs.data());
}

namespace std {
/*!
  Specialization of `std::swap` for UniquePointer.
  \relates UniquePointer
 */
    template <class T, class Cleanup>
    inline void swap(QeUniquePointer<T, Cleanup> &p1, QeUniquePointer<T, Cleanup> &p2)
    {
        p1.swap(p2);
    }
}
