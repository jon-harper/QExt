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

/*!
 \headerfile debugutil.h <qecore/debugutil.h>
 \brief Provides simple helper classes for debugging purposes.
*/

#ifndef QE_CORE_DEBUGUTIL_H
#define QE_CORE_DEBUGUTIL_H

#ifndef QEXT_CORE_NO_QT
#include <qecore/global.h>

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace qe
{
    QString toString(const QVariant &var);

    inline QString toHexString(const quint64 value) { return QString::number(value, 16); }
    inline QString toHexString(const qint64 value)  { return QString::number(value, 16); }
    inline QString toHexString(const quint32 value) { return QString::number(value, 16); }
    inline QString toHexString(const qint32 value)  { return QString::number(value, 16); }
    inline QString toHexString(const quint16 value) { return QString::number(uint(value), 16); }
    inline QString toHexString(const qint16 value)  { return QString::number(int(value), 16); }
    inline QString toHexString(const quint8 value)  { return QString::number(uint(value), 16); }
    inline QString toHexString(const qint8 value)   { return QString::number(int(value), 16); }
};

#include <QMetaType>
#include <QBrush>
#include <QFont>
#include <QUuid>
#include <QRegion>
#include <QRect>

QString qe::toString(const QVariant &var)
{
    QString ret;
    switch(static_cast<QMetaType::Type>(var.type())) {
    case QMetaType::QString:
    case QMetaType::Bool:
    case QMetaType::QByteArray:
    case QMetaType::QChar:
    case QMetaType::QDate:
    case QMetaType::QDateTime:
    case QMetaType::QStringList:
    case QMetaType::QTime:
    case QMetaType::Double:
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        ret = var.toString();
        break;
    case QMetaType::Float:
        ret = QString::number(var.value<qreal>());
        break;
    case QMetaType::Short:
        ret = QString::number(var.value<short>());
        break;
    case QMetaType::UShort:
        ret = QString::number(var.value<ushort>());
        break;
    case QMetaType::SChar:
        ret = QString::number(var.value<signed char>());
        break;
    case QMetaType::QPoint:
    {
        QPoint p = var.value<QPoint>();
        ret = QString(QStringLiteral("(%1, %2)")).arg(p.x()).arg(p.y());
        break;
    }
    case QMetaType::QSize:
    {
        QSize sz = var.value<QSize>();
        ret = QString(QStringLiteral("%1 x %2")).arg(sz.width()).arg(sz.height());
        break;
    }
    case QMetaType::QRect:
    {
        QRect r = var.value<QRect>();
        ret = QString(QStringLiteral("[(%1, %2), %3 x %4]")).arg(
                    r.left()).arg(r.top()).arg(r.width()).arg(r.height());
        break;
    }
    case QMetaType::QFont:
    {
        QFont f = var.value<QFont>();
        ret = QString(QStringLiteral("[%1, %2]")).arg(f.family()).arg(f.pointSize());
        break;
    }
    case QMetaType::QUuid:
        ret = var.value<QUuid>().toString();
        break;
    default:
        break;
    }
    return ret;
}
#endif // QEXT_CORE_NO_QT
#endif // QE_CORE_DEBUGUTIL_H
