#include "test_unknownpointer.h"
#include "test_shellnodeinfo.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    CoInitialize(nullptr);

    unknown_pointer_test::run();
    shell_node_info_test::run();
    CoUninitialize();
    return 0;
}
