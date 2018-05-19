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

#include "type_util.h"

namespace qe {

template <typename T>
struct DefaultDeleter
{
    static inline void cleanup(T *pointer)
    {
        qe::detail::assertCompleteType<T>();
        delete pointer;
    }
};

/*! \brief A moveable version of `QScopedPointer`.
  */
template <class T, class Cleanup = DefaultDeleter<T>>
class UniquePointer
{
public:
    //! For Boost/Standard Library compatibility.
    using value_type = T;
    //! For Boost/Standard Library compatibility.
    using pointer = std::add_pointer_t<T>;
    //! For Boost/Standard Library compatibility.
    using const_pointer = std::add_const_t<pointer>;
    //! For Boost/Standard Library compatibility.
    using reference = std::add_lvalue_reference_t<T>;
    //! For Boost/Standard Library compatibility.
    using const_reference = std::add_const_t<reference>;
    //! This is the cleanup object alias. See \ref qe::DefaultDeleter for an example.
    using deleter = Cleanup;

    //! Default constructor.
    inline UniquePointer(pointer p = nullptr) noexcept : d(p) {}
    //! Move constructor
    inline UniquePointer(UniquePointer && other) noexcept : d(other.take()) { }

    template <class U>
    inline UniquePointer(UniquePointer<U, Cleanup> && other)
        : d(other.take())
    {
        qe::detail::assertConvertible<T, U>();
    }
    //! Move assignment operator.
    inline UniquePointer &operator=(UniquePointer &&other)
    {
        if (*this != other)
            reset(other.take());
        return *this;
    }

    template <class U>
    inline UniquePointer &operator=(UniquePointer<U, Cleanup> &&other)
        noexcept(qe::detail::assertConvertible<U, T>())
    {
        qe::detail::assertConvertible<T, U>();
        if (*this != other)
            reset(other.take());
        return *this;
    }

    //! Disables copying.
    UniquePointer(const UniquePointer &) = delete;
    //! Disables copy assignment.
    UniquePointer &operator=(const UniquePointer &) = delete;

    //! Destroys the stored pointer and sets it to `nullptr`.
    inline ~UniquePointer()
    {
        reset(nullptr);
    }

    //! Sets the stored pointer to `nullptr` and returns its old value.
    pointer take() noexcept
    {
        auto oldD = this->d;
        this->d = nullptr;
        return oldD;
    }

    //! Sets the stored pointer to `other` and destroys the old pointer.
    void reset(pointer other = nullptr)
    {
        if (this->d == other)
            return;
        auto oldD = this->d;
        if (oldD)
            deleter::cleanup(oldD);
        this->d = other;

    }

    //! Returns the stored pointer. Equivalent to \ref get.
    pointer data() const noexcept           { return this->d; }

    //! For Boost/Standard Library compatibility. Equivalent to \ref data.
    pointer get() const noexcept            { return this->d; }

    //! Returns a pointer-to-pointer of T, that is, the address of the stored pointer.
    inline pointer * addressOf() noexcept   { return &(this->d); }

    //! Returns true if the stored pointer is valid. Allows `if (ptr)` to work.
    explicit operator bool() const noexcept { return !isNull(); }

    //! Returns true if the stored pointer is `nullptr`.
    bool operator!() const noexcept         { return isNull(); }

    //! Dereferences the stored pointer.
    reference operator*() const noexcept    { return *(this->d); }

    //! Returns the stored pointer, allowing pointer-to-member semantics.
    pointer operator->() const noexcept     { return this->d; }

    //! Returns if the stored pointer is null or not.
    bool isNull() const noexcept            { return !(this->d); }

    void swap(UniquePointer &other) noexcept{ std::swap(this->d, other.d); }

    //! Swaps two convertible instances.
    template <class U>
    void swap(UniquePointer<U, Cleanup> &other)
        noexcept(qe::detail::assertConvertible<U, T>() && qe::detail::assertConvertible<T, U>())
    {
        qe::detail::assertConvertible<U, T>();
        qe::detail::assertConvertible<T, U>();
        std::swap(this->d, other.d);
    }

    //! For Boost/Standard Library compatibility. Equivalent to \ref take.
    pointer release() noexcept              { return take(); }

private:
    pointer d;
};

//! Uses forwarding to construct an instance of UniquePointer.
template <class T, class... Args>
UniquePointer<T> makeUnique(Args && ...args)
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

/*! Partial specialization of `std::hash` for UniquePointer.
   \relates qe::UniquePointer
 */
template <class T, class Cleanup>
struct hash<qe::UniquePointer<T, Cleanup>>
{
    using argument_type = qe::UniquePointer<T, Cleanup>;
    using result_type = std::size_t;
    result_type operator()(const argument_type & p) const noexcept
    {
        return std::hash<T *>{}(p.data());
    }
};
} //namespace std

#ifndef QEXT_NO_CLUTTER
//! \relates qe::DefaultDeleter
template <class T>
using QeDefaultDeleter = qe::DefaultDeleter<T>;

//! \relates qe::UniquePointer
template <class T, class Cleanup = qe::DefaultDeleter<T>>
using QeUniquePointer = qe::UniquePointer<T, Cleanup>;

#endif

#endif //QE_CORE_UNIQUEPOINTER_H
