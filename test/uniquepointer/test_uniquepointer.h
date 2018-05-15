/*  QExt: Extensions to Qt
 *  Copyright (C) 2016  Jonathan Harper
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Based on: https://github.com/SRombauts/shared_ptr
 *
 * Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef TEST_UNIQUEPOINTER_H
#define TEST_UNIQUEPOINTER_H

#include <assert.h>
#include <vector>

#include <qecore/uniquepointer.h>

using namespace qe;

auto equal_check = [](auto && lhs, auto && rhs) -> bool
{
    return (lhs == rhs) && (rhs == lhs);
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

#define EXPECT_EQ(x, y) assert(equal_check(x, y))
#define EXPECT_NE(x, y) assert(not_equal_check(x, y))
#define EXPECT_LT(x, y) assert(less_than_check(x, y))
#define EXPECT_LE(x, y) assert(less_than_or_equal_check(x, y))
#define EXPECT_GT(x, y) assert(greater_than_check(x, y))
#define EXPECT_GE(x, y) assert(greater_than_or_equal_check(x, y))
#define EXPECT_TRUE(x) assert(implicit_true_check(x))
#define EXPECT_FALSE(x) assert(implicit_false_check(x))

struct Struct2
{
    explicit Struct2(int aVal) :
        value(aVal)
    {
        instances++;
    }

    ~Struct2()
    {
        instances--;
    }

    void incr()
    {
        value++;
    }

    void decr()
    {
        value--;
    }

    int         value;
    static int instances;
};

int Struct2::instances = 0;



void empty_pointer_test()
{
    // Create an empty (ie. nullptr) UniquePointer
    UniquePointer<Struct2> xPtr = nullptr;

    EXPECT_FALSE(xPtr);
    EXPECT_EQ(nullptr, xPtr.get());

    // Reset to nullptr (ie. do nothing)
    xPtr.reset();

    EXPECT_FALSE(xPtr);
    EXPECT_EQ(nullptr,  xPtr.get());

}

void basic_pointer_test()
{
    {
        // Create a UniquePointer
        UniquePointer<Struct2> xPtr(new Struct2(123));

        EXPECT_TRUE(xPtr);
        EXPECT_NE(nullptr, xPtr.get());

        if (xPtr)
        {
            EXPECT_EQ(123,  xPtr->value);
            EXPECT_EQ(1,    xPtr->instances);
            EXPECT_EQ(1,    Struct2::instances);

            // call a function
            xPtr->incr();
            EXPECT_EQ(124,  xPtr->value);
            (*xPtr).incr();
            EXPECT_EQ(125,  (*xPtr).value);
            xPtr->decr();
            xPtr->decr();

            // Copy construct the UniquePointer, transferring ownership
            UniquePointer<Struct2> yPtr(std::move(xPtr));
            xPtr.reset();

            EXPECT_NE(xPtr, yPtr);
            EXPECT_FALSE(xPtr);
            EXPECT_EQ(nullptr,  xPtr.get());
            EXPECT_TRUE(yPtr);
            EXPECT_NE(nullptr,  yPtr.get());
            EXPECT_EQ(123,   yPtr->value);
            EXPECT_EQ(1,     Struct2::instances);

            if (yPtr)
            {
                UniquePointer<Struct2> zPtr;
                // Assign the UniquePointer, transferring ownership
                zPtr = std::move(yPtr);
                yPtr.reset();

                EXPECT_NE(yPtr,  zPtr);
                EXPECT_FALSE(yPtr);
                EXPECT_EQ(nullptr, yPtr.get());
                EXPECT_TRUE(zPtr);
                EXPECT_NE(nullptr,  zPtr.get());
                EXPECT_EQ(123,   zPtr->value);
                EXPECT_EQ(1,     Struct2::instances);
            }

            EXPECT_FALSE(xPtr);
            EXPECT_EQ(nullptr, xPtr.get());
            EXPECT_FALSE(yPtr);
            EXPECT_EQ(nullptr, yPtr.get());
            EXPECT_EQ(0, Struct2::instances);
        }
        else
        {
            assert(false); //"bool cast operator error"
        }

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(nullptr, xPtr.get());
        EXPECT_EQ(0, Struct2::instances);
    }

    EXPECT_EQ(0, Struct2::instances);
}

void reset_pointer_test()
{
    // Create an empty (ie. nullptr) UniquePointer
    UniquePointer<Struct2> xPtr;

    // Reset it with a new pointer
    xPtr.reset(new Struct2(123));

    EXPECT_TRUE(xPtr);
    EXPECT_NE(nullptr, xPtr.get());
    EXPECT_EQ(123,  xPtr->value);
    EXPECT_EQ(1,    Struct2::instances);
    Struct2* pX  = xPtr.get();

    // Reset it with another new pointer
    xPtr.reset(new Struct2(234));

    EXPECT_TRUE(xPtr);
    EXPECT_NE(nullptr, xPtr.get());
    EXPECT_EQ(234,  xPtr->value);
    EXPECT_EQ(1,    Struct2::instances);
    EXPECT_NE(pX,   xPtr.get());

    // Copy-construct a new UniquePointer to the same object, transferring ownership
    UniquePointer<Struct2> yPtr = std::move(xPtr);
    xPtr.reset();

    EXPECT_NE(xPtr, yPtr);
    EXPECT_FALSE( xPtr);
    EXPECT_EQ(nullptr,  xPtr.get());
    EXPECT_TRUE( yPtr);
    EXPECT_NE(nullptr,  yPtr.get());
    EXPECT_EQ(234,   yPtr->value);
    EXPECT_EQ(1,     Struct2::instances);

    // Reset to nullptr
    yPtr.reset();

    EXPECT_EQ(nullptr,  yPtr.get());
    EXPECT_FALSE(xPtr);
    EXPECT_EQ(nullptr,  xPtr.get());
    EXPECT_EQ(0, Struct2::instances);
}

void compare_pointer_test()
{
    // Create a UniquePointer
    UniquePointer<Struct2> xPtr(new Struct2(123));

    EXPECT_TRUE(xPtr);
    EXPECT_NE(nullptr, xPtr.get());
    EXPECT_EQ(123,xPtr->value);
    EXPECT_EQ(1, Struct2::instances);
    Struct2* pX = xPtr.get();

    // Create another UniquePointer
    UniquePointer<Struct2> yPtr(new Struct2(234));

    EXPECT_TRUE(xPtr);
    EXPECT_NE(nullptr, xPtr.get());
    EXPECT_EQ(123,xPtr->value);
    EXPECT_EQ(2, Struct2::instances);

    EXPECT_TRUE(yPtr);
    EXPECT_NE(nullptr, yPtr.get());
    EXPECT_EQ(234, yPtr->value);
    Struct2* pY = yPtr.get();


    EXPECT_NE(xPtr, yPtr);
    EXPECT_NE(pY->value, pX->value);
}

void swap_pointer_test()
{
    // Create a UniquePointer
    UniquePointer<Struct2> xPtr(new Struct2(123));

    EXPECT_TRUE(xPtr);
    EXPECT_NE(nullptr, xPtr.get());
    EXPECT_EQ(123,xPtr->value);
    EXPECT_EQ(1, Struct2::instances);

    // Create another UniquePointer
    UniquePointer<Struct2> yPtr(new Struct2(234));

    EXPECT_TRUE(yPtr);
    EXPECT_NE(nullptr, yPtr.get());
    EXPECT_EQ(234, yPtr->value);
    EXPECT_EQ(2, Struct2::instances);

    EXPECT_LT(xPtr->value, yPtr->value);
    xPtr.swap(yPtr);
    EXPECT_GT(xPtr->value, yPtr->value);
    EXPECT_TRUE(xPtr);
    EXPECT_TRUE(yPtr);
}

void std_container_test()
{
    // Create a shared_ptr
    UniquePointer<Struct2> xPtr(new Struct2(123));

    EXPECT_TRUE(xPtr);
    EXPECT_NE(nullptr, xPtr.get());
    EXPECT_EQ(123, xPtr->value);
    EXPECT_EQ(1, Struct2::instances);
    Struct2* pX = xPtr.get();

    {
        std::vector<UniquePointer<Struct2> > PtrList;

        // Move-it inside a container, transferring ownership
        PtrList.push_back(std::move(xPtr));

        EXPECT_FALSE(xPtr);
        EXPECT_TRUE( PtrList.back());
        EXPECT_EQ(pX,PtrList.back().get());
        EXPECT_EQ(1, Struct2::instances);

    } // Destructor of the vector releases the last pointer thus destroying the object

    EXPECT_EQ(0, Struct2::instances);
}

#endif // TEST_UNIQUEPOINTER_H
