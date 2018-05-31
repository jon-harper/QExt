#include "shellnode.h"
#include <qewindows/shell.h>

namespace qe {
namespace windows {

static ShellNode::pointer_type rootNode = nullptr;

//! \brief Constructs a new instance from a `ShellItemPointer` and its parent node.
//! Node that `parent` may be nullptr, indicating this is the desktop (root) node.
ShellNode::ShellNode(ShellItemPointer item, ShellNode *parent)
{
    d.parent = parent;
    d.data = ShellNodeData::create(item);
}

ShellNode::ShellNode(const IdListPointer &id, ShellNode *parent)
{
    d.parent = parent;
    d.data = ShellNodeData::create(id);
}

ShellNode *ShellNode::desktopNode()
{
    if (rootNode)
        return rootNode.get();
    rootNode = makeUnique<ShellNode>(shell::desktopItem(), nullptr);
    return rootNode.get();
}

//ShellNode::ShellNode(ShellNodeData *root)
//    : d.data(root)
//{
//}


} // namespace windows
} // namespace qe
