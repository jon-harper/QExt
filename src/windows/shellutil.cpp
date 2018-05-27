#include "shellutil.h"
#include <ShlObj.h>

namespace qe {
namespace windows {
namespace util {

ShellItemPointer itemFromIdList(ITEMIDLIST_ABSOLUTE *id)
{
    if (!id)
        return nullptr;
    IShellItem2 * ret = nullptr;
    if (SHCreateItemFromIDList(id, IID_PPV_ARGS(&ret)) == S_OK)
        return ret;
    return nullptr;
}

IdListPointer idListFromItem(UnknownPointer<IShellItem2> item)
{
    if (!item)
        return nullptr;
    ITEMIDLIST_ABSOLUTE *ret = nullptr;
    SHGetIDListFromObject(item.asUnknown(), &ret);
    return ret;
}

UnknownPointer<IShellFolder2> desktopFolder()
{
    UnknownPointer<IShellFolder> desktopSf;
    SHGetDesktopFolder(desktopSf.addressOf());
    return desktopSf.queryInterface<IShellFolder2>();
}

} // namespace util
} // namespace windows
} // namespace qe
