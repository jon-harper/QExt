#include "shellnodeinfo.h"
#include <QVarLengthArray>
#include <QtCore/QDebug>
#include <qewindows/shell.h>
#include <qewindows/winutil.h>

namespace qe {
namespace windows {

ShellNodeInfo::ShellNodeInfo(QString filepath)
{
    setNode(filepath);
}

ShellNodeInfo &ShellNodeInfo::operator=(const ShellNodeInfo &other)
{
    d = other.d;
    return *this;
}

ShellNodeInfo &ShellNodeInfo::operator=(ShellNodeInfo &&other) noexcept
{
    swap(other);
    return *this;
}

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

    Q_ASSERT(filepath.toWCharArray(converted) > 0);
    auto item = ShellItemPointer();
    SHCreateItemFromParsingName(converted, nullptr, IID_PPV_ARGS(item.addressOf()));
    if (!item) {
        qDebug() << "Failed to create NodeInfo from path: " << filepath;
        return false;
    }
    d = ShellNodeData::create(item);
    refresh();
    return true;
}

void ShellNodeInfo::refresh() noexcept
{
    if (!d)
        return;
    d->refresh();
}

QString ShellNodeInfo::filesystemPathName() const
{
    if (!d)
        return {};
    WCharPointer ret;
    d->iface->GetDisplayName(SIGDN_FILESYSPATH, ret.addressOf());
    return QString::fromWCharArray(ret.get());
}

QVariant ShellNodeInfo::propertyValue(const PROPERTYKEY &pkey)
{
    QVariant ret;
    PROPVARIANT var;
    PropVariantInit(&var);
    if (d->iface->GetProperty(pkey, &var) == S_OK)
        ret = util::fromPROPVARIANT(var);
    PropVariantClear(&var);
    return ret;
}

} // namespace windows
} // namespace qe
