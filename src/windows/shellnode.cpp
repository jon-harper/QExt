#include "shellnode.h"
#include <qewindows/shell.h>
#include <qewindows/shellnodedata.h>

namespace qe {
namespace windows {

//! Returns whether or not the node has children. This should be considered advisory until the node
//! is enumerated.
//! If the node is \em not enumerated and the \ref shell::NodeFlag::MayHaveChildren flag is set,
//! this will return true.
//! If the node is not valid, this function returns false.
bool ShellNode::hasChildren() const noexcept
{
    if (!isValid())
        return false;
    if (!isEnumerated())
        return d.data->flags & shell::NodeFlag::MayHaveChildren;
    return !d.children.isEmpty();
}

//! Gets a copy of the enumerated children of the node.
QVector<ShellNodePointer> ShellNode::children()
{
    if (!isValid())
        return {};
    if (d.enumerated)
        return d.children;
    enumerate();
    return d.children;
}

//! Forces the node to enumerate its children. If the data is remote, this may be costly.
void ShellNode::enumerate()
{
    if (!isValid())
        return;
    if (!(d.data->flags & shell::NodeFlag::MayHaveChildren)) {
        d.enumerated = true;
        return;
    }

    //auto cache = ShellCache::globalInstance();
    auto items = shell::bindTo<IEnumShellItems>(d.data->item);
    Q_ASSERT(items);
    IShellItem *ptr = nullptr;
    auto hr = S_OK;
    while (hr == S_OK) {
        hr = items->Next(1, &ptr, nullptr);
        if (!SUCCEEDED(hr))
            break;
//        IShellItem2 *ptr2 = nullptr;
//        if (!SUCCEEDED(ptr->QueryInterface(IID_PPV_ARGS(&ptr2))))
//            break;
        bool found = false;
        auto iter_end = d.children.cend();
        auto iter = d.children.cbegin();
        for(; iter != iter_end; ++iter) {
            auto node = *iter;
            if (!shell::compareItems(node->item(), ptr)) {
                found = true;
                break;
            }
        }
        if (!found) {
            auto child = createChild(ptr);
            d.children.append(child);
        }
    }
}

//! Returns a `QFileInfo` representing the node if one can be constructed. Otherwise, returns a
//! default-constructed `QFileInfo`.
QFileInfo ShellNode::fileInfo() const noexcept
{
    if (!isValid())
        return {};
    Q_UNIMPLEMENTED();
    return {};
}

ShellNode::PointerType ShellNode::createChild(IShellItem *child)
{
    ShellItem2Pointer item;
    child->QueryInterface(IID_PPV_ARGS(item.addressOf()));
    if (!item)
        return {};
//    auto ret = ShellNodeBuilder(item).setParent(pointer()).build();
//    Q_ASSERT(ShellCache::globalInstance()->insert(ret));
    auto data = ShellNodeData::create(item);
    auto ret = ShellNode(data, pointer());
    return ret.pointer();
}

//! \brief Constructs a new instance from a `ShellNodeDataPointer` and its parent node.
//! Node that `parent` may be nullptr, indicating this is the desktop (root) node.
ShellNode::ShellNode(ShellNodeDataPointer data, ShellNodePointer parent) /*noexcept*/
{
    d.data = data;
    d.parent = parent;
}

} // namespace windows
} // namespace qe
