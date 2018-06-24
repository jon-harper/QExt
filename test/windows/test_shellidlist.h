#ifndef TEST_SHELLIDLIST_H
#define TEST_SHELLIDLIST_H

#include "../core/test.h"
#include <qewindows/idlist.h>
#include <qdebug.h>

qe::windows::shell::IdList copy_from_path(LPCWSTR path)
{
    qe::windows::shell::IdList ret;
    auto hr = SHParseDisplayName(path, nullptr, ret.castAddress<PIDLIST_ABSOLUTE *>(), 0, nullptr);
    EXPECT_EQ(hr, S_OK);
    return ret;
}

void dump_id(qe::windows::shell::IdList &id)
{
    qDebug() << "Address: " << id.data()
             << " Type:    "<< id.rawType()
             << " Length:  " << id.elementCount()
             << " Bytes:   " << id.byteCount() << '\n';
}

struct test_shellidlist
{
    static void run()
    {
        CoInitialize(nullptr);
        test_construction();
        test_iteration();
        CoUninitialize();
    }

    static void test_construction()
    {
        using namespace qe::windows::shell;
        IdList id1(copy_from_path(L"C:\\"));
        EXPECT_TRUE(id1);

        IdList id2(std::move(id1));
        EXPECT_TRUE(id2);

        IdList id3 = id2;
        EXPECT_TRUE(id3);

        IdList id4 = std::move(id2);
        EXPECT_TRUE(id4);
    }

    static void test_iteration()
    {
        using namespace qe::windows::shell;
        IdList id1(copy_from_path(L"C:\\Program Files\\Common Files"));
        EXPECT_TRUE(id1);

        auto iter = id1.begin();
        EXPECT_TRUE(iter);
        EXPECT_GT(iter->mkid.cb, 0);
        auto end_iter = id1.end();
        EXPECT_TRUE(end_iter.isNull());
        while (iter != end_iter) {
            ++iter;
        }
        for (auto iter2 = id1.begin(); iter2.hasNext(); iter2.next());

        for (const auto &iter3 : id1) {
            (void)iter3;
        }
    }
};

#endif // TEST_SHELLIDLIST_H
