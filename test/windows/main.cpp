#include "test_unknownpointer.h"
#include "test_shellnodeinfo.h"
#include "test_shellcache.h"

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication(argc, argv);

    unknown_pointer_test::run();
    shell_node_info_test::run();
    shell_cache_test::run();

    return 0;
}
