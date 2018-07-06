#include "shellnode.h"
#include <qewindows/shell.h>
#include <qewindows/shellnodedata.h>

namespace qe {
namespace windows {

//! Helper function that initializes the root node.
//! \internal
ShellNode *ShellNode::get_rootNode()
{
    auto item = shell::desktopItem();
    auto node = new ShellNode(ShellNodeData::create(item), nullptr);
    Q_ASSERT(node);
    return node;
}

//! Returns the desktop node.
ShellNode::PointerType ShellNode::rootNode()
{
    thread_local ShellNodePointer rootNode(ShellNode::get_rootNode());
    return rootNode;
}

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
        return d.cachedData->flags & shell::NodeFlag::MayHaveChildren;
    return !d.children.isEmpty();
}

//! Gets a copy of the enumerated children of the node.
//! \note This function is not `const` and is potentially expensive, as it may call `enumerate()`.
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
    if (!(d.cachedData->flags & shell::NodeFlag::MayHaveChildren)) {
        d.enumerated = true;
        return;
    }

    auto items = bindTo<IEnumShellItems>();
    Q_ASSERT(items);
    IShellItem *ptr = nullptr;
    auto hr = S_OK;
    while (hr == S_OK) {
        //iterate over an individual item
        hr = items->Next(1, &ptr, nullptr);
        if (!SUCCEEDED(hr))
            break;
        bool found = false;
        if (d.children.count()) {
            auto iter = d.children.cbegin();
            auto iter_end = d.children.cend();
            //search and see if this node exists already
            for(; iter != iter_end; ++iter) {
                auto node = *iter;
                if (node && !shell::compareItems(node->itemPointer().data(), ptr)) {
                    found = true;
                    break;
                }
            }
        }
        if (!found && ptr) {
            auto child = createChild(ptr);
            if (child)
                d.children.append(child);
        }
    }
    d.enumerated = true;
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

//! Creates a new child from the given IShellItem instance.
ShellNode::PointerType ShellNode::createChild(IShellItem *child)
{
    ShellItem2Pointer item;
    child->QueryInterface(IID_PPV_ARGS(item.addressOf()));
    if (!item)
        return {};
    auto data = ShellNodeData::create(item);
    auto ret = new ShellNode(data, pointer());
    return ShellNodePointer(ret);
}

//! \brief Constructs a new instance from a `ShellNodeDataPointer` and its parent node.
//! Node that `parent` may be nullptr, indicating this is the desktop (root) node.
ShellNode::ShellNode(ShellNodeDataPointer data, ShellNodePointer parent) /*noexcept*/
{
    d.cachedData = data;
    d.parent = parent;
}

} // namespace windows
} // namespace qe
