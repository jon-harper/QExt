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

#include <utility>
#include "type_util.h"

namespace qe {

//! The default deleter used with \ref qe::UniquePointer.
//! Unless you are using something other than `new` to construct your data,
//! this works fine.
template <typename T>
struct DefaultDeleter
{
    static void cleanup(T *pointer)
    {
        static_assert (sizeof (T) > 0, "DefaultDeleter requires a complete type on cleanup.");
        delete pointer;
    }
};

/*! \brief A moveable version of `QScopedPointer`.

  UniquePointer is a low-level wrapper around a raw pointer. It cannot be
  copied but can be moved, including to convertible types. It was motivated
  by a desire to create a version of `QScopedPointer` that can be moved like
  `std::unique_pointer`, while maintaining compatibility with both.

  The second template argument is the deleter, called `Cleanup`. It must
  be a struct with a static function named `cleanup` that accepts a pointer
  as an argument and destroys it. By default, this is \ref qe::DefaultDeleter,
  which uses `delete`. This deleter is compatible with the existing deleters
  available for `QScopedPointer`. If you are, for instance, using `malloc()`,
  you can pass `QScopedPointerPodDeleter` as the `Cleanup` template argument.

  \warning UniquePointer does **not** support basic arrays. Use std::array
  instead.
  */
template <class T, class Cleanup = DefaultDeleter<T>>
class UniquePointer
{
public:
    //! Shorthand for the template specialization in use
    using element_type = T;
    using pointer = std::add_pointer_t<T>;
    using const_pointer = std::add_const_t<pointer>;
    using reference = std::add_lvalue_reference_t<T>;
    using const_reference = std::add_const_t<reference>;
    //! This is the cleanup object alias. See \ref qe::DefaultDeleter for an example.
    using deleter_type = Cleanup;

    //! Default constructor accepting a (possibly null) pointer.
    UniquePointer(pointer p = nullptr) noexcept : d(p) {}
    //! Move constructor for UniquePointers managing the same types.
    UniquePointer(UniquePointer && other) noexcept : d(other.release()) { }

    template<class U, class CleanupU, class = std::enable_if_t<detail::is_pointer_static_castable<U*,pointer>::value>>
    UniquePointer(UniquePointer<U, CleanupU> && other) noexcept
        : d(static_cast<pointer>(other.release()))
    {
    }

    //! Move assignment operator.
    UniquePointer &operator=(UniquePointer &&other)
    {
        d = std::exchange(other.d, nullptr);
        return *this;
    }

    template<class U, class CleanupU, class = std::enable_if_t<detail::is_pointer_static_castable<U*,pointer>::value>>
    UniquePointer &operator=(UniquePointer<U, CleanupU> &&other) noexcept
    {
        d = std::exchange(static_cast<pointer>(other.d), nullptr);
        return *this;
    }

    //! Disables copying.
    UniquePointer(const UniquePointer &) = delete;
    //! Disables copy assignment.
    UniquePointer &operator=(const UniquePointer &) = delete;

    //! Destroys the stored pointer and sets it to `nullptr`.
    ~UniquePointer()
    {
        reset();
    }

    //! [std] Sets the stored pointer to `nullptr` and returns its old value.
    //! Equivalent to \ref take.
    pointer release() noexcept
    {
        return std::exchange(d, nullptr);
    }

    //! [Qt] Sets the stored pointer to `nullptr` and returns its old value.
    //! Equivalent to \ref release.
    pointer take() noexcept              { return release(); }

    //! [std/Qt] Sets the stored pointer to `other` and destroys the old pointer.
    //! Equivalent to \ref take.
    void reset(pointer other = nullptr)
    {
        if (d == other)
            return;
        deleter_type::cleanup(std::exchange(d, other));
    }

    //! [Qt] Returns the stored pointer. Equivalent to `get`.
    pointer data() const noexcept            { return d; }

    //! [std] Equivalent to \ref data.
    pointer get() const noexcept             { return d; }

    //! Returns a pointer-to-pointer of T, that is, the address of the stored pointer.
    pointer* addressOf() const noexcept      { return &d; }

    //! Returns true if the stored pointer is valid. Allows `if (ptr)` to work.
    explicit operator bool() const noexcept  { return d; }

    //! Returns true if the stored pointer is `nullptr`.
    bool operator!() const noexcept          { return !d; }

    //! Dereferences the stored pointer.
    reference operator*() const noexcept     { return *d; }

    //! Returns the stored pointer, allowing pointer-to-member semantics.
    pointer operator->() const noexcept      { return d; }

    //! Returns if the stored pointer is null or not.
    bool isNull() const noexcept             { return !d; }

    //! [std/Qt] Swaps two instances.
    void swap(UniquePointer &other) noexcept { std::swap(d, other.d); }

private:
    pointer d;
};

//! Uses `std::forward` and `new` to construct an instance of UniquePointer.
//! \relates qe::UniquePointer
template <class T, class... Args>
UniquePointer<T> makeUnique(Args && ...args)
{
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}

} //namespace qe

//! \brief Returns true if \a lhs and \a rhs are equal.
//! \a rhs may be a pointer, nullptr, or even another `UniquePointer`.
//! \relates qe::UniquePointer
template <class T, class Cleanup, class U>
bool operator==(const qe::UniquePointer<T, Cleanup> &lhs, const U &rhs) noexcept
{
    return lhs.data() == rhs;
}

//! \brief Returns true if \a lhs and \a rhs are equal.
//! The `enable_if_t` eliminates this version from selection when `U` is another `UniquePointer`.
//! \relates qe::UniquePointer
template<class T, class Cleanup, class U,
         class = std::enable_if_t<std::is_pointer<U>::value || std::is_null_pointer<U>::value>>
bool operator ==(const U &lhs, const qe::UniquePointer<T, Cleanup> &rhs) noexcept
{
    return rhs.data() == lhs;
}

/*!
  \brief Returns true if \a lhs and \a rhs are not equal.
  \a rhs may be a pointer, `nullptr`, or even another `qe::UniquePointer`.
  \relates qe::UniquePointer
 */
template <class T, class Cleanup, class U>
bool operator!=(const qe::UniquePointer<T, Cleanup> &lhs, const U &rhs) noexcept
{
    return !(lhs == rhs);
}

/*!
  \brief Returns true if \a lhs and \a rhs are not equal.
  The `enable_if_t` eliminates this template from selection when `U` is also a UniquePointer of some type.
  \relates qe::UniquePointer
 */
template <class T, class Cleanup, class U,
          class = std::enable_if_t<std::is_pointer<U>::value || std::is_null_pointer<U>::value>>
bool operator!=(const U &lhs, const qe::UniquePointer<T, Cleanup> &rhs) noexcept
{
    return !(lhs == rhs);
}

namespace std {
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

#ifndef QEXT_CORE_NO_QT
#include <QtCore/QMetaType>
Q_DECLARE_SMART_POINTER_METATYPE(qe::UniquePointer);
#endif

#ifndef QEXT_NO_CLUTTER
//! \relates qe::DefaultDeleter
template <class T>
using QeDefaultDeleter = qe::DefaultDeleter<T>;

//! \relates qe::UniquePointer
template <class T, class Cleanup = qe::DefaultDeleter<T>>
using QeUniquePointer = qe::UniquePointer<T, Cleanup>;

#endif

#endif //QE_CORE_UNIQUEPOINTER_H
