#ifndef QE_WINDOWS_SHELLUTIL_H
#define QE_WINDOWS_SHELLUTIL_H

#include <qewindows/types.h>

namespace qe {
namespace windows {
namespace util {

QE_WINDOWS_EXPORT ShellItemPointer itemFromIdList(ITEMIDLIST_ABSOLUTE *id);
QE_WINDOWS_EXPORT UnknownPointer<IShellFolder2> desktopFolder();
QE_WINDOWS_EXPORT IdListPointer idListFromItem(ShellItemPointer item);
QE_WINDOWS_EXPORT ShellItemPointer desktopItem();

} // namespace util
} // namespace windows
} // namespace qe



#endif // QE_WINDOWS_SHELLUTIL_H
