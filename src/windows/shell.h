#ifndef QE_WINDOWS_SHELL_H
#define QE_WINDOWS_SHELL_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <qewindows/types.h>

namespace qe {
namespace windows {

namespace shell {
Q_NAMESPACE;

QE_WINDOWS_EXPORT ShellItemPointer itemFromIdList(ITEMIDLIST_ABSOLUTE *id);
QE_WINDOWS_EXPORT UnknownPointer<IShellFolder2> desktopFolder();
QE_WINDOWS_EXPORT IdListPointer idListFromItem(ShellItemPointer item);
QE_WINDOWS_EXPORT IdListPointer idListFromUnknown(IUnknown *unk);
QE_WINDOWS_EXPORT ShellItemPointer desktopItem();
QE_WINDOWS_EXPORT QString parsingFilePath(const ITEMIDLIST_ABSOLUTE *id);

//! Flags with information about a node's type and capabilities.
enum class NodeFlag : quint32 {
    NoFlags         = 0x00000000,   //! No Flags
    Folder          = 0x00000001,   //! SFGAO_FOLDER  - Binds IID_IShellFolder
    FileSystem      = 0x00000002,   //! SFGAO_FILESYSTEM
    Library         = 0x00000004,   //! Shell library
    StorageObject   = 0x00000008,   //! SFGAO_STORAGE - Binds IID_Storage
    Stream          = 0x00000010,   //! SFGAO_STREAM  - Binds IID_Stream
    ReparsePoint    = 0x00000020,   //! FILE_ATTRIBUTE_REPARSE_POINT
    //! Any node that QFileInfo cannot handle, i.e. not on the filesystem or below file level.
    Virtual         = 0x00000040,

    ObjectTypeMask  = 0x000000FF,

    MountPoint      = 0x00000100,   //! FILE_ATTRIBUTE_REPARSE_POINT
    Junction        = 0x00000200,   //! FILE_ATTRIBUTE_REPARSE_POINT
    SymLink         = 0x00000400,   //! FILE_ATTRIBUTE_REPARSE_POINT
    ShellLink       = 0x00000800,   //! SFGAO_LINK

    LinkMask        = 0x00000F00,

    CanCopy         = 0x00001000,   //! SFGAO_CANCOPY
    CanMove         = 0x00002000,   //! SFGAO_CANMOVE
    CanRename       = 0x00004000,   //! SFGAO_CANRENAME
    CanDelete       = 0x00008000,   //! SFGAO_CANDELETE

    ReadOnly        = 0x00010000,   //! SFGAO_READONLY, FILE_ATTRIBUTE_READONLY
    Hidden          = 0x00020000,   //! SFGAO_HIDDEN, FILE_ATTRIBUTE_HIDDEN
    System          = 0x00040000,   //! SFGAO_SYSTEM
    Ghosted         = 0x00080000,   //! SFGAO_GHOSTED

    Remote          = 0x00100000,   //! SFGAO_ISSLOW
    Removable       = 0x00200000,   //! SFGAO_REMOVABLE
    Compressed      = 0x00400000,   //! SFGAO_COMPRESSED, FILE_ATTRIBUTE_COMPRESSED
    Encrypted       = 0x00800000,   //! FILE_ATTRIBUTE_ENCRYPTED

    AccessFlagMask  = 0x0FFF0000,

    MayHaveChildren = 0x10000000,   //! SFGAO_HASSUBFOLDER

    AllFlags        = 0x1FFFFFFF
};

Q_DECLARE_FLAGS(NodeFlags, NodeFlag);
Q_FLAG_NS(NodeFlag);

NodeFlags fileAttributeToNodeFlags(DWORD flags);
SFGAOF nodeFlagsToSfgao(NodeFlags flags);
NodeFlags sfgaoFlagsToNodeFlags(SFGAOF flags);

} // namespace shell
} // namespace windows
} // namespace qe



#endif // QE_WINDOWS_SHELL_H
