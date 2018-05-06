#include <iostream>
#include <qecore/uniquepointer.h>
#include <assert.h>

auto equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return lhs == rhs && rhs == lhs;
};

auto not_equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return lhs != rhs && rhs != lhs;
};

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    const int a = 3;
    QeUniquePointer<int> p = qeMakeUnique<int>(a);
    assert(equal_check(*p, 3));
    assert(not_equal_check(*p, 2));

    QeUniquePointer<int> p2 = QeUniquePointer<int>(qeMakeUnique<int>(1));
    p2.swap(p);
    assert(not_equal_check(p, p2));

    p.reset();
    assert(p.get() == nullptr);

    std::cout << "Success.\n";

    return 0;
}
