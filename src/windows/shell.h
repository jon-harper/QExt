#ifndef QE_WINDOWS_SHELL_H
#define QE_WINDOWS_SHELL_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <qewindows/types.h>
#include <qewindows/idlist.h>
#include "shell_impl.h"

namespace qe {
namespace windows {

namespace shell {
Q_NAMESPACE;

namespace detail {

inline int compareItems(IShellItem *lhs, IShellItem *rhs, SICHINTF flags)
{
    if (!lhs)
        return -1;
    if (!rhs)
        return 1;
    int result = 0;
    if (SUCCEEDED(lhs->Compare(rhs, flags, &result)))
        return result;
    return -1;
}


inline int compareItems(IShellItem2 *&lhs, IShellItem *&rhs, SICHINTF flags)
{
    if (!lhs)
        return -1;
    if (!rhs)
        return 1;
    int result = 0;
    if (SUCCEEDED(lhs->Compare(rhs, flags, &result)))
        return result;
    return -1;
}


inline int compareItems(IShellItem *lhs, IShellItem2 *rhs, SICHINTF flags)
{
    return compareItems(rhs, lhs, flags);
}

}

QE_WINDOWS_EXPORT int compareItems(ShellItem2Pointer lhs, IShellItem *rhs,
                                   SICHINTF flags = SICHINT_CANONICAL);
QE_WINDOWS_EXPORT int compareItems(ShellItem2Pointer lhs, ShellItem2Pointer &rhs,
                                   SICHINTF flags = SICHINT_CANONICAL);
QE_WINDOWS_EXPORT int compareItems(IShellItem *lhs, IShellItem *rhs,
                                   SICHINTF flags = SICHINT_CANONICAL);

QE_WINDOWS_EXPORT ShellItem2Pointer itemFromIdList(const IdList &id);
QE_WINDOWS_EXPORT IdList idListFromItem(ShellItem2Pointer item);
QE_WINDOWS_EXPORT IdList idListFromUnknown(IUnknown *unk);

QE_WINDOWS_EXPORT ShellFolder2Pointer desktopFolder();
QE_WINDOWS_EXPORT ShellItem2Pointer desktopItem();

QE_WINDOWS_EXPORT QString parsingFilePath(const ITEMIDLIST_ABSOLUTE *id);

QE_WINDOWS_EXPORT ShellItem2Pointer itemParent(ShellItem2Pointer item);
QE_WINDOWS_EXPORT UnknownPointer<IBindCtx> createBindContext();

QE_WINDOWS_EXPORT IdList knownFolderIdList(const KNOWNFOLDERID &id,
                                           KNOWN_FOLDER_FLAG flags = KF_FLAG_NO_ALIAS,
                                           HANDLE token = nullptr);
QE_WINDOWS_EXPORT ShellItem2Pointer knownFolderItem(const KNOWNFOLDERID &id,
                                                    KNOWN_FOLDER_FLAG flags = KF_FLAG_NO_ALIAS,
                                                    HANDLE token = nullptr);

//! This function calls `IShellItem::BindToHandler` based on a predefined set of known BHID_ values.
//! For calls using `BHID_SFObject`, use `bindToObject` instead. For any other situation, you will
//! have to call `IShellItem::BindToHandler` directly.
template <class T, class = std::void_t<decltype(bindingGuid<T>())>>
inline UnknownPointer<T> bindItem(ShellItem2Pointer &item, UnknownPointer<IBindCtx> ctx = createBindContext())
{
    const GUID bhid = bindingGuid<T>();
    UnknownPointer<T> ret;
    item->BindToHandler(ctx.data(), bhid, IID_PPV_ARGS(ret.addressOf()));
    return ret;
}

//! This function calls `IShellItem::BindToHandler` with `BHID_SFObject` as the `rbhid` object.
template <class T>
inline UnknownPointer<T> bindItemToObject(ShellItem2Pointer &item, UnknownPointer<IBindCtx> ctx = createBindContext())
{
    UnknownPointer<T> ret;
    item->BindToHandler(ctx.data(), BHID_SFObject, IID_PPV_ARGS(ret.addressOf()));
    return ret;
}

//! Flags with information about a node's type and capabilities.
enum class NodeFlag : quint32 {
    //! No Flags
    NoFlags         = 0x00000000,
    //! SFGAO_FOLDER  - Binds IID_IShellFolder
    Folder          = 0x00000001,
    //! SFGAO_FILESYSTEM
    FileSystem      = 0x00000002,
    //! Shell library
    Library         = 0x00000004,
    //! SFGAO_STORAGE - Binds IID_Storage
    StorageObject   = 0x00000008,
    //! SFGAO_STREAM  - Binds IID_Stream
    Stream          = 0x00000010,
    //! FILE_ATTRIBUTE_REPARSE_POINT - May be a mount point/junction or symlink.
    ReparsePoint    = 0x00000020,
    //! Any node that QFileInfo cannot handle, i.e. not on the filesystem or below file level.
    Virtual         = 0x00000040,

    ObjectTypeMask  = 0x000000FF,

    //! FILE_ATTRIBUTE_REPARSE_POINT - A mount point for a device
    MountPoint      = 0x00000100,
    //! FILE_ATTRIBUTE_REPARSE_POINT - A junction that is \em not a mount point.
    Junction        = 0x00000200,
    //! FILE_ATTRIBUTE_REPARSE_POINT - A symlink.
    SymLink         = 0x00000400,
    //! SFGAO_LINK - A node implementing `IShellLink`.
    ShellLink       = 0x00000800,

    LinkMask        = 0x00000F00,

    //! SFGAO_CANCOPY
    CanCopy         = 0x00001000,
    //! SFGAO_CANMOVE
    CanMove         = 0x00002000,
    //! SFGAO_CANRENAME
    CanRename       = 0x00004000,
    //! SFGAO_CANDELETE
    CanDelete       = 0x00008000,
    //! SFGAO_READONLY, FILE_ATTRIBUTE_READONLY
    ReadOnly        = 0x00010000,
    //! SFGAO_HIDDEN, FILE_ATTRIBUTE_HIDDEN
    Hidden          = 0x00020000,
    //! SFGAO_SYSTEM
    System          = 0x00040000,
    //! SFGAO_GHOSTED
    Ghosted         = 0x00080000,

    //! SFGAO_ISSLOW
    Remote          = 0x00100000,
    //! SFGAO_REMOVABLE
    Removable       = 0x00200000,
    //! SFGAO_COMPRESSED, FILE_ATTRIBUTE_COMPRESSED
    Compressed      = 0x00400000,
    //! FILE_ATTRIBUTE_ENCRYPTED
    Encrypted       = 0x00800000,

    AccessFlagMask  = 0x0FFF0000,

    //! SFGAO_HASSUBFOLDER
    MayHaveChildren = 0x10000000,

    AllFlags        = 0x1FFFFFFF
};

Q_DECLARE_FLAGS(NodeFlags, NodeFlag);
Q_FLAG_NS(NodeFlag);

QE_WINDOWS_EXPORT NodeFlags fileAttributeToNodeFlags(DWORD flags);
QE_WINDOWS_EXPORT SFGAOF nodeFlagsToSfgao(NodeFlags flags);
QE_WINDOWS_EXPORT NodeFlags sfgaoFlagsToNodeFlags(SFGAOF flags);

} // namespace shell
} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELL_H
