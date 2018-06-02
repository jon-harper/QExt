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
#ifndef QE_WINDOWS_SHELLNODE_H
#define QE_WINDOWS_SHELLNODE_H

#include <qewindows/global.h>
#include <QtCore/QVector>
#include <qewindows/shellnodedata.h>

namespace qe {
namespace windows {

class ShellNode;
//! The preferred pointer type for `ShellNode`s.
//! \relates ShellNode
using ShellNodePointer = QSharedPointer<ShellNode>;

class QE_WINDOWS_EXPORT ShellNode
{
public:
    ShellNode(ShellItemPointer item, ShellNodePointer parent);
    ShellNode(const IdListPointer &id, ShellNodePointer parent);
    ShellNode(const ShellNode &) = delete;
    ShellNode(ShellNode &&other) noexcept : d(std::move(other.d)) { }
    ShellNode &operator=(const ShellNode &) = delete;
    ShellNode &operator=(ShellNode &&other)             { swap(std::move(other)); return *this; }
    ~ShellNode() = default;

    void swap(ShellNode &&other) noexcept               { std::swap(d, other.d); }

    bool isValid() const noexcept             { return d.data; }

    ShellNodePointer parent() const noexcept;
    bool isDesktop() const noexcept                     { return !d.parent; }

    QVector<ShellNodePointer> children() const noexcept { return d.children; }
    constexpr bool isEnumerated() const noexcept        { return d.enumerated; }

private:
    struct LocalData {
        ShellNodePointer parent;
        ShellNodeDataPointer data;
        QVector<ShellNodePointer> children;
        bool enumerated = false;
    };

    LocalData d;
};

//! The preferred container type for ShellNodes
//! \relates ShellNode
using ShellNodeContainer = QVector<ShellNodePointer>;

} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLNODE_H
