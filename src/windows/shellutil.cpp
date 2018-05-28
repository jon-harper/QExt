#include "shellutil.h"
#include <ShlObj.h>

namespace qe {
namespace windows {
namespace util {

ShellItemPointer itemFromIdList(ITEMIDLIST_ABSOLUTE *id)
{
    if (!id)
        return nullptr;
    ShellItemPointer ret;
    SHCreateItemFromIDList(id, IID_PPV_ARGS(ret.addressOf()));
    return ret;
}

IdListPointer idListFromItem(ShellItemPointer item)
{
    if (!item)
        return nullptr;
    IdListPointer ret;
    SHGetIDListFromObject(item.asUnknown(), ret.addressOf());
    return ret;
}

UnknownPointer<IShellFolder2> desktopFolder()
{
    UnknownPointer<IShellFolder> desktopSf;
    SHGetDesktopFolder(desktopSf.addressOf());
    return desktopSf.queryInterface<IShellFolder2>();
}

ShellItemPointer desktopItem()
{
    ShellItemPointer ret;
    auto desktopSf = desktopFolder();
    auto hr = SHGetItemFromObject(desktopSf.asUnknown(), IID_PPV_ARGS(ret.addressOf()));
    Q_ASSERT(hr == S_OK && ret);
    return ret;
}

} // namespace util
} // namespace windows
} // namespace qe
