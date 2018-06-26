#ifndef QE_TEST_SHELLNODE_H
#define QE_TEST_SHELLNODE_H

#include <ShlObj_core.h>
#include <QtCore/QDebug>
#include <qewindows/shellnode.h>
#include "../core/test.h"

struct test_shellnode
{
    static void run()
    {
        CoInitialize(nullptr);
        test_shellnode::basic_tests();
        test_shellnode::test_comparison();
        test_shellnode::test_getters();
        test_shellnode::test_binding();
        CoUninitialize();
    }

    static void basic_tests()
    {
        using namespace qe::windows;
        auto root = ShellNode::rootNode();
        EXPECT_TRUE(root->isValid());
        EXPECT_TRUE(root->isRoot());
        EXPECT_FALSE(root->isEnumerated());
        EXPECT_TRUE(root->hasChildren());

        root->enumerate();
        EXPECT_TRUE(root->isEnumerated());
        EXPECT_TRUE(root->hasChildren());
        EXPECT_GT(root->childCount(), 0);

        auto child = root->children().first();
        EXPECT_TRUE(child);
        EXPECT_TRUE(child->isValid());
        EXPECT_FALSE(child->isRoot());
        EXPECT_EQ(root->childIndex(child), 0);
    }

    static void test_comparison()
    {
        using namespace qe::windows;
        auto root = ShellNode::rootNode();
        root->enumerate();
        EXPECT_TRUE(root);

        auto root2 = root;
        EXPECT_TRUE(root2);
        EXPECT_EQ(root, root2);

        auto child = root->childAt(0);
        EXPECT_TRUE(child);

        EXPECT_NE(root, child);
        EXPECT_NE(*root, *child);
        EXPECT_LT(root, child);

        auto child2 = root->childAt(1);
        EXPECT_NE(child, child2);
        EXPECT_LT(child, child2);
        EXPECT_GT(child2, child);
    }

    static void test_getters()
    {
        using namespace qe::windows;
        auto root = ShellNode::rootNode();
        if (!root->isEnumerated())
            root->enumerate();

        //Ensure we get a valid idlist
        shell::IdList id = root->idList();
        EXPECT_TRUE(id);

        //Same for the interface poiter
        ShellItem2Pointer item = root->itemPointer();
        EXPECT_TRUE(item);

        //And again for the node data
        const ShellNodeDataPointer data = root->data();
        EXPECT_FALSE(data->invalid);

        //verify that we can get children and they they are the same, regardless
        //of our method of access.
        auto child = root->childAt(0);
        EXPECT_TRUE(child);
        EXPECT_EQ(root->childIndex(child), 0);
        EXPECT_EQ(root->children().first(), child);

        EXPECT_FALSE(child->displayName().isEmpty());
        EXPECT_FALSE(child->parsingName().isEmpty());
    }

    static void test_binding()
    {
        using namespace qe::windows;
        auto root = ShellNode::rootNode();
        if (!root->isEnumerated())
            root->enumerate();


//        auto eid = root->bindToObject<IEnumIDList>();
//        EXPECT_TRUE(eid);
    }

};

#endif // QE_TEST_SHELLNODE_H
