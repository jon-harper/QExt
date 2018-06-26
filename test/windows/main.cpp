#include "test_unknownpointer.h"
#include "test_shellnodeinfo.h"
#include "test_unaligned.h"
#include "test_shellidlist.h"
#include "test_shellnode.h"

#include <QCoreApplication>
#include <qdebug.h>

int main(int argc, char **argv)
{
    QCoreApplication(argc, argv);
    CoInitialize(nullptr);
    qDebug() << "Testing unaligned.h";
    test_unaligned::run();
    qDebug() << "Testing shell::idList";
    test_shellidlist::run();
    qDebug() << "Testing UnknownPointer";
    test_unknownpointer::run();
    qDebug() << "Testing ShellNodeInfo";
    test_shellnodeinfo::run();
    qDebug() << "Testing ShellNode";
    test_shellnode::run();
    CoUninitialize();
    return 0;
}
