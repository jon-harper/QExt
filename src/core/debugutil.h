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

#ifndef QE_CORE_DEBUGUTIL_H
#define QE_CORE_DEBUGUTIL_H

#include <qecore/global.h>

#include <QString>
#include <QVariant>

namespace qe
{
    QString fromVariant(const QVariant &var);

    QString toHex(const quint64 value) { return QString::number(value, 16); }
    QString toHex(const qint64 value)  { return QString::number(value, 16); }
    QString toHex(const quint32 value) { return QString::number(value, 16); }
    QString toHex(const qint32 value)  { return QString::number(value, 16); }
    QString toHex(const quint16 value) { return QString::number(uint(value), 16); }
    QString toHex(const qint16 value)  { return QString::number(int(value), 16); }
    QString toHex(const quint8 value)  { return QString::number(uint(value), 16); }
    QString toHex(const qint8 value)   { return QString::number(int(value), 16); }
};
#endif // QE_CORE_DEBUGUTIL_H
