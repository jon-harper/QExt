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

    //! Deleted default constructor.
    ShellNode() = delete;
    //! Deleted copy constructor.
    ShellNode(const ShellNode &) = delete;
    //! Move constructs a node instance.
    ShellNode(ShellNode &&other) noexcept : d(std::move(other.d)) { }
    //! Deleted copy assignment operator.
    ShellNode &operator=(const ShellNode &) = delete;
    //! Move assigns from other.
    ShellNode &operator=(ShellNode &&other)             { swap(std::move(other)); return *this; }
    //! Defaulted destructor.
    ~ShellNode() = default;

    //! Swaps two instances.
    void swap(ShellNode &&other) noexcept               { std::swap(d, other.d); }

    //! Returns a shared pointer to this node.
    PointerType pointer()                               { return sharedFromThis(); }
    static PointerType rootNode();

    //! Returns true if the node was successfully constructed and has valid data.
    bool isValid() const noexcept                       { return d.cachedData; }
    //! Returns true if this is the root, i.e. desktop node.
    bool isRoot() const noexcept                        { return !d.parent; }
    //! Returns true if the children of this node are enumerated.
    bool isEnumerated() const noexcept                  { return d.enumerated; }

    //! Returns a pointer to the parent node, or an invalid pointer if this is the root node.
    PointerType parent() const noexcept                 { return d.parent ? d.parent : nullptr; }
    bool hasChildren() const noexcept;
    //! Returns the number of known children of this node. Call enumerate to ensure that this value is correct.
    int childCount() const noexcept                     { return d.children.count(); }
    int childIndex(PointerType node) const noexcept     { return d.children.indexOf(node); }
    PointerType childAt(int index) const noexcept       { return d.children.at(index); }
    QVector<PointerType> children(); //NOT const, may enumerate

    void enumerate();

    //! Returns a `const` pointer to `const` of the stored ShellNodeDataPointer.
    const ShellNodeDataPointer data() const noexcept    { return d.cachedData; }
    //! Returns a shared pointer to the stored IShellItem2 instance.
    ShellItem2Pointer itemPointer() const noexcept      { return d.cachedData->item; }
    //! Returns a copy of the stored IdList.
    shell::IdList idList() const noexcept               { return d.cachedData->id;}
    //! Retrieves a QFileInfo if possible, falling back to a default-constructed instance if not.
    QFileInfo fileInfo() const noexcept;
    //! Gets the (child) parsing name for the node.
    QString parsingName() const noexcept                { return d.cachedData->parsingName; }
    //! Gets the display name of the node.
    QString displayName() const noexcept                { return d.cachedData->displayName; }

    template <class T>
    inline UnknownPointer<T> bindTo(UnknownPointer<IBindCtx> ctx = shell::createBindContext());

    template <class T>
    inline UnknownPointer<T> bindToObject(UnknownPointer<IBindCtx> ctx = shell::createBindContext());

protected:
    ShellNode(ShellNodeDataPointer data, PointerType parent);
    PointerType createChild(IShellItem *child);

private:
    static ShellNode * get_rootNode();

    //! \internal
    struct LocalData {
        PointerType parent;
        ShellItem2Pointer item;
        ShellNodeDataPointer cachedData;
        QVector<PointerType> children;
        bool enumerated = false;
    };

    //! \internal
    LocalData d;
};

//! The preferred pointer type for `ShellNode`s.
//! \relates ShellNode
using ShellNodePointer = ShellNode::PointerType;
//! The preferred container type for ShellNodes
//! \relates ShellNode
using ShellNodeContainer = QVector<ShellNodePointer>;

//! This function calls `IShellItem::BindToHandler` based on a predefined set of known BHID_ values.
//! For calls using `BHID_SFObject`, use `bindToObject` instead. For any other situation, you will
//! have to call `IShellItem::BindToHandler` directly.
//!
//! Supportd types:
//! * `IStream`
//! * `IStorage`
//! * `IEnumShellItems`
//! * `ITransferSource`
//! * `ITransferDestination`
//! * `IPropertyStore`
//! * `IPropertyStoreFactory`
//! * `IExtractImage`
//! * `IThumbnailProvider`
//! * `IDataObject`
//! * `IQueryAssociation`
//! * `IEnumAssocHandlers`
//! * `IFilter`
template<class T>
UnknownPointer<T> ShellNode::bindTo(UnknownPointer<IBindCtx> ctx)
{
    return shell::bindItem<T>(d.cachedData->item, ctx);
}

//! This function calls `IShellItem::BindToHandler` with `BHID_SFObject` as the `rbhid` object.
template<class T>
UnknownPointer<T> ShellNode::bindToObject(UnknownPointer<IBindCtx> ctx)
{
    return shell::bindItemToObject<T>(d.cachedData->item, ctx);
}

namespace shell {

//! Retrieves a pointer to an IShellItem from the given node.
//! \related qe::windows::ShellNode
inline ShellItemPointer itemPointer(ShellNode &node)
{
    auto ret = node.itemPointer();
    return ret.queryInterface<IShellItem>();
}

//! Retrieves a pointer to an IShellItem from the given node pointer.
//! \related qe::windows::ShellNode
inline ShellItemPointer itemPointer(ShellNodePointer ptr)
{
    return itemPointer(*ptr);
}

} // namespace shell
} // namespace windows
} // namespace qe

inline bool operator ==(qe::windows::ShellNode &lhs, qe::windows::ShellNode &rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;
    return qe::windows::shell::compareItems(lhs.itemPointer().data(),
                                            rhs.itemPointer().data(),
                                            SICHINT_CANONICAL) == 0;
}

inline bool operator ==(qe::windows::ShellNodePointer lhs, qe::windows::ShellNodePointer rhs)
{
    return (lhs.data() == rhs.data());
}

inline bool operator !=(qe::windows::ShellNode &lhs, qe::windows::ShellNode &rhs)
{
    return !(lhs == rhs);
}

inline bool operator !=(qe::windows::ShellNodePointer lhs, qe::windows::ShellNodePointer rhs)
{
    return !(lhs.data() == rhs.data());
}

inline bool operator <(qe::windows::ShellNode &lhs, qe::windows::ShellNode &rhs)
{
    if (!lhs.isValid() || !rhs.isValid())
        return false;
    return qe::windows::shell::compareItems(lhs.itemPointer().data(),
                                            rhs.itemPointer().data(),
                                            SICHINT_CANONICAL);
}

inline bool operator <(qe::windows::ShellNodePointer lhs, qe::windows::ShellNodePointer rhs)
{
    return *lhs < *rhs;
}

inline bool operator >(qe::windows::ShellNode &lhs, qe::windows::ShellNode &rhs)
{
    return rhs < lhs;
}

inline bool operator >(qe::windows::ShellNodePointer lhs, qe::windows::ShellNodePointer rhs)
{
    return *rhs < *lhs;
}

inline bool operator <=(qe::windows::ShellNode &lhs, qe::windows::ShellNode &rhs)
{
    return !(lhs == rhs) || (lhs < rhs);
}

inline bool operator <=(qe::windows::ShellNodePointer lhs, qe::windows::ShellNodePointer rhs)
{
    return *rhs <= *lhs;
}

inline bool operator >=(qe::windows::ShellNode &lhs, qe::windows::ShellNode &rhs)
{
    return !(lhs < rhs);
}

inline bool operator >=(qe::windows::ShellNodePointer lhs, qe::windows::ShellNodePointer rhs)
{
    return !(lhs < rhs);
}
#endif // QE_WINDOWS_SHELLNODE_H
