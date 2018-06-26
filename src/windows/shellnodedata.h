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
#ifndef QE_WINDOWS_SHELLNODEDATA_H
#define QE_WINDOWS_SHELLNODEDATA_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>
#include <qewindows/shell.h>

namespace qe {
namespace windows {

//! This class holds data needed by both ShellNode and ShellNodeInfo. Both classes
//! carry internal shared pointers to a ShellNodeData object to allow easy
//! conversion from ShellNode to ShellNodeInfo.
struct QE_WINDOWS_EXPORT ShellNodeData
{
    //! The preferred method of passing ShellNodeData around is a QSharedPointer.
    using PointerType = QSharedPointer<ShellNodeData>;
    //! The preferred method of storing ShellNodeData in a container is a QVector.
    using ContainerType = QVector<PointerType>;

    //! Creates a new instance from an IdList.
    static PointerType create(const shell::IdList &ptr);
    //! Creates a new instance from a ShellItem2Pointer.
    static PointerType create(ShellItem2Pointer ptr);

    //! Clears all internally stored data.
    void clear();
    //! Refreshes any cached data.
    void refresh();

    ShellItem2Pointer item;
    shell::IdList id;

    bool invalid = true;

    QString displayName;
    QString parsingName;
    /*QString description;
    QDateTime createdDate;
    QDateTime modified;
    QDateTime accessed;*/

    shell::NodeFlags flags = shell::NodeFlag::NoFlags;
};

//! Alias for QSharedPointer<ShellNodeData>.
//! \related ShellNodeData
using ShellNodeDataPointer = ShellNodeData::PointerType;
//! Alias for QVector<QSharedPointer<ShellNodeData>>.
//! \related ShellNodeData
using ShellNodeDataContainer = ShellNodeData::ContainerType;


namespace shell {
//! Retrieves a pointer to an IShellItem from the given object.
//! \related qe::windows::ShellNodeData
inline IShellItem * retrieveItemPointer(ShellNodeData &data)
{
    auto ret = data.item;
    return ret.queryInterface<IShellItem>();
}

//! Retrieves a pointer to an IShellItem from the given object pointer.
//! \related qe::windows::ShellNodeData
inline IShellItem * retrieveItemPointer(ShellNodeDataPointer ptr)
{
    return retrieveItemPointer(*ptr);
}

} // namespace shell

} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLNODEDATA_H
