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
#ifndef QE_WINDOWS_SHELLCACHE_P_H
#define QE_WINDOWS_SHELLCACHE_P_H

#include <QMap>
#include <qewindows/shellcache.h>
#include <qewindows/types.h>
#include <qewindows/shellnode.h>
#include <qewindows/shellcache.h>

namespace qe {
namespace windows {

class QE_WINDOWS_EXPORT ShellCachePrivate : public PrivateBase
{
public:
    ShellCachePrivate(ShellCache *qq);
    Q_DISABLE_COPY(ShellCachePrivate)

    void init();

    ShellNodePointer desktop;
    ShellNodeContainer libraries;
    ShellNodeContainer drives;

    QMap<ShellCache::KeyType, ShellNodePointer> nodes;

private:
    QE_DECLARE_PUBLIC(ShellCache)
};

} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLCACHE_H
