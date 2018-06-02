#include "shell.h"
#define STRICT_TYPED_ITEMIDS
#include <ShlObj.h>

namespace qe {
namespace windows {
namespace shell {

ShellItemPointer itemFromIdList(ITEMIDLIST_ABSOLUTE *id)
{
    if (!id)
        return nullptr;
    ShellItemPointer ret;
    ::SHCreateItemFromIDList(id, IID_PPV_ARGS(ret.addressOf()));
    return ret;
}

IdListPointer idListFromItem(ShellItemPointer item)
{
    if (!item)
        return nullptr;
    IdListPointer ret;
    ::SHGetIDListFromObject(item.asUnknown(), ret.addressOf());
    return ret;
}

UnknownPointer<IShellFolder2> desktopFolder()
{
    UnknownPointer<IShellFolder> desktopSf;
    ::SHGetDesktopFolder(desktopSf.addressOf());
    return desktopSf.queryInterface<IShellFolder2>();
}

ShellItemPointer desktopItem()
{
    ShellItemPointer ret;
    auto desktopSf = desktopFolder();
    auto hr = ::SHGetItemFromObject(desktopSf.asUnknown(), IID_PPV_ARGS(ret.addressOf()));
    Q_ASSERT(hr == S_OK && ret);
    return ret;
}

QString parsingFilePath(const ITEMIDLIST_ABSOLUTE *id)
{
    WCharPointer path;
    ::SHGetPathFromIDListEx(id, path.get(), 0, GPFIDL_DEFAULT);
    return QString::fromWCharArray(path.get());
}

namespace detail {

SFGAOF nodeFlagsToSfgao(NodeFlags flags)
{
    SFGAOF ret = 0x0;
    if (flags & NodeFlag::Folder)           ret |= SFGAO_FOLDER;
    if (flags & NodeFlag::FileSystem)       ret |= SFGAO_FILESYSTEM;
    if (flags & NodeFlag::StorageObject)    ret |= SFGAO_STORAGE;
    if (flags & NodeFlag::Stream)           ret |= SFGAO_STREAM;

    if (flags & NodeFlag::CanCopy)          ret |= SFGAO_CANCOPY;
    if (flags & NodeFlag::CanMove)          ret |= SFGAO_CANMOVE;
    if (flags & NodeFlag::CanRename)        ret |= SFGAO_CANRENAME;
    if (flags & NodeFlag::CanDelete)        ret |= SFGAO_CANDELETE;

    if (flags & NodeFlag::ReadOnly)         ret |= SFGAO_READONLY;
    if (flags & NodeFlag::Hidden)           ret |= SFGAO_HIDDEN;
    if (flags & NodeFlag::System)           ret |= SFGAO_SYSTEM;
    if (flags & NodeFlag::Ghosted)          ret |= SFGAO_GHOSTED;
    if (flags & NodeFlag::ShellLink)        ret |= SFGAO_LINK;
    if (flags & NodeFlag::Remote)           ret |= SFGAO_ISSLOW;
    if (flags & NodeFlag::Removable)        ret |= SFGAO_REMOVABLE;
    if (flags & NodeFlag::Compressed)       ret |= SFGAO_COMPRESSED;
    if (flags & NodeFlag::Encrypted)        ret |= SFGAO_ENCRYPTED;
    return ret;
}
NodeFlags sfgaoFlagsToNodeFlags(SFGAOF flags)
{
    NodeFlags ret = NodeFlag::NoFlags;
    if (flags & SFGAO_FOLDER)       ret |= NodeFlag::Folder;
    if (flags & SFGAO_FILESYSTEM)   ret |= NodeFlag::FileSystem;
    if (flags & SFGAO_STORAGE)      ret |= NodeFlag::StorageObject;
    if (flags & SFGAO_STREAM)       ret |= NodeFlag::Stream;

    if (flags & SFGAO_CANCOPY)      ret |= NodeFlag::CanCopy;
    if (flags & SFGAO_CANMOVE)      ret |= NodeFlag::CanMove;
    if (flags & SFGAO_CANRENAME)    ret |= NodeFlag::CanRename;
    if (flags & SFGAO_CANDELETE)    ret |= NodeFlag::CanDelete;

    if (flags & SFGAO_READONLY)     ret |= NodeFlag::ReadOnly;
    if (flags & SFGAO_HIDDEN)       ret |= NodeFlag::Hidden;
    if (flags & SFGAO_SYSTEM)       ret |= NodeFlag::System;
    if (flags & SFGAO_GHOSTED)      ret |= NodeFlag::Ghosted;
    if (flags & SFGAO_LINK)         ret |= NodeFlag::ShellLink;
    if (flags & SFGAO_ISSLOW)       ret |= NodeFlag::Remote;
    if (flags & SFGAO_REMOVABLE)    ret |= NodeFlag::Removable;
    if (flags & SFGAO_COMPRESSED)   ret |= NodeFlag::Compressed;
    if (flags & SFGAO_ENCRYPTED)    ret |= NodeFlag::Encrypted;
    return ret;
}

NodeFlags fileAttributeToNodeFlags(DWORD flags)
{
    NodeFlags ret = NodeFlag::NoFlags;
    if (flags & FILE_ATTRIBUTE_READONLY)    ret |= NodeFlag::ReadOnly;
    if (flags & FILE_ATTRIBUTE_HIDDEN)      ret |= NodeFlag::Hidden;
    if (flags & FILE_ATTRIBUTE_SYSTEM)      ret |= NodeFlag::System;
    if (flags & FILE_ATTRIBUTE_DIRECTORY)   ret |= NodeFlag::Folder;
    if (flags & FILE_ATTRIBUTE_COMPRESSED)  ret |= NodeFlag::Compressed;
    if (flags & FILE_ATTRIBUTE_ENCRYPTED)   ret |= NodeFlag::Encrypted;
    return ret;
}

}

unsigned int idListHash(ITEMIDLIST_ABSOLUTE *id)
{
    return qHashBits(static_cast<const void *>(id), ILGetSize(id));
}

// namespace detail
} // namespace shell
} // namespace windows
} // namespace qe
