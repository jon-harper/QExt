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
namespace detail {

template <class Base, class Derived>
struct is_safely_castable
        : std::bool_constant<std::is_base_of<Base, Derived>() && std::has_virtual_destructor<Base>()>
{
};

template <class Base, class Derived>
using is_pointer_safely_castable_v = typename is_safely_castable<Base, Derived>::value;


template <class From, class To, typename = void>
struct is_pointer_static_castable : std::false_type
{
};

template <class From, class To>
struct is_pointer_static_castable<From, To,
                           std::void_t<decltype(static_cast<To>(std::declval<From>()))>>
    : std::true_type
{
    static constexpr bool value = true;
};

} //namespace detail
} //namespace qe

#endif //QE_CORE_TYPE_UTIL_H
