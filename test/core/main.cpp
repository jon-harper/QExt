#include "test_uniquepointer.h"
#include "test_managedpointer.h"

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    unique_pointer_test::run();
    managed_pointer_test::run();

    return 0;
}
