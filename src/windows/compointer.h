#ifndef QE_WINDOWS_COMPOINTER_H
#define QE_WINDOWS_COMPOINTER_H

#include <combaseapi.h>
#include <qecore/uniquepointer.h>
#include <qewindows/global.h>

namespace qe {
namespace windows {

/*!
    \brief Deleter struct for \ref ComPointer using `CoTaskMemFree()`.
 */
template <class T>
struct ComDeleter {
    static inline void cleanup(T *pointer) {
        if (pointer)
            CoTaskMemFree(reinterpret_cast<void *>(pointer));
    }
};

//!\brief  Specialization of \ref qe::UniquePointer that uses `CoTaskMemFree()` during destruction.
 template <class T>
using ComPointer = qe::UniquePointer<T, ComDeleter<T>>;

//! \brief Deleter struct for \ref BStrPointer using `SysFreeString`.
struct BStrDeleter {
    static inline void cleanup(BSTR pointer)
    {
        if (pointer)
            SysFreeString(pointer);
    }
};

//! \brief  Specialization of \ref qe::UniquePointer for BSTRs using \ref BStrDeleter.
using BStrPointer = qe::UniquePointer<OLECHAR, BStrDeleter>;

//! \brief  Specialization of \ref qe::UniquePointer for BSTRs using \ref ComDeleter.
using WCharPointer = qe::UniquePointer<WCHAR, ComDeleter<WCHAR>>;

} // namespace windows
} // namespace qe

#ifndef QEXT_NO_CLUTTER
template <class T>
using QeComPointer = qe::windows::ComPointer<T>;

template <class T>
using QeComDeleter = qe::windows::ComDeleter<T>;

using QeBStrPointer = qe::windows::BStrPointer;
using QeWCharPointer = qe::windows::WCharPointer;
#endif

#endif // QE_WINDOWS_COMPOINTER_H
