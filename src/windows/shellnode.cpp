#include "shellnode.h"
#include <qewindows/shell.h>
#include <qewindows/shellnodedata.h>
#include <qewindows/shellcache.h>

namespace qe {
namespace windows {

bool ShellNode::hasChildren() const noexcept
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

//! \brief Constructs a new instance from a `ShellItemPointer` and its parent node.
//! Node that `parent` may be nullptr, indicating this is the desktop (root) node.
ShellNode::ShellNode(ShellItemPointer item, ShellNodePointer parent, ShellCache *cache)
{
    Q_ASSERT(item);
    if (!cache)
        d.cache = cache;
    else
        d.cache = ShellCache::globalInstance();
    d.key = ShellCache::keyFor(item.get());
    Q_ASSERT(!d.key.isNull());
    Q_ASSERT(!cache->contains(d.key));
    d.data = ShellNodeData::create(item);
    d.parent = parent;
    d.cache->insert(ShellNodePointer(this));
}

ShellNode::ShellNode(const IdListPointer &id, ShellNodePointer parent, ShellCache *cache)
{
    d.parent = parent;
    d.data = ShellNodeData::create(id);
}

} // namespace windows
} // namespace qe
