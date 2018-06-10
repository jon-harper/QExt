/*  QExt: Extensions to Qt
 *  Copyright (C) 2016  Jonathan Harper
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef QE_WINDOWS_SHELLNODEINFO_H
#define QE_WINDOWS_SHELLNODEINFO_H

#define STRICT_TYPED_ITEMIDS
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <qewindows/global.h>
#include <qewindows/shellnodedata.h>
#include <qewindows/types.h>

namespace qe {
namespace windows {

//! Similar in function to QFileInfo, but for items in the shell namespace.
class QE_WINDOWS_EXPORT ShellNodeInfo
{
public:
    ShellNodeInfo() = default;
    ShellNodeInfo(QString filepath);
    //ShellNodeInfo(const QFileInfo &fi);
    ShellNodeInfo(ShellNodeDataPointer data) noexcept : d(data) {}
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
    void refresh();

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

    ShellItem2Pointer itemPointer() const;
    IdListPointer idListPointer() const;
private:
    ShellNodeDataPointer d;
};
} // namespace windows
} // namespace qe
#endif // QE_WINDOWS_SHELLNODEINFO_H
