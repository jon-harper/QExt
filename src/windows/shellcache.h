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

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QByteArray>
#include <qecore/dptr.h>
#include <qewindows/shell.h>
#include <qewindows/shellnode.h>

namespace qe {
namespace windows {

class ShellCachePrivate;
class QE_WINDOWS_EXPORT ShellCache : public ::QObject, public qe::PublicBase
{
    Q_OBJECT
public:
    using KeyType = QByteArray;
    using ValueType = ShellNodePointer;

    ShellCache(QObject *parent = nullptr);
    Q_DISABLE_COPY(ShellCache)
    ~ShellCache();

    static ShellCache *globalInstance();

    bool contains(KeyType key) const noexcept;

    ShellNodePointer insert(ShellNodePointer node);
    ShellNodePointer insert(IUnknown *unk);

    static KeyType keyFor(IUnknown *item);
    static KeyType keyFor(const ITEMIDLIST_ABSOLUTE *id);
    static KeyType keyFor(const wchar_t *parsingPath);
    static KeyType keyFor(const ShellNodePointer &pointer);

    bool remove(KeyType key);

    ShellNodePointer value(KeyType key) const;
protected:
    ShellCache(ShellCachePrivate &dd, QObject *parent);

    ShellNodePointer createNode(const IdListPointer &id);

    void clear();

private:
    QE_DECLARE_PRIVATE(ShellCache)
};


} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELLCACHE_H
