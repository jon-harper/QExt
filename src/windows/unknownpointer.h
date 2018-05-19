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

#include <type_traits>
#include <combaseapi.h>
#include <qecore/type_util.h>
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
    //! Shorthand for the specific template type.
    using this_type = UnknownPointer<IUnknownType>;

    //! Constructor using CoCreateInstance.
    explicit inline UnknownPointer(const IID &clsid, IUnknown *outer = nullptr, DWORD context = CLSCTX_INPROC)
        : d(nullptr)
    {
        CoCreateInstance(clsid, outer, context, __uuidof(*this), ppVoid());
    }

    //! Default constructor. `AddRef()` is not called because ownership of the pointer is taken.
    inline UnknownPointer(pointer p = nullptr) noexcept
        : d(p)
    {
    }

    //! Copy constructor. This calls `AddRef()`.
    inline UnknownPointer(const this_type &rhs)
        : d(rhs.data())
    {
        if (!isNull()) {
            data()->AddRef();
        }
    }

    //! Move constructor. `AddRef()` is not called because ownership of the pointer is taken.
    inline UnknownPointer(this_type &&rhs) noexcept
        : d(rhs.take())
    {
    }

    //! Destroys the stored pointer and sets it to `nullptr`.
    inline ~UnknownPointer()
    {
        reset(nullptr);
    }

    //! Copy assignment operator
    inline this_type &operator=(const this_type &rhs) noexcept
    {
        this_type copy(rhs);
        swap(copy);
        return *this;
    }

    //! Move assignment operator
    inline this_type &operator=(this_type &&rhs) noexcept
    {
        swap(std::move(rhs));
        return *this;
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
        this->d = other;
        if (oldD) {
            qe::detail::assertCompleteType<IUnknownType>();
            oldD->Release();
        }
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

    //! Swaps two instances.
    void swap(this_type &other) noexcept { std::swap(this->d, other.d); }

    //! For Boost/Standard Library compatibility. Equivalent to \ref take.
    pointer release() noexcept              { return take(); }

    //! Returns the address of the stored pointer, cast as pointer-to-pointer-to-void.
    inline void ** ppVoid()
    {
        return reinterpret_cast<void **>(addressOf());
    }

    //! Returns the data as a pointer-to-void.
    inline void * pVoid()
    {
        return reinterpret_cast<void *>(data());
    }

    template <class Other>
    inline UnknownPointer<Other> queryInterface()
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
template <class T>
inline bool operator ==(const qe::windows::UnknownPointer<T> &lhs, const qe::windows::UnknownPointer<T> &rhs)
{
    return lhs.data() == rhs.data();
}

//! \relates qe::windows::UnknownPointer
//! \overload
template<class T>
inline bool operator ==(const qe::windows::UnknownPointer<T> &lhs, std::nullptr_t)
{
    return lhs.isNull();
}

//! \relates qe::windows::UnknownPointer
//! \overload
template<class T>
inline bool operator ==(std::nullptr_t, const qe::windows::UnknownPointer<T> &rhs)
{
    return rhs.isNull();
}

//! \relates qe::windows::UnknownPointer
//! Inequality operator for \ref qe::windows::UnknownPointer.
template <class T>
inline bool operator !=(const qe::windows::UnknownPointer<T> &lhs, const qe::windows::UnknownPointer<T> &rhs)
{
    return !(lhs == rhs);
}

//! \relates qe::windows::UnknownPointer
//! \overload
template <class T>
inline bool operator !=(const qe::windows::UnknownPointer<T> &lhs, std::nullptr_t)
{
    return !(lhs.isNull());
}

//! \relates qe::windows::UnknownPointer
//! \overload
template <class T>
inline bool operator !=(std::nullptr_t, const qe::windows::UnknownPointer<T> &rhs)
{
    return !(rhs.isNull());
}

namespace std {

//! \relates qe::windows::UnknownPointer
//! Partial specialization of `std::swap` for \ref qe::windows::UnknownPointer.
template <class T>
inline void swap(qe::windows::UnknownPointer<T> &lhs, qe::windows::UnknownPointer<T> &rhs)
{
    lhs.swap(rhs);
}

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
