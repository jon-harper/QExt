#ifndef QE_WINDOWS_WINUTIL_H
#define QE_WINDOWS_WINUTIL_H

#include <wtypes.h>
#include <QtCore/QString>
#include <QtCore/QRect>
#include <QtCore/QDateTime>
#include <QtGui/QColor>
#include <QtCore/QVariant>
#include <qewindows/global.h>

namespace qe {
namespace windows {
namespace util {

QE_WINDOWS_EXPORT QString hresultToString(HRESULT err);
QE_WINDOWS_EXPORT QString win32ErrorToString(DWORD err);

QE_WINDOWS_EXPORT QRect fromRECT(const RECT &r);
QE_WINDOWS_EXPORT QString fromBSTR(BSTR str);
QE_WINDOWS_EXPORT QDateTime fromFILETIME(const FILETIME &ft);
QE_WINDOWS_EXPORT QColor fromCOLORREF(const COLORREF &ref);
QE_WINDOWS_EXPORT COLORREF toCOLORREF(const QColor &color);

QE_WINDOWS_EXPORT QVariant fromVARIANT(VARIANT &v);
QE_WINDOWS_EXPORT QVariant fromPROPVARIANT(PROPVARIANT &v);

} // namespace util
} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_WINUTIL_H
