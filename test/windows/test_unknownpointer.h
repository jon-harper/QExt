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

#ifndef QE_TEST_UNKNOWNPOINTER_H
#define QE_TEST_UNKNOWNPOINTER_H

#include <ShlObj.h>

#include <qewindows/unknownpointer.h>
#include "../core/test.h"

struct FakeUnknown
{
    HRESULT QueryInterface(REFIID riid, void **ppvObject);
    ULONG AddRef() { count++; return count; }
    ULONG STDMETHODCALLTYPE Release() { count--; return count; }

    explicit FakeUnknown(int aVal) : count(1), value(aVal) {}

    ULONG count;
    int value;
};

using namespace qe::windows;

struct unknown_pointer_test
{
    static void empty_pointer()
    {
        // Create an empty (ie. nullptr) UniquePointer
        UnknownPointer<FakeUnknown> xPtr = nullptr;

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(nullptr, xPtr.get());

        // Reset to nullptr (ie. do nothing)
        xPtr.reset();

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(nullptr,  xPtr.get());
    }

    static void test_desktop()
    {
        UnknownPointer<IShellFolder> xPtr;
        EXPECT_EQ(S_OK, SHGetDesktopFolder(xPtr.addressOf()));

        EXPECT_NE(nullptr, xPtr);

        auto yPtr = xPtr.queryInterface<IShellFolder2>();

        EXPECT_NE(nullptr, yPtr);

//TODO: Create test case for CoCreateInstance

//        EXPECT_NE(nullptr, zPtr);
    }

    static void basic_pointer_test()
    {
        // Create a UniquePointer
        FakeUnknown fake(123);
        UnknownPointer<FakeUnknown> xPtr(&fake);

        EXPECT_TRUE(xPtr);
        EXPECT_NE(nullptr, xPtr.get());

        if (xPtr)
        {
            EXPECT_EQ(123,  xPtr->value);
            EXPECT_EQ(1,    xPtr->count);

            // Copy construct the UniquePointer, transferring ownership
            UnknownPointer<FakeUnknown> yPtr(std::move(xPtr));
            xPtr.reset();

            EXPECT_NE(xPtr, yPtr);
            EXPECT_FALSE(xPtr);
            EXPECT_EQ(nullptr,  xPtr.get());
            EXPECT_TRUE(yPtr);
            EXPECT_NE(nullptr,  yPtr.get());
            EXPECT_EQ(123,   yPtr->value);
            EXPECT_EQ(1,     yPtr->count);

            if (yPtr)
            {
                UnknownPointer<FakeUnknown> zPtr = std::move(yPtr);
                yPtr.reset();

                EXPECT_NE(yPtr,  zPtr);
                EXPECT_FALSE(yPtr);
                EXPECT_EQ(nullptr, yPtr.get());
                EXPECT_TRUE(zPtr);
                EXPECT_NE(nullptr,  zPtr.get());
                EXPECT_EQ(123,   zPtr->value);
                EXPECT_EQ(1,     zPtr->count);
            }

            EXPECT_FALSE(xPtr);
            EXPECT_EQ(nullptr, xPtr.get());
            EXPECT_FALSE(yPtr);
            EXPECT_EQ(nullptr, yPtr.get());
        }
        else
        {
            assert(false); //"bool cast operator error"
        }

        EXPECT_FALSE(xPtr);
        EXPECT_EQ(nullptr, xPtr.get());

    }

    static void run()
    {
        empty_pointer();
        basic_pointer_test();
        test_desktop();
    }
};


#endif // QE_TEST_UNKNOWNPOINTER_H
