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

#include <qewindows/global.h>

#include <ShlObj.h>
#include <QMetaType>

#include <qewindows/unknownpointer.h>
#include <qewindows/compointer.h>
#include <qecore/managedpointer.h>

namespace qe {
namespace windows {

//! Manager struct for ITEMIDLIST_ABSOLUTE
struct IdListManager : ComDeleter<ITEMIDLIST_ABSOLUTE>
{
    static ITEMIDLIST_ABSOLUTE *copy(ITEMIDLIST_ABSOLUTE *ptr) {
        if (ptr)
            return ILCloneFull(ptr);
        else
            return nullptr;
    }
};

//! Specialization of \ref qe::UniquePointer for absolute ITEMIDLISTs using \ref ComDeleter.
//! ILFree is not necessary since Win2k or so.
using IdListPointer = qe::ManagedPointer<ITEMIDLIST_ABSOLUTE, IdListManager>;
using ShellItemPointer = UnknownPointer<IShellItem2>;
using StreamPointer = UnknownPointer<IStream>;
using StoragePointer = UnknownPointer<IStorage>;


} // namespace windows
} // namespace qe

Q_DECLARE_METATYPE(qe::windows::ShellItemPointer);
Q_DECLARE_METATYPE(qe::windows::StreamPointer);
Q_DECLARE_METATYPE(qe::windows::StoragePointer);
Q_DECLARE_METATYPE(GUID)

#ifndef QEXT_NO_CLUTTER
using QeShellIdListManager = qe::windows::IdListManager;
using QeShellIdListPointer = qe::windows::IdListPointer;
using QeShellItemPointer = qe::windows::ShellItemPointer;
using QeStreamPointer = qe::windows::StreamPointer;
using QeStoragePointer = qe::windows::StoragePointer;
#endif

#endif // QE_WINDOWS_TYPES_H
