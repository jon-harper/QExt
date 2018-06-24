#ifndef QE_TEST_TYPEUTIL_H
#define QE_TEST_TYPEUTIL_H

#include <qewindows/unaligned.h>

struct test_typeutil
{
    static void run()
    {
        using namespace qe::windows;
        //test that aligned_cast works for non-const pointers
        using AType = int __unaligned *;
        int a_data = 1;
        AType a = &a_data;
        int * aa = aligned_cast<int *>(a);
        (void)aa;
        //check that it works for const pointers
        using BType = const int __unaligned *;
        const int b_data = 2;
        BType b = &b_data;
        const int *bb = aligned_cast<const int *>(b);
        (void)bb;
        //int *bbb = aligned_cast<int *>(b); //<--won't compile, good

        //check that remove_aligned really is doing its job
        using CType = remove_aligned<AType>::type;
        static_assert (is_aligned<CType>::value, "");

        AType *d = &a;
        int ** aaa = aligned_cast<int **>(d); //<-- &a cannot be passed as a reference
        (void)aaa;
    }
};

#endif // QE_TEST_TYPEUTIL_H
