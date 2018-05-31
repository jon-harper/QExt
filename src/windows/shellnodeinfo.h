#ifndef QE_WINDOWS_SHELLNODEINFO_H
#define QE_WINDOWS_SHELLNODEINFO_H

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <qewindows/global.h>
#include <qewindows/shellnodedata.h>

namespace qe {
namespace windows {

//! Similar in function to QFileInfo, but for items in the shell namespace.
class QE_WINDOWS_EXPORT ShellNodeInfo
{
public:
    ShellNodeInfo() = default;
    ShellNodeInfo(QString filepath);
    //ShellNodeInfo(const QFileInfo &fi);
    ShellNodeInfo(const ShellNodeInfo &other) noexcept : d(other.d) { }
    ShellNodeInfo(ShellNodeInfo &&other) noexcept { swap(other); }
    ~ShellNodeInfo() = default;

    ShellNodeInfo &operator=(const ShellNodeInfo &other);
    ShellNodeInfo &operator=(ShellNodeInfo &&other) noexcept;

    void swap(ShellNodeInfo &other) noexcept { std::swap(d, other.d); }

    bool setNode(QString filepath);
    //bool setNode(const QFileInfo &fileinfo);

    bool exists() const noexcept { return d; }

    //! Updates the cached data in the node.
    //! \warning This may be slow for remote objects.
    inline void refresh() noexcept;

    //! Returns a `QFileInfo` representing the node, if one can be constructed.
    QFileInfo fileInfo() const noexcept;

    //! Returns the formatted display name of the node. An empty QString is returned for invalid nodes.
    QString displayName() const noexcept { return exists() ? d->displayName : QString();}

    //! \brief Returns the parsing name of the node. An empty QString is returned for invalid nodes.
    //! \note This value may or may not be a filesystem-compatible value. Call
    //! \ref filesystemPathName() if you a value you can pass to `QFile`, etc.
    QString parsingName() const noexcept { return exists() ? d->parsingName: QString(); }

    QString filesystemPathName() const;

    //! Returns the raw flags used to identify aspects of the node
    shell::NodeFlags flags() const noexcept { return exists() ? d->flags : shell::NodeFlag::NoFlags; }

    bool isVirtual() const noexcept { return !(flags() & shell::NodeFlag::FileSystem); }
    bool isFolder() const noexcept { return flags() & shell::NodeFlag::Folder; }

    QVariant propertyValue(const PROPERTYKEY &pkey);
private:
    ShellNodeDataPointer d;

};
} // namespace windows
} // namespace qe
#endif // QE_WINDOWS_SHELLNODEINFO_H
