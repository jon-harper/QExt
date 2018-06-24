#ifndef TEST_SHELLIDLIST_H
#define TEST_SHELLIDLIST_H

#include "../core/test.h"
#include <qewindows/idlist.h>
#include <qewindows/shell.h>
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
             << "Type:    "<< id.rawType()
             << "Length:  " << id.elementCount()
             << "Bytes:   " << id.byteCount();
}

struct test_shellidlist
{
    static void run()
    {
        CoInitialize(nullptr);
        test_shellidlist::test_construction();
        test_shellidlist::test_iteration();
        test_shellidlist::test_accessors();
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

    static void test_accessors()
    {
        using namespace qe::windows::shell;
        IdList id1(copy_from_path(L"C:\\Program Files"));
        EXPECT_TRUE(id1);

        //check that the byte count makes sense
        EXPECT_GT(id1.byteCount(), 5u);
        //This PC -> C: -> Program Files
        EXPECT_EQ(id1.elementCount(), 3);
        EXPECT_FLAG(unsigned int, id1.inferredType(), InferredType::Folder);

        IdList id2 = id1.parent();
        EXPECT_TRUE(id2);
        EXPECT_NE(id2, id1);
        EXPECT_TRUE(ILIsParent(id2.castTo<PCIDLIST_ABSOLUTE>(),
                               id1.castTo<PCIDLIST_ABSOLUTE>(), TRUE));

        EXPECT_EQ(id2.elementCount(), 2);
        EXPECT_FLAG(unsigned int, id2.inferredType(), InferredType::Drive);

        IdList id3 = id2.parent();
        EXPECT_TRUE(id3);
        EXPECT_NE(id3, id2);
        EXPECT_NE(id3, id1);

        EXPECT_FLAG(unsigned int, id3.inferredType(), InferredType::LocalVirtual);
    }
};

#endif // TEST_SHELLIDLIST_H
