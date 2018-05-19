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

namespace qe {
namespace detail {

//! Enforces a complete type. Useful for deleters to ensure the type they are to destroy is complete.
template <class T>
constexpr void assertCompleteType() noexcept
{
    using forced_complete_type = char[sizeof(T) ? 1 : -1];
    (void)sizeof(forced_complete_type);
}

//! Forces a compile error if `T` is not convertible to `U`;
template <class T, class U>
void assertConvertible()
{
    using convertible = char[sizeof(std::is_convertible<T, U>()) ? 1 : -1];
    (void)sizeof(convertible);
}

} //namespace detail
} //namespace qe

#endif //QE_CORE_TYPE_UTIL_H
