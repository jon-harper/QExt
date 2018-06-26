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

        qDebug() << child->displayName();
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

        auto child = root->children().first();
        EXPECT_TRUE(child);

        EXPECT_NE(root, child);
    }

    static void test_binding()
    {
        Q_UNIMPLEMENTED();
    }

};

#endif // QE_TEST_SHELLNODE_H
