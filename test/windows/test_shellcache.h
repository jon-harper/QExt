#ifndef QE_TEST_SHELLCACHE_H
#define QE_TEST_SHELLCACHE_H

#define STRICT_TYPED_ITEMIDS
#include <shtypes.h>
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
        test_insert();
    }

    static void test_construction()
    {
        ShellCache *cache = ShellCache::globalInstance();
        EXPECT_TRUE(cache);
    }

    static void test_key_for()
    {
        {
            auto item = shell::desktopItem();
            EXPECT_TRUE(item);

            auto id = shell::idListFromItem(item);

            auto itemKey = ShellCache::keyFor(item.asUnknown());
            auto idKey = ShellCache::keyFor(id.get());
            EXPECT_EQ(itemKey, idKey);
        }
        {
            auto id = shell::knownFolderIdList(FOLDERID_ComputerFolder);
            auto idKey = ShellCache::keyFor(id.get());

            auto item = shell::knownFolderItem(FOLDERID_ComputerFolder);
            EXPECT_TRUE(item);
            auto itemKey = ShellCache::keyFor(item.asUnknown());
            EXPECT_EQ(idKey, itemKey);
        }
    }

    static void test_insert()
    {
        auto node = ShellCache::globalInstance()->desktopNode();
        EXPECT_TRUE(node);
        EXPECT_TRUE(node->isDesktop());
        EXPECT_TRUE(node->isValid());
        node->enumerate();
        qDebug() << node->children().count();
    }
};
#endif // QE_TEST_SHELLCACHE_H
