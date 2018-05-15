#include "test_uniquepointer.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    empty_pointer_test();
    basic_pointer_test();
    reset_pointer_test();
    compare_pointer_test();
    swap_pointer_test();
    std_container_test();

//    qe::UniquePointer<int>();
//    qe::UniquePointer<char>{};
//    qe::UniquePointer<int> t1 = nullptr;
//    qe::UniquePointer<int> t2 = {nullptr};
//    //qe::UniquePointer<int> t3 = t1; //no copying
//    //qe::UniquePointer<int> t4(t1); //no copying

//    qe::UniquePointer<int> p(nullptr);

//    assert(equal_check(p, nullptr));
//    assert(!not_equal_check(p, nullptr));

//    const int a = 3;
//    p = qe::makeUnique<int>(a);
//    assert(equal_check(*p, 3));
//    assert(not_equal_check(*p, 2));

//    auto p2 = qe::makeUnique<int>(1);
//    p2.swap(p);
//    assert(not_equal_check(p, p2));

//    p.reset();
//    assert(p.get() == nullptr);

//    std::cout << "Success." << std::endl;

    return 0;
}
