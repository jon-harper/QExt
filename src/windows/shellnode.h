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
#include <QtCore/QEnableSharedFromThis>
#include <QtCore/QFileInfo>
#include <qewindows/types.h>
#include <qewindows/shell.h>
#include <qewindows/shellnodedata.h>

namespace qe {
namespace windows {

//! Represents a location in the shell namespace.
//!
//! A location is typically identified by a PIDL, IShellItem/IShellItem2, IShellFolder/IShellFolder2,
//! or a parsing name (as an LPCWSTR). The name "node" was chosen explicitly to not resemble any of
//! the above names, as this class provides a single starting point for access to a location in the
//! shell namespace. Ideally, one may obtain the same data with significantly less boilerplate.
//!
//! \warning This class is *not* thread safe. ShellNode instances may only exist in one thread.
//! \warning Access to COM interfaces for the Shell namespace cannot be performed from an MTA thread.
//! You must initialize COM as a Single-Threaded Apartment for the main thread.
class QE_WINDOWS_EXPORT ShellNode : public QEnableSharedFromThis<ShellNode>
{
public:
    using PointerType = QSharedPointer<ShellNode>;

    ShellNode() = delete;
    ShellNode(const ShellNode &) = delete;
    ShellNode(ShellNode &&other) noexcept : d(std::move(other.d)) { }
    ShellNode &operator=(const ShellNode &) = delete;
    ShellNode &operator=(ShellNode &&other)             { swap(std::move(other)); return *this; }
    ~ShellNode() = default;

    void swap(ShellNode &&other) noexcept               { std::swap(d, other.d); }

    PointerType pointer()                               { return sharedFromThis(); }
    static PointerType rootNode();

    bool isValid() const noexcept                       { return d.data; }
    bool isRoot() const noexcept                        { return !d.parent; }
    bool isEnumerated() const noexcept                  { return d.enumerated; }

    PointerType parent() const noexcept                 { return d.parent ? d.parent : nullptr; }
    bool hasChildren() const noexcept;
    QVector<PointerType> children(); //NOT const, may enumerate

    void enumerate();

    const ShellNodeDataPointer data() const noexcept    { return d.data; }
    ShellItem2Pointer itemPointer() const noexcept      { return d.data->item; }
    shell::IdList idList() const noexcept               { return d.data->id;}
    QFileInfo fileInfo() const noexcept;

    template <class T>
    inline UnknownPointer<T> bindTo(UnknownPointer<IBindCtx> ctx = shell::createBindContext()) const;

    template <class T>
    inline UnknownPointer<T> bindToObject(UnknownPointer<IBindCtx> ctx = shell::createBindContext()) const;
protected:
    ShellNode(ShellNodeDataPointer data, PointerType parent) /*noexcept*/;
    PointerType createChild(IShellItem *child);

private:
    static ShellNode * get_rootNode();

    struct LocalData {
        PointerType parent;
        ShellNodeDataPointer data;
        QVector<PointerType> children;
        bool enumerated = false;
    };

    LocalData d;
};

//! This function calls `IShellItem::BindToHandler` based on a predefined set of known BHID_ values.
//! For calls using `BHID_SFObject`, use `bindToObject` instead. For any other situation, you will
//! have to call `IShellItem::BindToHandler` directly.
template<class T>
UnknownPointer<T> ShellNode::bindTo(UnknownPointer<IBindCtx> ctx) const
{
    return shell::bindItem<T>(d.data->item, ctx);
}

//! This function calls `IShellItem::BindToHandler` with `BHID_SFObject` as the `rbhid` object.
template<class T>
UnknownPointer<T> ShellNode::bindToObject(UnknownPointer<IBindCtx> ctx) const
{
    return shell::bindItemToObject<T>(d.data->item, ctx);
}

//! The preferred pointer type for `ShellNode`s.
//! \relates ShellNode
using ShellNodePointer = ShellNode::PointerType;
//! The preferred container type for ShellNodes
//! \relates ShellNode
using ShellNodeContainer = QVector<ShellNodePointer>;

} // namespace windows
} // namespace qe

inline bool operator==(IShellItem *lhs, const qe::windows::ShellNodeDataPointer &rhs) noexcept
{
    return qe::windows::shell::compareItems(rhs->item, lhs);
}

inline bool operator==(IShellItem *lhs, const qe::windows::ShellNodePointer &rhs) noexcept
{
    return qe::windows::shell::compareItems(rhs->data()->item, lhs);
}

//template <class T>
//inline bool operator!=(const T &lhs, const qe::windows::ShellNode &rhs)
//{
//    return !(rhs == lhs);
//}

//template <class T, class = std::enable_if<std::is_same_v<T, qe::windows::ShellNode>
//                                       || std::is_same_v<T, qe::windows::ShellNodeData>>>
//inline bool operator!=(const T &lhs, IShellItem *rhs) noexcept
//{
//    return !(rhs == lhs);
//}
#endif // QE_WINDOWS_SHELLNODE_H
