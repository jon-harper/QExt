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
QE_WINDOWS_EXPORT ShellItemPointer desktopItem();

QE_WINDOWS_EXPORT QString parsingFilePath(const ITEMIDLIST_ABSOLUTE *id);

QE_WINDOWS_EXPORT unsigned int idListHash(ITEMIDLIST_ABSOLUTE *id);

enum class NodeFlag : quint32 {
    NoFlags         = 0x00000000,
    Folder          = 0x00000001,   //SFGAO_FOLDER  - Binds IID_IShellFolder
    FileSystem      = 0x00000002,   //SFGAO_FILESYSTEM
    Library         = 0x00000004,   //Shell library
    StorageObject   = 0x00000008,   //SFGAO_STORAGE - Binds IID_Storage
    Stream          = 0x00000010,   //SFGAO_STREAM  - Binds IID_Stream
    ReparsePoint    = 0x00000020,   //FILE_ATTRIBUTE_REPARSE_POINT
    Virtual         = 0x00000040,

    ObjectTypeMask  = 0x0000007F,   //Browseable, Folder, FileSystem, StorageObject & Stream

    MountPoint      = 0x00000100,   //FILE_ATTRIBUTE_REPARSE_POINT
    HardLink        = 0x00000200,
    Junction        = 0x00000400,   //FILE_ATTRIBUTE_REPARSE_POINT
    SymLink         = 0x00000800,   //FILE_ATTRIBUTE_REPARSE_POINT
    ShellLink       = 0x00001000,   //SFGAO_LINK

    LinkMask        = 0x0000FF000,

    CanCopy         = 0x00010000,   //SFGAO_CANCOPY
    CanMove         = 0x00020000,   //SFGAO_CANMOVE
    CanRename       = 0x00040000,   //SFGAO_CANRENAME
    CanDelete       = 0x00080000,   //SFGAO_CANDELETE

    ReadOnly        = 0x00100000,   //SFGAO_READONLY, FILE_ATTRIBUTE_READONLY
    Hidden          = 0x00200000,   //SFGAO_HIDDEN, FILE_ATTRIBUTE_HIDDEN
    System          = 0x00400000,   //SFGAO_SYSTEM
    Ghosted         = 0x00800000,   //SFGAO_GHOSTED

    AccessFlagMask  = 0x00FF0000,

    Remote          = 0x01000000,   //SFGAO_ISSLOW
    Removable       = 0x02000000,   //SFGAO_REMOVABLE
    Compressed      = 0x04000000,   //SFGAO_COMPRESSED, FILE_ATTRIBUTE_COMPRESSED
    Encrypted       = 0x08000000,    //FILE_ATTRIBUTE_ENCRYPTED
};

Q_DECLARE_FLAGS(NodeFlags, NodeFlag);
Q_FLAG_NS(NodeFlag);

namespace detail {

QString longPathPrefix()
{
    return QStringLiteral("\\\\?\\");
}

SFGAOF nodeFlagsToSfgao(NodeFlags flags);
NodeFlags sfgaoFlagsToNodeFlags(SFGAOF flags);

NodeFlags fileAttributeToNodeFlags(DWORD flags);

} // namespace detail
} // namespace shell
} // namespace windows
} // namespace qe



#endif // QE_WINDOWS_SHELL_H
