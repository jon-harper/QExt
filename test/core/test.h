#ifndef QE_TEST_TEST_H
#define QE_TEST_TEST_H

#include <assert.h>

auto equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs == rhs);
};

auto not_equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs != rhs);
};

auto less_than_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs < rhs);
};

auto less_than_or_equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs <= rhs);
};

auto greater_than_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs > rhs);
};

auto greater_than_or_equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs >= rhs);
};

auto implicit_true_check = [](auto && lhs) -> bool
{
    return lhs ? true : false;
};

auto implicit_false_check = [](auto && lhs) -> bool
{
    return lhs ? false : true;
};

template <class T>
auto flag_check = [](auto && lhs, auto && rhs) -> bool
{
    return static_cast<T>(lhs) & static_cast<T>(rhs) ? true : false;
};

#define EXPECT_EQ(x, y) assert(equal_check(x, y))
#define EXPECT_NE(x, y) assert(not_equal_check(x, y))
#define EXPECT_LT(x, y) assert(less_than_check(x, y))
#define EXPECT_LE(x, y) assert(less_than_or_equal_check(x, y))
#define EXPECT_GT(x, y) assert(greater_than_check(x, y))
#define EXPECT_GE(x, y) assert(greater_than_or_equal_check(x, y))
#define EXPECT_TRUE(x)  assert(implicit_true_check(x))
#define EXPECT_FALSE(x) assert(implicit_false_check(x))
#define EXPECT_FLAG(type, x, y) assert(flag_check<type>(x, y))

#endif // QE_TEST_TEST_H
