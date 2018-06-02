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
#ifndef QE_WINDOWS_SHELLNODEDATA_H
#define QE_WINDOWS_SHELLNODEDATA_H

#include <qewindows/global.h>

#include <wtypes.h>
#include <ShlObj.h>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>

#include <qewindows/shell.h>

namespace qe {
namespace windows {

struct QE_WINDOWS_EXPORT ShellNodeData
{
    using pointer_type = QSharedPointer<ShellNodeData>;
    using container_type = QVector<pointer_type>;

    //pointer_type create(const QString &filepath);
    //pointer_type create(const QFile &file);
    //pointer_type create(const QFileInfo &fileinfo);
    static pointer_type create(const IdListPointer &ptr);
    static pointer_type create(ShellItemPointer ptr);

    void refresh();

    ShellItemPointer item;
    IdListPointer id;
    uint hash = 0;
    bool invalid = true;

    QString displayName;
    QString parsingName;
    /*QString description;
    QDateTime createdDate;
    QDateTime modified;
    QDateTime accessed;*/

    shell::NodeFlags flags = shell::NodeFlag::NoFlags;
};

using ShellNodeDataPointer = ShellNodeData::pointer_type;
using ShellNodeDataContainer = QVector<ShellNodeDataPointer>;

} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLNODEDATA_H
