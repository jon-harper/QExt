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

} // namespace windows
} // namespace qe
