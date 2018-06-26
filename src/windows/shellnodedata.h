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

struct QE_WINDOWS_EXPORT ShellNodeData
{
    using PointerType = QSharedPointer<ShellNodeData>;
    using ContainerType = QVector<PointerType>;

    static PointerType create(const shell::IdList &ptr);
    static PointerType create(ShellItem2Pointer ptr);

    void clear();
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

using ShellNodeDataPointer = ShellNodeData::PointerType;
using ShellNodeDataContainer = ShellNodeData::ContainerType;

} // namespace windows
} // namespace qe

inline bool operator==(const qe::windows::ShellNodeData &left, IShellItem2 *right)
{
    if (left.invalid)
        return right ? true : false;
    if (!right)
        return false;
    int result = 0;
    left.item->Compare(right, static_cast<DWORD>(SICHINT_CANONICAL), &result);
    return result;
}

//inline bool operator==(const qe::windows::ShellNodeDataPointer &left, const qe::windows::ShellNodeDataPointer &right)
//{
//    return left == right.data();
//}

//inline bool operator==(const IShellItem *left, const qe::windows::ShellNodeDataPointer &right)
//{
//    return right == left;
//}

//inline bool operator !=(const qe::windows::ShellNodeDataPointer &left, const qe::windows::ShellNodeDataPointer &right)
//{
//    return !(left == right);
//}
#endif // QE_WINDOWS_SHELLNODEDATA_H
