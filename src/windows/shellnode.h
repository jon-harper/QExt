#ifndef QE_WINDOWS_SHELLNODE_H
#define QE_WINDOWS_SHELLNODE_H

#include <qewindows/shellnodedata.h>
#include <QtCore/QVector>

namespace qe {
namespace windows {

class ShellNode
{
public:
    using pointer_type = UniquePointer<ShellNode>;

    ShellNode(ShellItemPointer item, ShellNode *parent);
    ShellNode(const IdListPointer &id, ShellNode *parent);
    ShellNode(const ShellNode &) = delete;
    ShellNode(ShellNode &&other) noexcept { swap(std::move(other)); }
    ShellNode &operator=(const ShellNode &) = delete;
    ShellNode &operator=(ShellNode &&other) { swap(std::move(other)); return *this; }
    ~ShellNode() = default;

    void swap(ShellNode &&other) noexcept { std::swap(d, other.d); }

    static ShellNode *desktopNode();

private:
//    ShellNode(ShellNodeData *desktop);

    struct LocalData {
        ShellNode *parent = nullptr;
        ShellNodeData::pointer_type data;
        QVector<ShellNode::pointer_type> children;
        bool enumerated = false;
    };

    LocalData d;
};

} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLNODE_H
