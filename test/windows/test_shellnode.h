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

        qDebug() << root->data()->parsingName;
        qDebug() << root->data()->displayName;
    }

};

#endif // QE_TEST_SHELLNODE_H
