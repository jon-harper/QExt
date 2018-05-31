#include "shellnodedata.h"
#include <propkey.h>
#include <shell.h>

namespace qe {
namespace windows {

ShellNodeDataPointer ShellNodeData::create(const IdListPointer &ptr)
{
    ShellNodeData *ret = new ShellNodeData;
    ret->id = ptr;
    ret->iface = shell::itemFromIdList(ret->id.get());
    if (ret->id && ret->iface)
        ret->invalid = false;
    return ShellNodeData::pointer_type(ret);
}

ShellNodeDataPointer ShellNodeData::create(ShellItemPointer ptr)
{
    ShellNodeData *ret = new ShellNodeData;
    ret->iface = ptr;
    ret->id = shell::idListFromItem(ret->iface);
    if (ret->id && ret->iface)
        ret->invalid = false;
    return ShellNodeData::pointer_type(ret);
}

void ShellNodeData::refresh()
{
    if (!id || !iface) {
        invalid = true;
        return;
    }
    {
        UnknownPointer<IBindCtx> ctx;
        CreateBindCtx(0, ctx.addressOf());
        iface->Update(ctx.get());
    }

    WCharPointer name;
    iface->GetDisplayName(SIGDN_NORMALDISPLAY, name.addressOf());
    if (name)
        displayName = QString::fromWCharArray(name.get());
    else
        displayName.clear();

    name.reset();
    iface->GetDisplayName(SIGDN_PARENTRELATIVEPARSING, name.addressOf());
    if (name)
        parsingName = QString::fromWCharArray(name.get());
    else
        parsingName.clear();

    SFGAOF outFlags;
    shell::NodeFlags inFlags = shell::NodeFlag::ObjectTypeMask;
    inFlags |= shell::NodeFlag::AccessFlagMask;
    inFlags |= shell::NodeFlag::LinkMask;
    iface->GetAttributes(shell::detail::nodeFlagsToSfgao(inFlags), &outFlags);
    flags = shell::detail::sfgaoFlagsToNodeFlags(outFlags);

    if (flags & shell::NodeFlag::FileSystem) {
        PROPVARIANT var;
        PropVariantInit(&var);
        iface->GetProperty(PKEY_FindData, &var);
        if (var.caui.pElems) {
            auto data = reinterpret_cast<WIN32_FIND_DATA *>(var.caui.pElems);
            flags |= shell::detail::fileAttributeToNodeFlags(data->dwFileAttributes);

            if (data->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
                if (data->dwReserved0 & IO_REPARSE_TAG_SYMLINK)
                    flags |= shell::NodeFlag::SymLink;
            }   else if (data->dwReserved0 & IO_REPARSE_TAG_MOUNT_POINT) {
                //TODO: check for junctions and mount points
            }
        }
    }

    invalid = false;
}

} // namespace windows
} // namespace qe

