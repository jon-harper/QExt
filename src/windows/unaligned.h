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
#ifndef QE_WINDOWS_UNALIGNED_H
#define QE_WINDOWS_UNALIGNED_H

#include <utility>

namespace qe {
namespace windows {

#ifndef __unaligned

namespace detail {

template <class T>
struct remove_all_pointers { using type = T; };

template <class T>
struct remove_all_pointers<T *> { using type = remove_all_pointers<T>; };

template <class T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

template<class T>
struct is_aligned_helper : std::true_type {};

template<class T>
struct is_aligned_helper<__unaligned T> : std::false_type {};

template <class T>
struct remove_aligned_helper { using type = T; };

template <class T>
struct remove_aligned_helper<__unaligned T> { using type = T; };
}

//! Checks if a given T has the `__unaligned` specifier attached
template<class T , class = std::enable_if<std::is_pointer_v<T>>>
struct is_aligned : detail::is_aligned_helper<detail::remove_all_pointers_t<T>> {};

//! Shorthand for the value returned by is_aligned.
template<class T>
using is_aligned_v = typename is_aligned<T>::value;

//! This removes the unaligned specifier from a type by stripping its pointer and readding it.
//! \note This may remove any `const` specifier on the pointer
template <class T>
struct remove_aligned
{
    //remove the pointer, remove the __unaligned specifier, then put the pointer back
    using type = std::add_pointer_t<
        typename detail::remove_aligned_helper<
            std::remove_pointer_t<T>>::type>;
};

//! Partial specialization of remove_aligned for pointers to pointers
//! \note This may remove any const specifiers on the pointers
template <class T>
struct remove_aligned<T **> {
    using type = std::add_pointer_t<typename remove_aligned<T *>::type>;
};

//! Shorthand for the type defined by remove_aligned<T>::type
template <class T>
using remove_aligned_t = typename remove_aligned<T>::type;

#else
namespace detail {
template <class T> struct identity_of { using type = T; };
}

template <class>
using is_aligned = std::true_type;

template <class T>
using is_aligned_v = is_aligned<T>();

template <class T>
using remove_aligned = detail::identity_of<T>;

template <class T>
using remove_aligned_t = T;
#endif // __unaligned

//! Uses const_cast to remove the `__unaligned` specifier from a given type and reinterpret_cast
//! to convert From to To.
template <class To, class From, class = std::enable_if<std::is_pointer<From>::value/*() && !is_aligned<From>()*/>>
inline constexpr To aligned_cast(From value) noexcept
{
    return reinterpret_cast<To>(const_cast<remove_aligned_t<From>>(value));
}

} // namespace windows
} // namespace qe


#endif // QE_WINDOWS_UNALIGNED_H
