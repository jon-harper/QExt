#include "shellnode.h"
#include <qewindows/shell.h>
#include <qewindows/shellnodedata.h>
#include <qewindows/shellcache.h>

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
    Q_UNIMPLEMENTED();
    return;
}

//! \brief Constructs a new instance from a `ShellItemPointer` and its parent node.
//! Node that `parent` may be nullptr, indicating this is the desktop (root) node.
ShellNode::ShellNode(ShellItem2Pointer item, ShellNodePointer parent, QByteArray key)
{
    d.cache = ShellCache::globalInstance();
    d.key = key;
    d.data = ShellNodeData::create(item);
    d.parent = parent;
}

} // namespace windows
} // namespace qe
