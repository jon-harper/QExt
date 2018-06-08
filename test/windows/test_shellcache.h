#ifndef QE_TEST_SHELLCACHE_H
#define QE_TEST_SHELLCACHE_H

#include <QDebug>
#include <qewindows/global.h>
#include <qewindows/shellcache.h>
#include "../core/test.h"

using namespace qe::windows;

struct shell_cache_test
{
    static void run()
    {
        test_construction();
        test_key_for();
    }

    static void test_construction()
    {
        ShellCache *cache = ShellCache::globalInstance();

        EXPECT_TRUE(cache);
    }

    static void test_key_for()
    {
        auto item = ShellItem2Pointer();
        auto hr = ::SHCreateItemFromParsingName(L"C:",
                                                shell::createBindContext().get(),
                                                IID_PPV_ARGS(item.addressOf()));
        EXPECT_EQ(hr, S_OK);

        auto id = shell::idListFromItem(item);
        auto itemKey = ShellCache::keyFor(item.asUnknown());
        auto idKey = ShellCache::keyFor(id.get());

        EXPECT_EQ(itemKey, idKey);
    }
};
#endif // QE_TEST_SHELLCACHE_H
