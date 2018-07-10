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
#include <QtCore/QMetaType>
#include <qewindows/global.h>

namespace qe {
namespace windows {

//! Smart pointer for managing `IUnknown`-derived interfaces, similar in purpose to ATL's `CComPtr`.
template <class IUnknownType>
class UnknownPointer
{
public:
    //! [std] Alias to the interface type.
    using element_type = IUnknownType;
    //! [std] Alias for `IUnknownType *`.
    using pointer = std::add_pointer_t<IUnknownType>;
    //! [std] Alias for `const IUnknownType *`.
    using const_pointer = std::add_const_t<pointer>;
    //! [std] Alias for `IUnknownType &`.
    using reference = std::add_lvalue_reference_t<IUnknownType>;
    //! [std] Alias for `const IUnknownType &`.
    using const_reference = std::add_const_t<reference>;

    //! Constructor using CoCreateInstance. These arguments are passed directly to a call to `CoCreateInstance`.
    explicit UnknownPointer(const IID &clsid, IUnknown *outer = nullptr, DWORD context = CLSCTX_INPROC)
        : d(nullptr)
    {
        CoCreateInstance(clsid, outer, context, IID_PPV_ARGS(&d));
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
        if (d)
            d->AddRef();
    }

    //! Move constructor. `AddRef()` is not called because ownership of the pointer is taken.
    UnknownPointer(UnknownPointer &&rhs) noexcept
        : d(rhs.take())
    {
    }

    //! Destroys the stored pointer and sets it to `nullptr`. This calls `Release()`.
    ~UnknownPointer()
    {
        reset();
    }

    //! Copy assignment operator. This causes a call to `AddRef()`.
    UnknownPointer &operator=(const UnknownPointer &rhs) noexcept
    {
        UnknownPointer tmp(rhs);
        swap(tmp);
        return *this;
    }

    //! Move assignment operator. `AddRef()` is not called.
    UnknownPointer &operator=(UnknownPointer &&rhs) noexcept
    {
        d = std::exchange(rhs.d, nullptr);
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

    //! [std/Qt] Swaps two instances.
    void swap(UnknownPointer &other) noexcept   { std::swap(d, other.d); }

    //! [std] Sets the stored pointer to `nullptr` and returns its old value. Equivalent to take.
    pointer release() noexcept                  { return std::exchange(d, nullptr); }

    //! [Qt] Sets the stored pointer to `nullptr` and returns its old value. Equivalent to release.
    pointer take() noexcept                     { return release(); }

    //! [Qt] Returns a copy of the stored pointer. Equivalent to `get`.
    pointer data() const noexcept               { return d; }

    //! [std] Returns a copy of the stored pointer. Equivalent to `data`.
    pointer get() const noexcept                { return d; }

    //! Returns a pointer-to-pointer of `T`, that is, the address of the stored pointer.
    pointer * addressOf() noexcept              { return &d; }

    //! Returns true if the stored pointer is not null. Allows `if (ptr)` to work.
    explicit operator bool() const noexcept     { return d; }

//    //! Returns true if the stored pointer is `nullptr`.
//    bool operator!() const noexcept         { return !d; }

    //! Dereferences the stored pointer.
    reference operator*() const noexcept        { return *d; }

    //! Allows pointer-to-member semantics on the stored pointer.
    pointer operator->() const noexcept         { return d; }

    //! [Qt] Returns if the stored pointer is `nullptr`.
    bool isNull() const noexcept                { return !d; }

    //! Returns the data as a pointer-to-void.
    void * asVoid() const noexcept              { return static_cast<void *>(d); }

    IUnknown *asUnknown() const noexcept        { return reinterpret_cast<IUnknown *>(d); }

    //! Returns the address of the stored pointer, cast as pointer-to-pointer-to-void.
    void ** addressAsVoid() noexcept            { return reinterpret_cast<void **>(&d); }

    template <class Other>
    UnknownPointer<Other> queryInterface() noexcept
    {
        if (!d)
            return {};
        UnknownPointer<Other> ret;
        d->QueryInterface(IID_PPV_ARGS(ret.addressOf()));
        return ret;
    }

private:
    IUnknownType *d;
};

//! Alias of UnknownPointer for IUnknown
//! \relates UnknownPointer
using UnknownBasePointer = UnknownPointer<IUnknown>;

//! Alias of UnknownPointer for IDispatch
//! \relates UnknownPointer
using DispatchPointer = UnknownPointer<IDispatch>;

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
    return rhs.data() == lhs;
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

Q_DECLARE_SMART_POINTER_METATYPE(qe::windows::UnknownPointer);
Q_DECLARE_METATYPE(qe::windows::UnknownBasePointer);
Q_DECLARE_METATYPE(qe::windows::DispatchPointer);

#ifndef QEXT_NO_CLUTTER
//! \relates qe::windows::UnknownPointer
template <class T>
using QeUnknownPointer = qe::windows::UnknownPointer<T>;
#endif

#endif // QE_WINDOWS_UNKNOWNPOINTER_H
