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

#ifndef QE_CORE_POINTER_DELETERS_H
#define QE_CORE_POINTER_DELETERS_H

#include <cstdlib>

namespace qe {

//! The default deleter used with \ref qe::UniquePointer.
//! Unless you are using something other than `new` to construct your data,
//! this works fine.
template <typename T>
struct DefaultDeleter {
    static void cleanup(T *ptr) {
        if (ptr)
			static_assert (sizeof (T) > 0, "DefaultDeleter requires a complete type on cleanup.");
        delete ptr;
    }
};

struct PodDeleter
{
    static void cleanup(void *pointer) {
        if (pointer)
            free(pointer);
    }
};

#ifndef QEXT_CORE_NO_QT
template <typename T>
struct ObjectDeleterBase
{
    static void cleanup(T *pointer) {
        if (pointer)
            pointer->deleteLater();
    }
};

class QObject;
using ObjectDeleter = ObjectDeleterBase<QObject>;
#endif //QEXT_CORE_NO_QT

} // namespace qe

#ifndef QEXT_NO_CLUTTER
//! \relates qe::DefaultDeleter
template <class T>
using QeDefaultDeleter = qe::DefaultDeleter<T>;
//! \relates qe::ObjectDeleterBase
using QeObjectDeleter = qe::ObjectDeleter;
#endif //QEXT_NO_CLUTTER

#endif //QE_CORE_POINTER_DELETERS_H
