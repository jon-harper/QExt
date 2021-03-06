#include "shellnodedata.h"
#include <propkey.h>

namespace qe {
namespace windows {

ShellNodeDataPointer ShellNodeData::create(const shell::IdList &ptr)
{
    ShellNodeData *ret = new ShellNodeData;
    ret->id = ptr;
    ret->item = shell::itemFromIdList(ret->id);
    if (ret->id && ret->item)
        ret->invalid = false;
    ret->refresh();
    return ShellNodeData::PointerType(ret);
}

ShellNodeDataPointer ShellNodeData::create(ShellItem2Pointer ptr)
{
    ShellNodeData *ret = new ShellNodeData;
    ret->item = ptr;
    ret->id = shell::idListFromItem(ret->item);
    if (ret->id && ret->item)
        ret->invalid = false;
    ret->refresh();
    return ShellNodeData::PointerType(ret);
}

void ShellNodeData::clear()
{
    invalid = true;
    item.reset();
    id.reset();
    displayName.clear();
    parsingName.clear();
    flags = shell::NodeFlag::NoFlags;
}

void ShellNodeData::refresh()
{
    if (!id || !item) {
        clear();
        return;
    }
    //Update the item's cached data first
    auto ctx = shell::createBindContext();
    item->Update(ctx.get());

    WCharPointer name;
    item->GetDisplayName(SIGDN_NORMALDISPLAY, name.addressOf());
    if (name)
        displayName = QString::fromWCharArray(name.get());
    else
        displayName.clear();

    name.reset();
    item->GetDisplayName(SIGDN_PARENTRELATIVEPARSING, name.addressOf());
    if (name)
        parsingName = QString::fromWCharArray(name.get());
    else
        parsingName.clear();

    SFGAOF outFlags;
    shell::NodeFlags inFlags = shell::NodeFlag::AllFlags;
    item->GetAttributes(shell::nodeFlagsToSfgao(inFlags), &outFlags);
    flags = shell::sfgaoFlagsToNodeFlags(outFlags);

    //For filesystem nodes, we grab the WIN32_FIND_DATA struct
    if (flags & shell::NodeFlag::FileSystem) {
        PROPVARIANT var;
        PropVariantInit(&var);
        item->GetProperty(PKEY_FindData, &var);
        if (var.caui.pElems) {
            auto data = reinterpret_cast<WIN32_FIND_DATA *>(var.caui.pElems);
            Q_ASSERT(data);
            flags |= shell::fileAttributeToNodeFlags(data->dwFileAttributes);

            if (data->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                flags |= shell::NodeFlag::ReparsePoint;
                if (data->dwReserved0 & IO_REPARSE_TAG_SYMLINK)
                    flags |= shell::NodeFlag::SymLink;
            }   else if (data->dwReserved0 & IO_REPARSE_TAG_MOUNT_POINT) {
                    flags |= shell::NodeFlag::Junction;
            }
        }
    }

    invalid = false;
}

} // namespace windows
} // namespace qe

