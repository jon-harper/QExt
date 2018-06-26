#include "shell.h"

namespace qe {
namespace windows {
namespace shell {

//! Retrieves a `ShellItem2Pointer` for the given absolute id.
ShellItem2Pointer itemFromIdList(const IdList &id)
{
    ShellItem2Pointer ret;
    ::SHCreateItemFromIDList(id.castTo<PCIDLIST_ABSOLUTE>(), IID_PPV_ARGS(ret.addressOf()));
    return ret;
}

//! Retrieves the id list for a given ShellItem2Pointer.
IdList idListFromItem(ShellItem2Pointer item)
{
    if (!item)
        return nullptr;
    return idListFromUnknown(item.asUnknown());
}

//! Returns an IDListPointer, given any IUnknown that is compatible with `SHGetIDListFromObject`.
IdList idListFromUnknown(IUnknown *unk)
{
    IdList ret;
    ::SHGetIDListFromObject(unk, ret.castAddress<PIDLIST_ABSOLUTE *>());
    return ret;
}

ShellFolder2Pointer get_desktopFolder()
{
    UnknownPointer<IShellFolder> ret;
    ::SHGetDesktopFolder(ret.addressOf());
    return ret.queryInterface<IShellFolder2>();
}

//! Returns a pointer to the IShellFolder2 interface for the desktop.
ShellFolder2Pointer desktopFolder()
{
    thread_local UnknownPointer<IShellFolder2> ret = get_desktopFolder();
    return ret;
}

inline ShellItem2Pointer get_desktopItem()
{
    ShellItem2Pointer ret;
    auto desktopSf = desktopFolder();
    ::SHGetItemFromObject(desktopSf.asUnknown(), IID_PPV_ARGS(ret.addressOf()));
    Q_ASSERT(ret);
    return ret;
}

//! Retrieves a `ShellItemPointer` for the desktop.
ShellItem2Pointer desktopItem()
{
    static ShellItem2Pointer ret = get_desktopItem();
    return ret;
}

//! Calls `SHGetPathFromIdListEx` to get the parsing path for a given \arg id.
QString parsingFilePath(const ITEMIDLIST_ABSOLUTE *id)
{
    WCharPointer ret;
    ::SHGetPathFromIDListEx(id, ret.get(), 0, GPFIDL_DEFAULT);
    return QString::fromWCharArray(ret.get());
}

//! Convenience function that creates a new binding context.
UnknownPointer<IBindCtx> createBindContext()
{
    UnknownPointer<IBindCtx> ctx;
    CreateBindCtx(0, ctx.addressOf());
    return ctx;
}

//! Gets the id for a KNOWNFOLDERID. Note that this takes KNOWN_FOLDER_FLAG as an argument,
//! unlike `SHGetKnownFolderIDList`.
IdList knownFolderIdList(const KNOWNFOLDERID &id, KNOWN_FOLDER_FLAG flags, HANDLE token)
{
    IdList ret;
    ::SHGetKnownFolderIDList(id, static_cast<DWORD>(flags), token, ret.castAddress<PIDLIST_ABSOLUTE *>());
    return ret;
}

//! Gets a pointer to the IShellItem2 implementing a given known folder.
ShellItem2Pointer knownFolderItem(const KNOWNFOLDERID &id, KNOWN_FOLDER_FLAG flags, HANDLE token)
{
    ShellItem2Pointer ret;
    ::SHGetKnownFolderItem(id, flags, token, IID_PPV_ARGS(ret.addressOf()));
    return ret;
}

//! Helper function to translate to native format.
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
    if (flags & NodeFlag::MayHaveChildren)  ret |= SFGAO_HASSUBFOLDER;
    return ret;
}

//! Helper function to translate native flags to NodeFlags.
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
    if (flags & SFGAO_HASSUBFOLDER) ret |= NodeFlag::MayHaveChildren;
    return ret;
}

//! Helper function to translate FILE_ATTRIBUTE_* flags to NodeFlags.
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

ShellItem2Pointer itemParent(ShellItem2Pointer item)
{
    Q_ASSERT(item);
    ShellItemPointer parent;
    item->GetParent(parent.addressOf());
    if (!parent)
        return {};
    return item.queryInterface<IShellItem2>();
}

//! Compares a ShellItem2Pointer to a raw IShellItem pointer.
//! This function equates a null pointer to less than any valid pointer. When two null pointers are
//! passed they are equal.
//! If both pointers are valid, this function calls `IShellItem::Compare` with the
//! `SICHINT_CANONICAL` flag set.
int compareItems(IShellItem *lhs, IShellItem *rhs, SICHINTF flags)
{
    if (!lhs)
        return rhs ? 1 : 0;
    if (!rhs)
        return -1;
    int result = 0;
    lhs->Compare(rhs, static_cast<DWORD>(flags), &result);
    return result;
}

int compareItems(ShellItem2Pointer lhs, IShellItem *rhs, SICHINTF flags)
{
    if (!lhs)
        return -1;
    if (!rhs)
        return -1;
    int result = 0;
    lhs->Compare(rhs, static_cast<DWORD>(SICHINT_CANONICAL), &result);
    return result;
}

int compareItems(ShellItem2Pointer lhs, ShellItem2Pointer &rhs, SICHINTF flags)
{
    auto right = rhs.queryInterface<IShellItem>();
    return compareItems(lhs, right.data(), flags);
}



} // namespace shell
} // namespace windows
} // namespace qe
