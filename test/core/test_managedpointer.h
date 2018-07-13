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

#ifndef QE_TEST_MANAGEDPOINTER_H
#define QE_TEST_MANAGEDPOINTER_H

#include <vector>
#include <qecore/managedpointer.h>
#include "test.h"

using namespace qe;

struct Dummy
{
    explicit Dummy(int aVal)
        : value(aVal)
    {
        instances++;
    }

    Dummy(const Dummy &other)
    {
        instances++;
        value = other.value;
    }
    Dummy(Dummy &&) = default;

    ~Dummy()
    {
        instances--;
    }

    Dummy &operator =(const Dummy &other)
    {
        instances++;
        value = other.value;
        return *this;
    }
    Dummy &operator =(Dummy &&) = default;

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

int Dummy::instances = 0;

struct Dummy2 : public Dummy
{
    explicit Dummy2(const int aVal)
        : Dummy(aVal)
    {

    }
};

struct managed_pointer_test
{
    static void run()
    {
        empty_pointer_test();
        basic_pointer_test();
    }

    //! Basic `nullptr` tests
    static void empty_pointer_test()
    {
        using pointer = ManagedPointer<Dummy2, DefaultManager<Dummy2>>;
        pointer xPtr = nullptr;

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(nullptr, xPtr.get());

        // Reset to nullptr (ie. do nothing)
        xPtr.reset();

        EXPECT_FALSE(xPtr);
    }

    static void basic_pointer_test()
    {
        {
            using pointer = ManagedPointer<Dummy2, DefaultManager<Dummy2>>;
            pointer xPtr(new Dummy2(123));

            EXPECT_TRUE(xPtr);
            EXPECT_NE(nullptr, xPtr.get());

            if (xPtr)
            {
                EXPECT_EQ(123, xPtr->value);
                EXPECT_EQ(1, xPtr->instances);
                EXPECT_EQ(1, Dummy::instances);

                // call a function
                xPtr->incr();
                EXPECT_EQ(124,  xPtr->value);
                (*xPtr).incr();
                EXPECT_EQ(125,  (*xPtr).value);
                xPtr->decr();
                xPtr->decr();

                // Move construct, transferring ownership
                pointer yPtr{std::move(xPtr)};
                xPtr.reset();

                EXPECT_NE(xPtr, yPtr);
                EXPECT_FALSE(xPtr);
                EXPECT_TRUE(yPtr);
                EXPECT_EQ(123,   yPtr->value);
                EXPECT_EQ(1,     Dummy::instances);

                {
                    //move initialization
                    pointer zPtr;
                    zPtr = std::move(yPtr);
                    yPtr.reset();

                    EXPECT_NE(yPtr,  zPtr);
                    EXPECT_FALSE(yPtr);
                    EXPECT_TRUE(zPtr);
                    EXPECT_EQ(123,   zPtr->value);
                    EXPECT_EQ(1,     Dummy::instances);

                    // test copy initialization
                    yPtr = zPtr;
                    EXPECT_TRUE(zPtr);
                    EXPECT_TRUE(yPtr);

                    yPtr.reset();
                    EXPECT_FALSE(yPtr);

                    //copy initialization
                    yPtr = std::move(zPtr);
                    EXPECT_TRUE(yPtr);
                    EXPECT_FALSE(zPtr);
                    yPtr.reset();
                }

                EXPECT_FALSE(xPtr);
                EXPECT_FALSE(yPtr);
                EXPECT_EQ(0, Dummy::instances);
            }
            else
            {
                assert(false); //"bool cast operator error"
            }

            EXPECT_FALSE(xPtr);
            EXPECT_EQ(0, Dummy::instances);
        }

        EXPECT_EQ(0, Dummy::instances);
    }
};

#endif // QE_TEST_MANAGEDPOINTER_H
