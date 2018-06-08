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
ShellCache::ShellCache()
    : ShellCache(*new ShellCachePrivate(this), qApp)
{

}

//! Virtual destructor; does nothing.


//! Returns the global instance of the shell cache, which is created on first call.
//! \note There must be a constructed instance of `Q*Application` to call this.
ShellCache *ShellCache::globalInstance() //static
{
    if Q_LIKELY(detail::globalInstance) {
        return detail::globalInstance;
    }
    Q_ASSERT(qApp); //ShellCache requires a QApplication instance
    detail::globalInstance = new ShellCache;
    return detail::globalInstance;
}

//! Returns if the cache contains the given \arg key.
bool ShellCache::contains(ShellCache::KeyType key) const noexcept
{
    QE_CD;
    return d->nodes.contains(key);
}

//! Returns the value for \arg key if the cache contains it, otherwise an empty pointer.
bool ShellCache::find(const ShellCache::KeyType &key, ShellNodePointer &outPtr) noexcept
{
    QE_D;
    if (!d->nodes.contains(key))
        return false;
    outPtr = d->nodes.value(key);
    return true;
}

ShellNodePointer ShellCache::find(const ShellCache::KeyType &key) const noexcept
{
    QE_CD;
    if (!d->nodes.contains(key))
        return {};
    return d->nodes.value(key);
}

bool ShellCache::insert(ShellNodePointer node)
{
    QE_D;
    if (!node->isValid() || !d->nodes.contains(node->key()))
        return false;
    d->nodes.insert(node->key(), node);
    return true;
}

bool ShellCache::insert(IUnknown *unk, ShellNodePointer &outPtr)
{
    Q_UNIMPLEMENTED();
    return {};
}

//! Computes and returns the hash key for \arg item.
ShellCache::KeyType ShellCache::keyFor(IUnknown *item)
{
    IdListPointer id;
    ::SHGetIDListFromObject(item, id.addressOf());
    if (!id)
        return {};
    return ShellCache::keyFor(id.get());
}

//! Computes and returns the hash key of \arg id.
ShellCache::KeyType ShellCache::keyFor(const ITEMIDLIST_ABSOLUTE *id)
{
    //This causes overflow, but it's just a key, so we don't care.
    return QByteArray(reinterpret_cast<const char *>(id));
}

//! Calls `SHParseDisplayName()` to obtain the key value.
ShellCache::KeyType ShellCache::keyFor(const wchar_t *parsingPath)
{
    IdListPointer id;
    UnknownPointer<IBindCtx> ctx;
    if (!::CreateBindCtx(0, ctx.addressOf()))
        return {};

    ::SHParseDisplayName(parsingPath, ctx.get(), id.addressOf(), 0, nullptr);
    if (!id)
        return {};
    return ShellCache::keyFor(id.get());

}

ShellCache::KeyType ShellCache::keyFor(const ShellNodePointer &pointer)
{
    if (pointer.isNull())
        return {};
    return pointer->key();
}

//! Removes a key/value pair from the cache for the given \arg key, returning false on failure.
bool ShellCache::remove(ShellCache::KeyType key)
{
    QE_D;
    if (d->nodes.contains(key))
        return d->nodes.remove(key);
    return false;
}

//! Main internal constructor.
ShellCache::ShellCache(ShellCachePrivate &dd, QObject *parent)
    : QObject(parent), PublicBase(dd)
{

}

ShellNodePointer ShellCache::createNode(const IdListPointer &id)
{
    auto key = ShellCache::keyFor(id.get());
    auto item = shell::itemFromIdList(id.get());
    Q_ASSERT(!key.isNull());
    Q_ASSERT(item);

    auto parentNode = ShellNodePointer();
    bool needInsert = true;

    auto parent = ShellItemPointer();
    item->GetParent(parent.addressOf());
    if (parent) {
        auto parentKey = ShellCache::keyFor(parent.asUnknown());
        //check if the cache contains the parent node
        if (contains(parentKey)) {
            parentNode = find(parentKey);
            //find out if the parent contains the new node as a child
            for (auto &iter : parentNode->d.children) {
                if (iter->key() == key) {
                    needInsert = false;
                    return iter;

                }
            }
        }
    }
    QE_D;
    auto ret = ShellNodePointer(new ShellNode(item, parentNode, key));
    d->nodes.insert(key, ret);
    if (needInsert)
        parentNode->d.children.append(ret);
    return ret;
}

} // namespace windows
} // namespace qe
