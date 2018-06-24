#include "test_unknownpointer.h"
#include "test_shellnodeinfo.h"
#include "test_typeutil.h"
#include "test_shellidlist.h"


int main(int argc, char *argv[])
{
//    QCoreApplication(argc, argv);
    CoInitialize(nullptr);
    test_typeutil::run();
    test_shellidlist::run();
    test_unknownpointer::run();
    test_shellnodeinfo::run();
    CoUninitialize();
    return 0;
}
