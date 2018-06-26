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

#define STRICT_TYPED_ITEMIDS
#include <ShlObj.h>
#include <QMetaType>
#include <qewindows/global.h>
#include <qewindows/unknownpointer.h>
#include <qewindows/compointer.h>
#include <qewindows/idlist.h>

namespace qe {
namespace windows {

//! Predefined type for `IShellItem` pointers.
using ShellItemPointer      = UnknownPointer<IShellItem>;
//! Predefined type for `IShellItem2` pointers.
using ShellItem2Pointer     = UnknownPointer<IShellItem2>;
//! Predefined type for `IShellFolder` pointers.
using ShellFolderPointer    = UnknownPointer<IShellFolder>;
//! Predefined type for `IShellFolder2` pointers.
using ShellFolder2Pointer   = UnknownPointer<IShellFolder2>;
//! Predefined type for `IStream` pointers.
using StreamPointer         = UnknownPointer<IStream>;
//! Predefined type for `IStorage` pointers.
using StoragePointer        = UnknownPointer<IStorage>;

} // namespace windows
} // namespace qe

// Metatype declarations for QVariant support
Q_DECLARE_METATYPE(qe::windows::ShellItemPointer);
Q_DECLARE_METATYPE(qe::windows::ShellItem2Pointer);
Q_DECLARE_METATYPE(qe::windows::ShellFolderPointer);
Q_DECLARE_METATYPE(qe::windows::ShellFolder2Pointer);
Q_DECLARE_METATYPE(qe::windows::StreamPointer);
Q_DECLARE_METATYPE(qe::windows::StoragePointer);
Q_DECLARE_METATYPE(GUID) //Allows returning CLSID values from PROPVARIANTs

#ifndef QEXT_NO_CLUTTER
using QeShellItemPointer = qe::windows::ShellItemPointer;
using QeShellItem2Pointer = qe::windows::ShellItem2Pointer;
using QeShellFolderPointer = qe::windows::ShellFolderPointer;
using QeShellFolder2Pointer = qe::windows::ShellFolder2Pointer;
using QeStreamPointer = qe::windows::StreamPointer;
using QeStoragePointer = qe::windows::StoragePointer;
#endif

#endif // QE_WINDOWS_TYPES_H
