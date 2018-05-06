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

#include <type_traits>

namespace qe {
namespace detail {

template <class T, class Cleanup>
class Pointer_Impl
{
public:
    //! For STL compatibility.
    using pointer = std::add_pointer_t<T>;
    //! For STL compatibility.
    using element_type = T;
    using reference = std::add_lvalue_reference_t<T>;
    using deleter = Cleanup;

    //! Sets the stored pointer to `nullptr` and returns its old value.
    pointer take() noexcept
    {
        auto ret = data();
        this->d = nullptr;
        return ret;
    }

    //! Sets the stored pointer to `nullptr` and destroys its old data.
    void reset()
    {
        auto oldD = take();
        if (oldD)
            deleter::cleanup(oldD);
    }

    //! Sets the stored pointer to `other` and destroys the old pointer.
    void reset(pointer other)
    {
        if (data() == other)
            return;
        auto oldD = data();
        this->d = other;
        if (oldD)
            deleter::cleanup(oldD);
    }

    //! Returns the stored interface pointer.
    pointer data() const noexcept           { return isNull() ? nullptr : this->d; }

    //! For STL compatibility. Calls \ref data.
    pointer get() const noexcept            { return data(); }

    //! Returns true if the stored pointer is valid.
    explicit operator bool() const noexcept { return !isNull(); }

    //! Returns true if the stored pointer is `nullptr`.
    bool operator!() const noexcept         { return isNull(); }

    //! Dereferences the stored pointer.
    reference operator*() const             { return *(data()); }

    //! Returns the stored pointer, allowing pointer-to-member semantics.
    pointer operator->() const noexcept     { return data(); }

    //! Returns if the stored pointer is null or not.
    bool isNull() const noexcept            { return (this->d == nullptr); }

    void swap(Pointer_Impl &other) noexcept { std::swap(this->d, other.d); }
    //! For STL compatibility. Equivalent to \ref take.
    pointer release() noexcept              { return take(); }

protected:
    //! Default construction
    Pointer_Impl() noexcept : d(nullptr) {}
    Pointer_Impl(pointer p) noexcept : d(p) {}
    ~Pointer_Impl() {}

private:
    pointer d;
};

} //namespace detail
} //namespace qe

