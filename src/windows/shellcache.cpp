#include "shellcache_p.h"
#include <QtCore/QCoreApplication>

namespace qe {
namespace windows {

namespace detail {

static ShellCache *globalInstance = nullptr;

} // namespace detail

ShellCachePrivate::ShellCachePrivate(ShellCache *qq)
    : PrivateBase(qq)
{

}

void ShellCachePrivate::init()
{

}

//! Constructs a new, separate instance of the cache with a parent object.
ShellCache::ShellCache(QObject *parent)
    : ShellCache(*new ShellCachePrivate(this), parent)
{

}

//! Virtual destructor; does nothing.
ShellCache::~ShellCache()
{

}


//! Returns the global instance of the shell cache, which is created on first call.
//! \note There must be a constructed instance of `Q*Application` to call this.
ShellCache *ShellCache::globalInstance() const
{
    if Q_LIKELY(detail::globalInstance) {
        return detail::globalInstance;
    }
    Q_ASSERT(qApp); //ShellCache requires a QApplication instance
    detail::globalInstance = new ShellCache(qApp);
    return detail::globalInstance;
}

//! Returns if the cache contains the given \arg key.
bool ShellCache::contains(ShellCache::key_type key) const noexcept
{
    QE_CD;
    return d->nodes.contains(key);
}

//! Computes and returns the hash key for \arg item.
ShellCache::key_type ShellCache::keyFor(IShellItem2 *item) const
{
    IdListPointer id;
    ::SHGetIDListFromObject(reinterpret_cast<IUnknown *>(item), id.addressOf());
    if (!id)
        return 0;
    return keyFor(id.get());
}

//! Computes and returns the hash key of \arg id.
ShellCache::key_type ShellCache::keyFor(const ITEMIDLIST_ABSOLUTE *id) const
{
    return idListHash(id);
}

//! Returns the value for \arg key if the cache contains it, otherwise an empty pointer.
ShellNodePointer ShellCache::retrieve(ShellCache::key_type key) const
{
    QE_CD;
    if (!d->nodes.contains(key))
        return ShellNodePointer();
    return d->nodes.value(key);
}

//! Removes a key/value pair from the cache for the given \arg key, returning false on failure.
bool ShellCache::remove(ShellCache::key_type key)
{
    Q_UNUSED(key);
    Q_UNIMPLEMENTED();
    return false;
}

//! Main internal constructor.
ShellCache::ShellCache(ShellCachePrivate &dd, QObject *parent)
    : QObject(parent), PublicBase(dd)
{

}

} // namespace windows
} // namespace qe
