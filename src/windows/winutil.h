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
