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
#ifndef QE_WINDOWS_SHELLCACHE_H
#define QE_WINDOWS_SHELLCACHE_H

#include <qewindows/global.h>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <qecore/dptr.h>
#include <qewindows/shell.h>

namespace qe {
namespace windows {

class ShellNode;
using ShellNodePointer = QSharedPointer<ShellNode>;

class ShellCachePrivate;
class QE_WINDOWS_EXPORT ShellCache : public ::QObject, public qe::PublicBase
{
    Q_OBJECT
public:

    using key_type = unsigned int;
    using value_type = ShellNode;

    ShellCache(QObject *parent);
    Q_DISABLE_COPY(ShellCache)
    ~ShellCache();

    ShellCache *globalInstance() const;

    bool contains(key_type key) const noexcept;
    ShellNodePointer get(key_type key) const;

    ShellNodePointer insert(ShellItemPointer item);
    ShellNodePointer insert(const IdListPointer &id);

    key_type keyFor(IShellItem2 *item) const;
    inline key_type keyFor(const ITEMIDLIST_ABSOLUTE *id) const;
    key_type keyFor(const wchar_t *parsingPath) const;

    bool remove(key_type key);

protected:
    ShellCache(ShellCachePrivate &dd, QObject *parent);

    bool insert(key_type, ShellNodePointer &object);
    void clear();

private:
    QE_DECLARE_PRIVATE(ShellCache)
};


} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLCACHE_H
