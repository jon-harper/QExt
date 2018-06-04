#include "shellnode.h"
#include <qewindows/shell.h>

namespace qe {
namespace windows {

//! \brief Constructs a new instance from a `ShellItemPointer` and its parent node.
//! Node that `parent` may be nullptr, indicating this is the desktop (root) node.
ShellNode::ShellNode(ShellItemPointer item, ShellNodePointer parent)
{
    d.parent = parent;
    d.data = ShellNodeData::create(item);
}

ShellNode::ShellNode(const IdListPointer &id, ShellNodePointer parent)
{
    d.parent = parent;
    d.data = ShellNodeData::create(id);
}

ShellNodePointer ShellNode::parent() const noexcept
{
    return d.parent ? d.parent : nullptr;
}

bool ShellNode::hasChildren() const
{
    if (!isValid())
        return false;
    if (!isEnumerated())
        return d.data->flags & shell::NodeFlag::MayHaveChildren;
    return !d.children.isEmpty();
}

QVector<ShellNodePointer> ShellNode::children()
{
    if (!isValid())
        return {};
    if (d.enumerated)
        return d.children;
    enumerate();
    return d.children;
}

void ShellNode::enumerate()
{
    if (!isValid())
        return;
    if (!(d.data->flags & shell::NodeFlag::MayHaveChildren)) {
        d.enumerated = true;
        return;
    }
    auto sf = d.data->item.queryInterface<IShellFolder>();

}

} // namespace windows
} // namespace qe
