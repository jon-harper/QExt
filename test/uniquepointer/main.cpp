#include <iostream>
#include <qecore/uniquepointer.h>
#include <assert.h>

auto equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs == rhs) && (rhs == lhs);
};

auto not_equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs != rhs) && (rhs != lhs);
};

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QeUniquePointer<int>();
    QeUniquePointer<char>{};
    QeUniquePointer<int> t1();
    QeUniquePointer<int> t2{};
    //QeUniquePointer<int> t3 = t1; //no copying
    //QeUniquePointer<int> t4(t1); //no copying

    QeUniquePointer<int> p(nullptr);

    assert(equal_check(p, nullptr));
    assert(!not_equal_check(p, nullptr));

    const int a = 3;
    p = qeMakeUnique<int>(a);
    assert(equal_check(*p, 3));
    assert(not_equal_check(*p, 2));

    auto p2 = qeMakeUnique<int>(1);
    p2.swap(p);
    assert(not_equal_check(p, p2));

    p.reset();
    assert(p.get() == nullptr);

    std::cout << "Success." << std::endl;

    return 0;
}
