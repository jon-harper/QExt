#ifndef QE_WINDOWS_COMPOINTER_H
#define QE_WINDOWS_COMPOINTER_H

#include <combaseapi.h>
#include <shtypes.h>
#include <qecore/uniquepointer.h>
#include <qewindows/global.h>

namespace qe {
namespace windows {

//! Deleter struct for \ref ComPointer using `CoTaskMemFree()`.
template <class T>
struct ComDeleter
{
    static void cleanup(T *pointer) {
        if (pointer)
            CoTaskMemFree(static_cast<void *>(pointer));
    }
};

//! Deleter struct for \ref BStrPointer using `SysFreeString`.
struct BStrDeleter {
    static void cleanup(BSTR pointer) {
        if (pointer)
            SysFreeString(pointer);
    }
};

//! Specialization of \ref qe::UniquePointer that uses `CoTaskMemFree()` during destruction.
template <class T>
using ComPointer = qe::UniquePointer<T, ComDeleter<T>>;

//! Specialization of \ref qe::UniquePointer for BSTRs using \ref BStrDeleter.
using BStrPointer = qe::UniquePointer<OLECHAR, BStrDeleter>;

//! Specialization of \ref qe::UniquePointer for BSTRs using \ref ComDeleter.
using WCharPointer = qe::UniquePointer<WCHAR, ComDeleter<WCHAR>>;

using IdListPointer = qe::UniquePointer<ITEMIDLIST_ABSOLUTE, ComDeleter<ITEMIDLIST_ABSOLUTE>>;

} // namespace windows
} // namespace qe

#ifndef QEXT_NO_CLUTTER
template <class T>
using QeComPointer = qe::windows::ComPointer<T>;

template <class T>
using QeComDeleter = qe::windows::ComDeleter<T>;

using QeBStrPointer = qe::windows::BStrPointer;
using QeWCharPointer = qe::windows::WCharPointer;
using QeIdListPointer = qe::windows::IdListPointer;
#endif

#endif // QE_WINDOWS_COMPOINTER_H
