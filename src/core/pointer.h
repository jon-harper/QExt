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

#ifndef QE_CORE_POINTER_H
#define QE_CORE_POINTER_H

#include <type_traits>

namespace qe {
namespace detail {

//! Enforces a complete type. Useful for deleters to ensure the type they are to destroy is complete.
template <class T>
constexpr void forceCompleteType() noexcept
{
    using forced_complete_type = char[sizeof(T) ? 1 : -1];
    (void)sizeof(forced_complete_type);
}

/*!
    \class qe::detail::Pointer_Impl
    \brief Implements general smart pointer functions and operators.

    All QExt smart pointers derive from this base class.
*/
template <class T, class Cleanup>
class Pointer_Impl
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

    //! Destroys the stored pointer and sets it to `nullptr`.
    inline ~Pointer_Impl()
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

    //! Swaps two instances.
    void swap(Pointer_Impl<T, Cleanup> &other) noexcept { std::swap(this->d, other.d); }

    //! For Boost/Standard Library compatibility. Equivalent to \ref take.
    pointer release() noexcept              { return take(); }

protected:
    //! \brief The default constructor.
    //! \note Inheriting classes \em must implement all constructors and assignment operators!
    Pointer_Impl(pointer p = nullptr) noexcept : d(p) {}

private:
    pointer d;
};

} //namespace detail
} //namespace qe

#endif //QE_CORE_POINTER_H
