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
#include <QVariant>
#include <qewindows/unknownpointer.h>
#include "../core/test.h"


struct FakeUnknown
{
    HRESULT QueryInterface(REFIID riid, void **ppvObject) {
        (void)riid; (void)ppvObject; return S_FALSE;
    }
    ULONG AddRef() { count++; return count; }
    ULONG STDMETHODCALLTYPE Release() { count--; return count; }

    explicit FakeUnknown(int aVal) : count(1), value(aVal) {}

    ULONG count;
    int value;
};
Q_DECLARE_METATYPE(qe::windows::UnknownPointer<FakeUnknown>);

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

            // Move the UniquePointer, transferring ownership
            UnknownPointer<FakeUnknown> yPtr(std::move(xPtr));
            xPtr.reset();

            EXPECT_NE(xPtr, yPtr);
            EXPECT_FALSE(xPtr);
            EXPECT_TRUE(yPtr);
            EXPECT_EQ(123,   yPtr->value);
            EXPECT_EQ(1,     yPtr->count);

            if (yPtr)
            {
                //Test move operator
                UnknownPointer<FakeUnknown> zPtr = std::move(yPtr);
                yPtr.reset();

                EXPECT_NE(yPtr, zPtr);
                EXPECT_FALSE(yPtr);
                EXPECT_TRUE(zPtr);
                EXPECT_EQ(123,   zPtr->value);
                EXPECT_EQ(1,     zPtr->count);
            }

            if (yPtr)
            {
                //Test copy operator
                UnknownPointer<FakeUnknown> zPtr = yPtr;

                EXPECT_EQ(yPtr, zPtr);
                EXPECT_EQ(123,   zPtr->value);
                EXPECT_EQ(2,     zPtr->count);
            }

            EXPECT_FALSE(xPtr);
            EXPECT_FALSE(yPtr);
        }
        else
        {
            assert(false); //"bool cast operator error"
        }

        EXPECT_FALSE(xPtr);
    }

    static void test_qt()
    {
        using pointer = qe::windows::UnknownPointer<FakeUnknown>;
        FakeUnknown unk(2);
        auto ptr = pointer(&unk);

        EXPECT_TRUE(ptr);

        QVariant var = QVariant::fromValue<pointer>(ptr);

        EXPECT_TRUE(var.isValid());

        ptr = var.value<pointer>();

        EXPECT_TRUE(ptr);
    }

    static void run()
    {
        empty_pointer();
        basic_pointer_test();
        test_desktop();
        test_qt();
    }
};


#endif // QE_TEST_UNKNOWNPOINTER_H
