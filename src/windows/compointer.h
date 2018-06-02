#ifndef QE_WINDOWS_COMPOINTER_H
#define QE_WINDOWS_COMPOINTER_H

#include <qewindows/global.h>
#include <combaseapi.h>
#include <shtypes.h>
#include <strsafe.h>
#include <QtCore/QMetaType>
#include <qecore/managedpointer.h>

namespace qe {
namespace windows {

namespace detail {

std::size_t stringLen(wchar_t * str)
{
    std::size_t len = 0;
    ::StringCchLengthW(str, STRSAFE_MAX_CCH, &len);
    return len;
}

std::size_t stringSize(wchar_t *str)
{
    std::size_t len = 0;
    ::StringCbLengthW(str, STRSAFE_MAX_CCH * sizeof(wchar_t), &len);
    return len;
}

wchar_t *stringCopy(wchar_t *str)
{
    auto ret = static_cast<wchar_t *>(CoTaskMemAlloc(stringSize(str)));
    ::StringCchCopyW(ret, STRSAFE_MAX_CCH, str);
    return ret;
}

}

//! Deleter struct for \ref ComPointer using `CoTaskMemFree()`.
template <class T>
struct ComDeleter
{
    static void cleanup(T *pointer) {
        if (pointer)
            CoTaskMemFree(static_cast<void *>(pointer));
    }
};

struct WCharManager : ComDeleter<wchar_t>
{
    static wchar_t *copy(wchar_t *pointer)
    {
        if (pointer)
            return detail::stringCopy(pointer);
        return nullptr;
    }
};

//! Manager for BStrPointer using `SysFreeString` and `SysAllocString`.
struct BStrManager {
    //! Copies a string using `SysAllocString`.
    static BSTR copy(BSTR pointer) {
        if (pointer) {
            return ::SysAllocString(pointer);
        }
        return nullptr;
    }
    //! Destroys a string using `SysFreeString`.
    static void cleanup(BSTR pointer) {
        if (pointer)
            ::SysFreeString(pointer);
    }
};

//! Specialization of \ref qe::UniquePointer that uses `CoTaskMemFree()` during destruction.
template <class T>
using ComPointer = qe::UniquePointer<T, ComDeleter<T>>;

//! Specialization of \ref qe::ManagedPointer for BSTRs using \ref BStrDeleter.
using BStrPointer = qe::ManagedPointer<OLECHAR, BStrManager>;

//! Specialization of \ref qe::ManagedPointer for BSTRs using \ref WCharManager.
using WCharPointer = qe::ManagedPointer<WCHAR, WCharManager>;

} // namespace windows
} // namespace qe

Q_DECLARE_SMART_POINTER_METATYPE(qe::windows::ComPointer)
Q_DECLARE_METATYPE(qe::windows::BStrPointer)
Q_DECLARE_METATYPE(qe::windows::WCharPointer)

#ifndef QEXT_NO_CLUTTER
template <class T>
using QeComPointer = qe::windows::ComPointer<T>;

template <class T>
using QeComDeleter = qe::windows::ComDeleter<T>;

using QeBStrPointer = qe::windows::BStrPointer;
using QeWCharPointer = qe::windows::WCharPointer;
#endif

#endif // QE_WINDOWS_COMPOINTER_H
