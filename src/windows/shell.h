#ifndef QE_WINDOWS_SHELL_H
#define QE_WINDOWS_SHELL_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <qewindows/types.h>
#include <qewindows/idlist.h>
#include "shell_impl.h"

namespace qe {
namespace windows {

//! \namespace qe::windows::shell
//!
//! \brief The shell namespace contains functions, enumerations, and classes that interact with the Windows
//! Desktop Shell.
//!
//! The most basic type defined in this namespace is the IdList, a wrapper around an `ITEMIDLIST`
//! pointer (a `pidl`). Like a pidl, an IdList identifies a location in the shell namespace. In
//! QExt, however, an IdList is *always* absolute (relative to the desktop), but has accessor functions
//! to grab the child ID. There is also IdList::const_iterator, a convenient method of comparing two
//! separate pidls. IdLists have the advantage of not storing an interface pointer, and can thus be
//! freely copied and sent between threads.
//!
//! ShellItem2Pointer is a wrapper around an IShellItem2 interface. Together with IdList, this forms
//! the basis of a Node, the interface for interacting with a specific location in the namespace.
//! ShellItem2Pointer (and any Windows Desktop Shell interface) cannot be marshalled, and thus cannot
//! pass between threads. Since a Node stores a ShellItem2Pointer, *it cannot be passed between threads*.
//!
//! Much of the *data* that a Node instance caches can be passed between threads, specifically for
//! this reason. NodeData is a class that stores an IdList and any cached data for a Node. Worker
//! threads can be sent an IdList, construct the appropriate interface from it, gather what data they
//! need and store it in a NodeData instance. Once the processing is done, that NodeData can then
//! be returned.
//!
//! NodeInfo is the Shell namespace equivalent of `QFileInfo`. It acts like `QFileInfo` with the
//! addition of information specific to a Node, such as the GUID identifying its type, whether it is
//! virtual or not, etc. NodeInfo is also backed by NodeData.

namespace shell {
Q_NAMESPACE;

//Instance Getters
QE_WINDOWS_EXPORT ShellFolder2Pointer desktopFolder();
QE_WINDOWS_EXPORT ShellItem2Pointer desktopItem();
QE_WINDOWS_EXPORT IdList knownFolderIdList(const KNOWNFOLDERID &id,
                                           KNOWN_FOLDER_FLAG flags = KF_FLAG_NO_ALIAS,
                                           HANDLE token = nullptr);
QE_WINDOWS_EXPORT ShellItem2Pointer knownFolderItem(const KNOWNFOLDERID &id,
                                                    KNOWN_FOLDER_FLAG flags = KF_FLAG_NO_ALIAS,
                                                    HANDLE token = nullptr);

//equality
QE_WINDOWS_EXPORT int compareItems(IShellItem *lhs, IShellItem *rhs,
                                   SICHINTF flags = SICHINT_CANONICAL);
//QE_WINDOWS_EXPORT int compareItems(ShellItem2Pointer lhs, ShellItem2Pointer rhs,
//                                   SICHINTF flags = SICHINT_CANONICAL);

/////
// Shell Property Getters
//

//IShellItem2 Getters
QE_WINDOWS_EXPORT ShellItem2Pointer itemFromIdList(const IdList &id);
//QE_WINDOWS_EXPORT ShellItem2Pointer itemFromParsingName(QString name);

//IdList
QE_WINDOWS_EXPORT IdList idListFromItem(ShellItem2Pointer item);
QE_WINDOWS_EXPORT IdList idListFromUnknown(IUnknown *unk);
//QE_WINDOWS_EXPORT IdList idListFromParsingname(QString parsingName);

//Parsing Name
QE_WINDOWS_EXPORT QString parsingPathName(const IdList &id);
QE_WINDOWS_EXPORT QString parsingPathName(ShellItem2Pointer item);
//QE_WINDOWS_EXPORT QString parsingPath(const IdList &id);
//QE_WINDOWS_EXPORT QString parsingPath(ShellItem2Pointer item);
//QE_WINDOWS_EXPORT QString parsingName(const IdList &id);
//QE_WINDOWS_EXPORT QString parsingName(ShellItem2Pointer item);

//Parent
QE_WINDOWS_EXPORT ShellItem2Pointer parent(ShellItem2Pointer item);
//QE_WINDOWS_EXPORT IdList parent(const IdList &id);

QE_WINDOWS_EXPORT UnknownPointer<IBindCtx> createBindContext();

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
