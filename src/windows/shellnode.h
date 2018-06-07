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

#include <QtCore/QVector>
#include <QtCore/QSharedPointer>
#include <qewindows/types.h>
#include <qewindows/shellnodedata.h>

namespace qe {
namespace windows {

class ShellCache;

class QE_WINDOWS_EXPORT ShellNode
{
public:
    using pointer_type = QSharedPointer<ShellNode>;

    ShellNode() {}
    ShellNode(const ShellNode &) = delete;
    ShellNode(ShellNode &&other) noexcept : d(std::move(other.d)) { }
    ShellNode &operator=(const ShellNode &) = delete;
    ShellNode &operator=(ShellNode &&other)             { swap(std::move(other)); return *this; }
    ~ShellNode() = default;

    void swap(ShellNode &&other) noexcept               { std::swap(d, other.d); }

    bool isValid() const noexcept                       { return d.data; }
    bool isDesktop() const noexcept                     { return !d.parent; }
    bool isEnumerated() const noexcept                  { return d.enumerated; }

    pointer_type parent() const noexcept                { return d.parent ? d.parent : nullptr; }
    bool hasChildren() const noexcept;
    QVector<pointer_type> children(); //NOT const

    void enumerate();

    const ShellNodeDataPointer data() const noexcept    { return d.data; }
    QByteArray key() const noexcept                     { return d.key; }
protected:
    ShellNode(ShellItemPointer item, pointer_type parent, ShellCache *cache = nullptr);
    ShellNode(const IdListPointer &id, pointer_type parent, ShellCache *cache = nullptr);

    void initFrom(ShellItemPointer item, const IdListPointer &id, pointer_type parent, ShellCache *cache);

private:
    struct LocalData {
        QByteArray key;
        pointer_type parent;
        ShellNodeDataPointer data;
        QVector<pointer_type> children;
        bool enumerated = false;
        ShellCache *cache = nullptr;
    };

    LocalData d;

    friend class ShellCache;
};

//! The preferred pointer type for `ShellNode`s.
//! \relates ShellNode
using ShellNodePointer = ShellNode::pointer_type;
//! The preferred container type for ShellNodes
//! \relates ShellNode
using ShellNodeContainer = QVector<ShellNodePointer>;

} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLNODE_H
