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
#ifndef QE_WINDOWS_TYPES_H
#define QE_WINDOWS_TYPES_H

#include <ShObjIdl.h>
#include <QMetaType>
#include <qewindows/unknownpointer.h>
#include <qewindows/compointer.h>

namespace qe {
namespace windows {

using ShellItemPointer = UnknownPointer<IShellItem2>;
using StreamPointer = UnknownPointer<IStream>;
using StoragePointer = UnknownPointer<IStorage>;

} // namespace windows
} // namespace qe

Q_DECLARE_METATYPE(qe::windows::ShellItemPointer);
Q_DECLARE_METATYPE(qe::windows::StreamPointer);
Q_DECLARE_METATYPE(qe::windows::StoragePointer);
Q_DECLARE_METATYPE(GUID)

#endif // QE_WINDOWS_TYPES_H
