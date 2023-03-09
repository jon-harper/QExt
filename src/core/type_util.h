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

#ifndef QE_CORE_TYPE_UTIL_H
#define QE_CORE_TYPE_UTIL_H

#include <type_traits>
#include <utility>

namespace qe {

//! Tests if \a Derived inherits from \a Base and a virtual destructor is available.
template <class Base, class Derived>
struct is_derived_pointer_safely_castable
        : ::std::bool_constant<::std::is_base_of<Base, Derived>() && ::std::has_virtual_destructor<Base>()>
{
};


//! \cond
template <class From, class To, class = void>
struct is_pointer_static_castable : std::false_type
{
};
//! \endcond

//! Evaluates to true if \a From will `static_cast` to \a To without returning `nullptr`.
template <class From, class To>
struct is_pointer_static_castable<From, To,
                           ::std::void_t<decltype(static_cast<To>(::std::declval<From>()))>>
    : ::std::true_type
{
    static constexpr bool value = true;
};

} //namespace qe

#endif //QE_CORE_TYPE_UTIL_H
