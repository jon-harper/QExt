#include "shellnodeinfo.h"
#include <QVarLengthArray>
#include <QtCore/QDebug>
#include <qewindows/shell.h>
#include <qewindows/winutil.h>

namespace qe {
namespace windows {

//! Constructs an object with the given filepath.
ShellNodeInfo::ShellNodeInfo(QString filepath)
{
    setNode(filepath);
}

//! Copy assignment operator.
ShellNodeInfo &ShellNodeInfo::operator=(const ShellNodeInfo &other)
{
    d = other.d;
    return *this;
}

//! Move assignment operator.
ShellNodeInfo &ShellNodeInfo::operator=(ShellNodeInfo &&other) noexcept
{
    swap(other);
    return *this;
}

//! Sets the node to the given filepath.
//! \note This function uses `SHCreateItemFromParsingName()`, which is limited to `MAX_PATH` (260) chars.
bool ShellNodeInfo::setNode(QString filepath)
{
    wchar_t converted[MAX_PATH] = {'\0'};
    // maybe some day
    /*
    auto converted = QVarLengthArray<wchar_t, MAX_PATH>();
    if (filepath.length() > MAX_PATH) {
        if (!filepath.startsWith(detail::longPathPrefix))
            filepath.prepend(detail::longPathPrefix);
        converted.reserve(filepath.length());
    }*/
    if (filepath.length() > MAX_PATH - 1) {
        qDebug() << "Can't add paths longer than 260 literals to the shell.";
        d.reset();
        return false;
    }

    //Actually copy the string
    Q_ASSERT(filepath.toWCharArray(converted) > 0);

    auto item = ShellItem2Pointer();
    SHCreateItemFromParsingName(converted, nullptr, IID_PPV_ARGS(item.addressOf()));
    if (!item) {
        qDebug() << "Failed to create NodeInfo from path: " << filepath;
        return false;
    }
    d = ShellNodeData::create(item);
    refresh();
    return true;
}

//! Refresh any cached data. This may be slow for network nodes.
void ShellNodeInfo::refresh()
{
    if (!d)
        return;
    d->refresh();
}

//! Returns the filesystem path and filename for the object.
QString ShellNodeInfo::filesystemPathName() const
{
    if (!d)
        return {};
    WCharPointer ret;
    d->item->GetDisplayName(SIGDN_FILESYSPATH, ret.addressOf());
    return QString::fromWCharArray(ret.get());
}

//! Retrieves the value associated with pkey, if it exists.
QVariant ShellNodeInfo::propertyValue(const PROPERTYKEY &pkey)
{
    QVariant ret;
    PROPVARIANT var;
    PropVariantInit(&var);
    if (d->item->GetProperty(pkey, &var) == S_OK)
        ret = util::fromPROPVARIANT(var);
    PropVariantClear(&var);
    return ret;
}

//! Returns a pointer to the object's `IShellItem2` interface.
ShellItem2Pointer ShellNodeInfo::itemPointer() const
{
    return d->item;
}

//! Returns the absolute id of the object.
IdListPointer ShellNodeInfo::idListPointer() const
{
    return d->id;
}

QFileInfo ShellNodeInfo::fileInfo() const
{
    if (exists())
        return QFileInfo(filesystemPathName());
    return {};
}

} // namespace windows
} // namespace qe
