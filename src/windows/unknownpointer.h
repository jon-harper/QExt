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
#ifndef QE_WINDOWS_UNKNOWNPOINTER_H
#define QE_WINDOWS_UNKNOWNPOINTER_H

#include <utility>
#include <type_traits>
#include <combaseapi.h>
#include <qewindows/global.h>

namespace qe {
namespace windows {

/*!
    \class UnknownPointer
    \brief  Smart pointer for managing `IUnknown`-derived interfaces.
 */
template <class IUnknownType>
class UnknownPointer
{
public:
    //! For Boost/Standard Library compatibility.
    using element_type = IUnknownType;
    //! For Boost/Standard Library compatibility.
    using pointer = std::add_pointer_t<IUnknownType>;
    //! For Boost/Standard Library compatibility.
    using const_pointer = std::add_const_t<pointer>;
    //! For Boost/Standard Library compatibility.
    using reference = std::add_lvalue_reference_t<IUnknownType>;
    //! For Boost/Standard Library compatibility.
    using const_reference = std::add_const_t<reference>;

    //! Constructor using CoCreateInstance.
    explicit UnknownPointer(const IID &clsid, IUnknown *outer = nullptr, DWORD context = CLSCTX_INPROC)
        : d(nullptr)
    {
        CoCreateInstance(clsid, outer, context, __uuidof(*this), ppVoid());
    }

    //! Default constructor. `AddRef()` is not called because ownership of the pointer is taken.
    UnknownPointer(pointer p = nullptr) noexcept
        : d(p)
    {
    }

    //! Copy constructor. This calls `AddRef()`.
    UnknownPointer(const UnknownPointer &rhs)
        : d(rhs.data())
    {
        if (d) {
            d->AddRef();
        }
    }

    //! Move constructor. `AddRef()` is not called because ownership of the pointer is taken.
    UnknownPointer(UnknownPointer &&rhs) noexcept
        : d(rhs.take())
    {
    }

    //! Destroys the stored pointer and sets it to `nullptr`.
    ~UnknownPointer()
    {
        reset();
    }

    //! Copy assignment operator
    UnknownPointer &operator=(const UnknownPointer &rhs) noexcept
    {
        d = rhs.d;
        if (d)
            d->AddRef();
        return *this;
    }

    //! Move assignment operator
    UnknownPointer &operator=(UnknownPointer &&rhs) noexcept
    {
        swap(std::move(rhs));
        return *this;
    }

    //! [std/Qt] Sets the stored pointer to `other` and destroys the old pointer.
    void reset(pointer other = nullptr)
    {
        if (d == other)
            return;
        auto oldD = std::exchange(d, other);
        if (oldD) {
            static_assert(sizeof(IUnknownType) > 0, "Type must be complete at destruction.");
            oldD->Release();
        }
    }

    //! [std] Equivalent to \ref take.
    pointer release() noexcept              { return std::exchange(d, nullptr); }

    //! [std] Sets the stored pointer to `nullptr` and returns its old value.
    pointer take() noexcept                 { return release(); }

    //! [Qt] Returns a copy of the stored pointer. Equivalent to `get`.
    pointer data() const noexcept           { return d; }

    //! [std] Returns a copy of the stored pointer. Equivalent to `data`.
    pointer get() const noexcept            { return d; }

    //! Returns a pointer-to-pointer of `T`, that is, the address of the stored pointer.
    pointer * addressOf() noexcept          { return &d; }

    //! Returns true if the stored pointer is not null. Allows `if (ptr)` to work.
    explicit operator bool() const noexcept { return d; }

    //! Returns true if the stored pointer is `nullptr`.
    bool operator!() const noexcept         { return !d; }

    //! Dereferences the stored pointer.
    reference operator*() const noexcept    { return *d; }

    //! Returns the stored pointer, allowing pointer-to-member semantics.
    pointer operator->() const noexcept     { return d; }

    //! Returns if the stored pointer is null or not.
    bool isNull() const noexcept            { return !d; }

    //! Swaps two instances.
    void swap(UnknownPointer &other) noexcept { std::swap(d, other.d); }

    //! Returns the address of the stored pointer, cast as pointer-to-pointer-to-void.
    void ** ppVoid() noexcept               { return reinterpret_cast<void **>(addressOf()); }

    //! Returns the data as a pointer-to-void.
    void * pVoid() const noexcept           { return static_cast<void *>(data()); }

    template <class Other>
    UnknownPointer<Other> queryInterface()
    {
        UnknownPointer<Other> ret;
        auto hr = data()->QueryInterface(__uuidof(*ret), ret.ppVoid());
        if (hr == S_OK && ret)
            return ret;
        else {
            ret.reset();
            return ret;
        }
    }

private:
    pointer d;
};

/*!
  \brief Returns an \ref UnknownPointer created by forwarding \a args to the \ref UnknownPointer constructor.
  \relates UnknownPointer
*/
template <class T, class... Args>
UnknownPointer<T> makeUnknown(Args ...args)
{
    return UnknownPointer<T>(std::forward<Args>(args)...);
}

} //namespace windows
} //namespace qe

//! Equality operator for \ref qe::windows::UnknownPointer.
//! \relates qe::windows::UnknownPointer
template <class T, class U>
bool operator ==(const qe::windows::UnknownPointer<T> &lhs, const U &rhs) noexcept
{
    return lhs.data() == rhs;
}

//! \relates qe::windows::UnknownPointer
//! \overload
template <class T, class U,
          class = std::enable_if_t<std::is_pointer<U>::value || std::is_null_pointer<U>::value>>
bool operator ==(const U &lhs, const qe::windows::UnknownPointer<T> &rhs)
{
    return rhs.data() = lhs;
}

//! \relates qe::windows::UnknownPointer
//! Inequality operator for \ref qe::windows::UnknownPointer.
template <class T, class U>
bool operator !=(const qe::windows::UnknownPointer<T> &lhs, const U &rhs)
{
    return !(lhs == rhs);
}

//! \relates qe::windows::UnknownPointer
//! \overload
template <class T, class U,
          class = std::enable_if_t<std::is_pointer<U>::value || std::is_null_pointer<U>::value>>
bool operator !=(const U & lhs, const qe::windows::UnknownPointer<T> &rhs)
{
    return !(lhs == rhs);
}

namespace std {

/*! \relates qe::windows::UnknownPointer
    Partial specialization of `std::hash` for \ref qe::windows::UnknownPointer.
 */
template <class T>
struct hash<qe::windows::UnknownPointer<T>>
{
    using argument_type = qe::windows::UnknownPointer<T>;
    using result_type = std::size_t;
    result_type operator()(const argument_type & p) const noexcept
    {
        return std::hash<T *>{}(p.data());
    }
};
} //namespace std

#ifndef QEXT_NO_CLUTTER
//! \relates qe::windows::UnknownPointer
template <class T>
using QeUnknownPointer = qe::windows::UnknownPointer<T>;
#endif

#endif // QE_WINDOWS_UNKNOWNPOINTER_H
