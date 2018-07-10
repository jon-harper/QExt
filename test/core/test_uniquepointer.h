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
 * Originally distributed under the MIT License (MIT) (See http://opensource.org/licenses/MIT)
 */

#ifndef QE_TEST_UNIQUEPOINTER_H
#define QE_TEST_UNIQUEPOINTER_H

#include <vector>
#include <qecore/uniquepointer.h>
#include "test.h"

struct Struct1
{
    explicit Struct1(int aVal)
        : value(aVal)
    {
        instances++;
    }

    Struct1(const Struct1 &other)
    {
        instances++;
        value = other.value;
    }
    Struct1(Struct1 &&) = default;

    ~Struct1()
    {
        instances--;
    }

    Struct1 &operator =(const Struct1 &other)
    {
        instances++;
        value = other.value;
        return *this;
    }
    Struct1 &operator =(Struct1 &&) = default;

    void incr()
    {
        value++;
    }

    void decr()
    {
        value--;
    }

    int value = 0;
    static int instances;
};

int Struct1::instances = 0;

struct Struct2 : public Struct1
{
    explicit Struct2(const int aVal)
        : Struct1(aVal)
    {

    }
};


struct Struct3 : public Struct2
{
};

struct unique_pointer_test
{

    static void run()
    {
        empty_pointer_test();
        /*basic_pointer_test();
        reset_pointer_test();
        compare_pointer_test();
        swap_pointer_test();
        std_container_test();*/
    }

    static void empty_pointer_test()
    {
        using namespace qe;
        // Default construction
        UniquePointer<Struct2> xPtr;

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(nullptr, xPtr.get());

        // Reset to nullptr (ie. do nothing)
        xPtr.reset();

        EXPECT_FALSE(xPtr);
    }

    static void basic_pointer_test()
    {
        using namespace qe;
        {
            // Initialization with a pointer
            UniquePointer<Struct2> xPtr(new Struct2(123));

            EXPECT_TRUE(xPtr);
            EXPECT_NE(nullptr, xPtr.get());

            if (xPtr)
            {
                EXPECT_EQ(123,  xPtr->value);
                EXPECT_EQ(1,    xPtr->instances);
                EXPECT_EQ(1,    Struct1::instances);

                // call a function
                xPtr->incr();
                EXPECT_EQ(124,  xPtr->value);
                (*xPtr).incr();
                EXPECT_EQ(125,  (*xPtr).value);
                xPtr->decr();
                xPtr->decr();

                // Move construct, transferring ownership
                UniquePointer<Struct2> yPtr{std::move(xPtr)};
                xPtr.reset();

                EXPECT_NE(xPtr, yPtr);
                EXPECT_FALSE(xPtr);
                EXPECT_TRUE(yPtr);
                EXPECT_EQ(123,   yPtr->value);
                EXPECT_EQ(1,     Struct1::instances);

                {
                    //move initialization
                    UniquePointer<Struct2> zPtr = std::move(yPtr);
                    yPtr.reset();

                    EXPECT_NE(yPtr,  zPtr);
                    EXPECT_FALSE(yPtr);
                    EXPECT_TRUE(zPtr);
                    EXPECT_EQ(123,   zPtr->value);
                    EXPECT_EQ(1,     Struct1::instances);
                }

                EXPECT_FALSE(xPtr);
                EXPECT_FALSE(yPtr);
                EXPECT_EQ(0, Struct1::instances);
            }
            else
            {
                assert(false); //"bool cast operator error"
            }

            EXPECT_FALSE(xPtr);
            EXPECT_EQ(0, Struct1::instances);
        }

        EXPECT_EQ(0, Struct1::instances);
    }

    static void reset_pointer_test()
    {
        using namespace qe;
        // Create an empty (ie. nullptr) UniquePointer
        UniquePointer<Struct2> xPtr;

        // Reset it with a new pointer
        xPtr.reset(new Struct2(123));

        EXPECT_TRUE(xPtr);
        EXPECT_NE(nullptr, xPtr.get());
        EXPECT_EQ(123, xPtr->value);
        EXPECT_EQ(1, Struct1::instances);
        Struct2* pX  = xPtr.get();

        // Reset it with another new pointer
        xPtr.reset(new Struct2(234));

        EXPECT_TRUE(xPtr);
        EXPECT_EQ(234, xPtr->value);
        EXPECT_EQ(1,  Struct1::instances);
        EXPECT_NE(pX, xPtr.get());

        // Move-construct a new UniquePointer to the same object, transferring ownership
        UniquePointer<Struct2> yPtr = std::move(xPtr);
        xPtr.reset();

        EXPECT_NE(xPtr, yPtr);
        EXPECT_FALSE(xPtr);
        EXPECT_TRUE(yPtr);
        EXPECT_EQ(234, yPtr->value);
        EXPECT_EQ(1, Struct1::instances);

        // Reset to nullptr
        yPtr.reset();

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(0, Struct1::instances);
    }

    static void compare_pointer_test()
    {
        using namespace qe;
        // Create a UniquePointer
        UniquePointer<Struct2> xPtr(new Struct2(123));

        EXPECT_TRUE(xPtr);
        EXPECT_NE(nullptr, xPtr.get());
        EXPECT_EQ(123,xPtr->value);
        EXPECT_EQ(1, Struct1::instances);
        Struct2* pX = xPtr.get();

        // Create another UniquePointer
        UniquePointer<Struct2> yPtr(new Struct2(234));

        EXPECT_TRUE(xPtr);
        EXPECT_EQ(123,xPtr->value);
        EXPECT_EQ(2, Struct1::instances);

        EXPECT_TRUE(yPtr);
        EXPECT_EQ(234, yPtr->value);
        Struct2* pY = yPtr.get();

        EXPECT_NE(xPtr, yPtr);
        EXPECT_NE(pY->value, pX->value);
    }

    static void swap_pointer_test()
    {
        using namespace qe;
        // Create a UniquePointer
        UniquePointer<Struct2> xPtr(new Struct2(123));

        EXPECT_TRUE(xPtr);
        EXPECT_NE(nullptr, xPtr.get());
        EXPECT_EQ(123,xPtr->value);
        EXPECT_EQ(1, Struct1::instances);

        // Create another UniquePointer
        UniquePointer<Struct2> yPtr(new Struct2(234));

        EXPECT_TRUE(yPtr);
        EXPECT_EQ(234, yPtr->value);
        EXPECT_EQ(2, Struct1::instances);

        EXPECT_LT(xPtr->value, yPtr->value);
        xPtr.swap(yPtr);
        EXPECT_GT(xPtr->value, yPtr->value);
        EXPECT_TRUE(xPtr);
        EXPECT_TRUE(yPtr);
    }

    static void std_container_test()
    {
        using namespace qe;
        UniquePointer<Struct2> xPtr(new Struct2(123));

        EXPECT_TRUE(xPtr);
        EXPECT_EQ(123, xPtr->value);
        EXPECT_EQ(1, Struct1::instances);
        Struct2* pX = xPtr.get();

        {
            std::vector<UniquePointer<Struct2> > PtrList;

            // Move it inside a container, transferring ownership
            PtrList.push_back(std::move(xPtr));

            EXPECT_FALSE(xPtr);
            EXPECT_TRUE(PtrList.back());
            EXPECT_EQ(PtrList.back().get(), pX);
            EXPECT_EQ(Struct1::instances, 1);

        } // Destructor of the vector releases the last pointer thus destroying the object

        EXPECT_EQ(0, Struct1::instances);
    }

};
#endif // QE_TEST_UNIQUEPOINTER_H
